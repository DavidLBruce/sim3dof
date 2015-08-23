//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  SimExec.cpp

  Overview:
  Simulation executive

  Author:
  David Bruce

*/
//############################################################################//

#include "SimExec.hpp"


//
// C/C++ includes and namespaces
//
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include <string>
#include <cmath>
#include <ctype.h>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
#include <algorithm>
using std::sort;

//
// Sim includes
//
#include "MathUtils.hpp"
//#include "genMath.h"
//#include "genMathConstants.h"
//using namespace genMath;



const double LARGE = 999.0e9;
const double SMALL = 1.0e-6;

//############################################################################//
//
// Function:  SimExec()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
SimExec::SimExec() :
MdlBase(),
ap(),
gdn(),
fcRdr(),
fcTseO(),
fcTse6(),
msl(),
mslTse6(),
skr(),
tgt()
{
  //
  // Default initialization of input variables
  //
  fileStub[0]    = '\0';  // na      Output file name stub
  flyerThld      = 10.0;  // m       Threshold for flyer miss distance
  maxTime        = 100.0; // sec     Maximum amount of simulation time
  outputRate     = -1.0;  // Hz      Trajectory data output rate
  pHitThld       = 1.0;   // m       Miss threshold for pHit calculation
  printAll       = false; // na      Flag directing to print all Monte
                          //         Carlo run miss data
  runs           = 1;     // na      Number of runs in the Monte Carlo set
  runStart       = 1;     // na      First run to be made resolved
  timeStep       = 1.0e-3;// sec     Simulation time step  
  xa             = 0.0;   // rad     Crossing angle.  Suplement of angle between
                          //         velocity vectors.

  tFinal         = 50.0;  // sec     Sim duration

}  // end SimExec()

//############################################################################//
//
// Function: </b>  ~SimExec()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
SimExec::~SimExec()
{

}  // end ~SimExec()

///////////////////////////////////////////////////////////////////////////////
//
// AddModel()
//
// Overview:
//
// Add module to module vector
//
///////////////////////////////////////////////////////////////////////////////
void SimExec::AddModel( MdlBase *pMdl, string mdlName )
{
  mdlVec.push_back( pMdl );

  //
  // Pointer to simExec's time
  //
  pMdl->SetSimTime( &simTime );

  pMdl->mdlName = mdlName;

}  // end AddModel()

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void  SimExec::Initialize()
{
  int idx;

  ResetAll();

  for ( idx = 0; idx < mdlVec.size(); idx++ )
  {
    mdlVec[idx]->Initialize();
  }

  outputTimeStep = 1.0 / outputRate;
}

//############################################################################//
//
// Function: Integrate()                                     
//                                                                       
// Purpose:  Integrate the dynamic models with the simulation time step
//                                                                           
//############################################################################//
void SimExec::Integrate()
{
  int idx;
  MdlBase *pMdl;
  

  for ( idx = 0; idx < mdlVec.size(); ++idx )
  {
    pMdl = mdlVec[idx];
    if ( pMdl->intEn )
    {
      pMdl->Integrate( deltaTime );
    }
  }
}

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
void SimExec::Update()
{
  int idx;

  for ( idx = 0; idx < mdlVec.size(); ++idx )
  {
    mdlVec[idx]->Update();
  }

} // end Update()

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform model frame updates
//
//############################################################################//
void SimExec::FrameUpdate()
{
  int    idx;
  double temp;

  //
  // Write selected variables to output file
  //
  if ( (simTime >= outputTime) && (pOutFile != NULL) )
  {
    Output( false, pOutFile );

    outputTime += outputTimeStep;
  }

  //
  // Start next time step calculation with the sim time step.
  //
  deltaTime = timeStep;

  if ( simTime >= nextFrameTime - SMALL )
  {
    nextFrameTime += timeStep;
  }

  temp = nextFrameTime - simTime;
  if ( deltaTime - temp > SMALL )
  {
    deltaTime = temp;
  }

  //
  // Check output time step
  //
  if (pOutFile != NULL )    
  {
    temp = outputTime - simTime;
    if ( deltaTime - temp > SMALL )
    {
      deltaTime = temp;
    }
  }

  //
  // Perform model frame updates.
  //
  for ( idx = 0; idx < mdlVec.size(); ++idx )
  {
    pMdl = mdlVec[idx];

    if ( simTime >= pMdl->nextFrameTime - SMALL )
    {
      pMdl->FrameUpdate();

      pMdl->nextFrameTime += pMdl->frameTimeStep;
    }

    //
    //  Check the models for the next time step.
    //
    temp = pMdl->nextFrameTime - simTime;
    if ( deltaTime - temp > SMALL )
    {
      deltaTime = temp;
    }

  }  // end for ( idx = 0; idx < mdlVec.size(); ++idx )

} // end FrameUpdate()

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
void SimExec::Finalize()
{
  int idx;

  for ( idx = 0; idx < mdlVec.size(); ++idx )
  {
    mdlVec[idx]->Finalize();
  }

}  // end Finalize()

//############################################################################//
//
// Function: Execute()
//
// Purpose:  Run the simulation
//
//############################################################################//
void SimExec::Execute( int argc, char *argv[] )
{
  double angA;      // rad Angle between relative position and velocity vectors.
  double dummy;     // na  Throw away variable
  int    idx;       // na  Loop variable
  double overShoot; // m   Distance of travel beyond the closest point of
                    //     approach.

  strcpy( fileStub, argv[0] );

  process_command_line(argc, argv);

  //
  // Set up all the sim modules
  //

  //
  // Target
  //
  AddModel( &tgt, "Target" );
  tgt.SetSimExec( this );

  //
  // Fire Control Radar
  //
  AddModel( &fcRdr, "FcRdr" );
  fcRdr.SetSimExec( this );
  fcRdr.SetTarget( &tgt );

  //
  // Fire Control Target State Estimator
  //
  AddModel( &fcTseO, "fcTseO" );
  fcTseO.SetSimExec( this );
  fcTseO.SetRadar( &fcRdr );

  //
  // Fire Control six state Target State Estimator
  //
  AddModel( &fcTse6, "fcTse6" );
  fcTse6.SetSimExec( this );
  fcTse6.SetRadar( &fcRdr );
  fcTse6.SetTarget( &tgt ); 

  //
  // Projectile
  //
  AddModel( &msl, "madFires" );
  msl.SetSimExec( this );
  msl.SetAutopilot( &ap );

  //
  // Seeker
  //
  AddModel( &skr, "Seeker" );
  skr.SetMissile( &msl );
  skr.SetSimExec( this );
  skr.SetTarget( &tgt );

  //
  // Projectile MSC TSE
  //
  AddModel( &mslTse6, "mslTse6" );
  mslTse6.SetSimExec( this );
  mslTse6.SetFcTse6( &fcTse6 );
  mslTse6.SetFcTseO( &fcTseO );
  mslTse6.SetMissile( &msl );
  mslTse6.SetFcRadar( &fcRdr );
  mslTse6.SetSeeker( &skr );
  mslTse6.SetTarget( &tgt );

  //
  // Guidance
  //
  AddModel( &gdn, "guidance" );
  gdn.SetSimExec( this );
  gdn.SetMissile( &msl );
  gdn.SetRadar( &fcRdr );
  gdn.SetTarget( &tgt );
  gdn.SetTse6( &fcTse6 ); 
  gdn.SetMslTse( &mslTse6 ); 
  gdn.SetAutopilot( &ap ); 

  AddModel( &ap, "autopilot" );
  ap.SetSimExec( this );
  ap.SetMissile( &msl );
  ap.SetGuidance( &gdn ); 

  //
  //
  // 
  nonFlyerCnt  = 0;
  avgMissMag   = 0.0;
  avgMissX     = 0.0;
  avgMissY     = 0.0;
  avgXang      = 0.0;
  hitCnt       = 0;
  runIdx       = 0;

  //
  // Set size of vector containers.  Hopefully this is faster than push_back()
  //
  missVec.insert(missVec.end(), runs, 0.0);
  missNonFlyers.insert(missNonFlyers.end(), runs, 0.0);
  missXvec.insert(missXvec.end(), runs, 0.0);
  missYvec.insert(missYvec.end(), runs, 0.0);

  if ( printAll )  
  {
    printf( "%5s", "run" );
    printf( "%10s", "miss" );
    printf( "%10s", "tof" );
    printf( "%10s", "seed" );
    printf( "\n" );

    fflush( NULL );
  }

  for ( run = runStart; run < runStart + runs; run++ )
  {
    //
    // Initialize random number generator
    //
    seed = -abs(2 * run - 1) * 13579;
    runSeed = seed;

    //
    // "Warm up" random number seed
    //
    for ( idx = 0; idx < 500; ++idx )
    {
      uniform( 0.0, 1.0, &seed );
    }

    //
    // Run simulation
    //

    //
    // Initialize the modules
    //
    Initialize();

    sprintf( filename, "%s", fileStub );

    sprintf( filename, "%s-%05d.dat", filename, run );

    if ( outputRate > 0.0 )
    {
      pOutFile = fopen( filename, "w" );
    }

    //
    // Print the output file header if necessary
    //
    if ( pOutFile != NULL )
    {
      Output( true, pOutFile );
    }

    deltaTime = timeStep;

    //
    // Main simulation loop
    //
    done = false;
    while ( !done )
    {
      //
      // Update variables in preparation for derivative calculations.
      //
      Update();
    
      Integrate();

      //
      // Update variables with the resultes of integration.
      //
      Update();

      //
      // Frame updates can run asynchrously with respect to eash other and the
      // base simulation run rate.  Dynamics integration will be current at the
      // time of any frame update.
      //
      FrameUpdate();

      relPxf = tgt.pxf - msl.pxf;
      relPyf = tgt.pyf - msl.pyf;

      range = sqrt( SQ(relPxf) + SQ(relPyf) );

      relVxf = tgt.vxf - msl.vxf;
      relVyf = tgt.vyf - msl.vyf;

      relVmag = sqrt( SQ(relVxf) + SQ(relVyf) );

      rDot = ( relVxf * relPxf + relVyf * relPyf ) / range;

      if ( rDot >= 0.0 )
      {
        done = true;
     
        //
        // Calculate the miss distance
        //

        //
        // Use dot product to calculate the angle between relative position and
        // velocity vectors.
        //
        if ( range == 0.0 )
        {
          angA = 0.0;
          miss = 0.0;
          overShoot = 0.0;
          missX = 0.0;
          missY = 0.0;
        }
        else
        {
          angA = acos( (relPxf * relVxf + relPyf * relVyf) / (range * relVmag) );
          miss = range * sin( angA );

          overShoot = range * cos(angA);

          missX = relPxf - overShoot * relVxf / relVmag;
          missY = relPyf - overShoot * relVyf / relVmag;
        }
      }  // end if ( rDot >= 0.0 )
      else if ( simTime >= maxTime )
      {
        done = true;
        
        miss = missX = missY = LARGE;
      }

      if ( !done )
      {
        simTime += deltaTime;
      }
    
    }  // end while ( !done )
    
    //
    // Do final dump of data
    //
    if ( pOutFile != NULL ) 
    {
      Output( false, pOutFile );

      fclose( pOutFile );
    }

    //
    // Collect miss data
    //
    missVec[runIdx] = miss;
    if ( miss < flyerThld )
    {
      ++nonFlyerCnt;
      missNonFlyers[nonFlyerCnt-1] = miss;
      missXvec[nonFlyerCnt-1] = missX;
      missYvec[nonFlyerCnt-1] = missY;

      avgMissMag = ( avgMissMag * static_cast<double>(nonFlyerCnt - 1) +
                                                  fabs(miss) ) /  nonFlyerCnt;
      avgMissX = ( avgMissX * static_cast<double>(nonFlyerCnt - 1) + missX ) /
                                                                  nonFlyerCnt;
      avgMissY = ( avgMissY * static_cast<double>(nonFlyerCnt - 1) + missY ) /
                                                                  nonFlyerCnt;

      //
      // Crossing angle. Supplement of angle between tgt & msl velocity vectors.
      //
      Xang = acos( (msl.vxf * tgt.vxf + msl.vyf * tgt.vyf) /
              sqrt((msl.vxf*msl.vxf + msl.vyf*msl.vyf) *
                   (tgt.vxf*tgt.vxf + tgt.vyf*tgt.vyf)) );

      Xang = PI - Xang;

      avgXang = ( avgXang * static_cast<double>(nonFlyerCnt - 1) + Xang ) /
                                                                  nonFlyerCnt;

    }

    //
    // Count hits for pHit calculation
    //
    if ( miss < pHitThld )
    {
      ++hitCnt;
    }

    if ( printAll )  
    {
      if ( miss < flyerThld )
      {
        printf( "%5d",    run );
        printf( "%10.3f", miss );
        printf( "%10.3f", simTime );
        printf( "%10d",   runSeed );
        printf( "\n" );
        fflush( NULL );
      }
      else
      {
        printf( "%5d",    run );
        printf( "%10.3e", miss );
        printf( "%10.3f", simTime );
        printf( "%10d",   runSeed );
        printf( "\n" );
        fflush( NULL );
      }
    }
    
    if ( runIdx < runs - 1 )
    {
      ++runIdx;
    }

  }  // end for ( run = runStart; run < runStart + runs; run++ )

  pHit = static_cast<double>( hitCnt ) / static_cast<double>( runs );

  sort( missVec.begin(), missVec.end() );
  if ( modf(static_cast<double>(runs) / 2.0, &dummy) < SMALL )
  {
    medMiss = ( missVec[runs/2] + missVec[runs/2+1] ) / 2.0;
  }
  else
  {
    medMiss = missVec[(int)((runs-1)/2)];
  }

  if ( runs > 1 )
  {
    miss70 = missVec[(int)(runs*0.7)-1];
    miss20 = missVec[(int)(runs*0.2)-1];
    miss35 = missVec[(int)(runs*0.35)-1];
  }
  else
  {
    miss70 = missVec[0];
    miss20 = missVec[0];
    miss35 = missVec[0];
  }

  minMiss = missVec[0];
  maxMiss = missVec[runs-1];

  stdMissX = CalcStdDev( missXvec );
  stdMissY = CalcStdDev( missYvec );

  qtyFly = runs - nonFlyerCnt;

  printf( "%10s ", "runs" );
  printf( "%10s ", "pHit" );
  printf( "%10s ", "pHitThld" );
  printf( "%10s ", "miss70" );
  printf( "%10s ", "medMiss" );
  printf( "%10s ", "miss35" );
  printf( "%10s ", "miss20" );
  printf( "%10s ", "avgMissMag" );
  printf( "%10s ", "minMiss" );
  printf( "%10s ", "maxMiss" );
  printf( "%10s ", "avgMissX" );
  printf( "%10s ", "avgMissY" );
  printf( "%10s ", "stdMissX" );
  printf( "%10s ", "stdMissY" );
  printf( "%10s ", "avgXang" );
  printf( "%10s ", "qtyFly" );
  printf( "\n" );

  printf( "%10d ",   runs );  
  printf( "%10.3f",  pHit );
  printf( "%10.3f ", pHitThld );
  printf( "%10.3f ", miss70 );
  printf( "%10.3f ", medMiss );
  printf( "%10.3f ", miss35 );
  printf( "%10.3f ", miss20 );
  printf( "%10.3f ", avgMissMag );
  printf( "%10.3f ", minMiss );
  printf( "%10.3f ", maxMiss );
  printf( "%10.3f ", avgMissX );
  printf( "%10.3f ", avgMissY );
  printf( "%10.3f ", stdMissX );
  printf( "%10.3f ", stdMissY );
  printf( "%10.3f ", avgXang );
  printf( "%10d ",   qtyFly );
  printf( "\n " );

}  // end Execute()
      
//############################################################################//
//
// Function: Output()
//
// Purpose:  Run the simulation
//
//############################################################################//
void SimExec::Output( bool  printHeader,
                      FILE *pOutFile )
{
  int idx;

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "simTime");
    fprintf( pOutFile, "%20s", "seed");
  }
  else
  {
    fprintf( pOutFile, "%20.6e", simTime ); 
    fprintf( pOutFile, "%20d", runSeed ); 
  }

  for ( idx = 0; idx < mdlVec.size(); ++idx )
  {
    mdlVec[idx]->Output( printHeader,
                         pOutFile );
  }

  fprintf( pOutFile, "\n" );

}  // end Output()

////////////////////////////////////////////////////////////////////////////////
//
// process_command_line()
//
// Process command line function
//
////////////////////////////////////////////////////////////////////////////////
void SimExec::process_command_line( int argc,
                                    char *argv[] )
{
  int  i;
  char name[512];
  char val[512];

  if (argc > 1) 
  {
    for (i=1; i<=argc-1; i++)
    {
      get_args(argv[i], name, val);

      parse_symbols(name, val);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// read_file( char *filename )
//
// Overview:
//
//  Read the input file
//
///////////////////////////////////////////////////////////////////////////////
void SimExec::read_file( char *filename )
{
  FILE *fp;
  char name[512];
  char val[512];
  char line[512];
  int  num_syms = 0;


  if ((fp = fopen(filename, "r")) != NULL) 
  {

    while (!feof(fp)) 
    {

      fgets(line, 512, fp);

      if ((line[0] != '#') && !(isspace(line[0]))) 
      {
        get_args(line, name, val);

        parse_symbols(name, val);

        num_syms++;
      }
    }

    fclose(fp);
  }
  else 
  {
    printf("Nonfatal error opening file %s\n", filename);
  }
}  // end read_file()

////////////////////////////////////////////////////////////////////////////////
//
// boolean_string()
//
// Convert boolean to a character array.
//
////////////////////////////////////////////////////////////////////////////////
const char* SimExec::boolean_string(int val)
{
  const char *TRUE_STR = "true";
  const char *FALSE_STR = "false";

  if (val)
    return TRUE_STR;
  else
    return FALSE_STR;
}

////////////////////////////////////////////////////////////////////////////////
//
// parse_symbols()
//
// Parse symbols from command line
//
////////////////////////////////////////////////////////////////////////////////
void SimExec::parse_symbols( char *name, char *val )
{

  //
  // SimExec inputs
  //
  if (strcmp(name, "ex.file")==0)                 read_file(val);
  else if (strcmp(name, "ex.dt")==0)              timeStep                     = atof(val);
  else if (strcmp(name, "ex.fileStub")==0)        sprintf(fileStub, "%s", val);
  else if (strcmp(name, "ex.flyerThld")==0)       flyerThld                    = atof(val);
  else if (strcmp(name, "help")==0)               Help();
  else if (strcmp(name, "ex.maxTime")==0)         maxTime                      = atof(val);
  else if (strcmp(name, "ex.outputRate")==0)      outputRate                   = atof(val);
  else if (strcmp(name, "ex.pHitThld")==0)        pHitThld                     = atof(val);
  else if (strcmp(name, "ex.printAll")==0)        printAll                     = get_boolean(val);
  else if (strcmp(name, "ex.runs")==0)            runs                         = atoi(val);
  else if (strcmp(name, "ex.runStart")==0)        runStart                     = atoi(val);
  else if (strcmp(name, "ex.tgo")==0)             tFinal                       = atof(val);
  else if (strcmp(name, "ex.xa")==0)              xa                           = atof(val)*d2r;

  //
  // Target inputs
  //
  else if (strcmp(name, "tgt.frameTimeStep")==0)  tgt.frameTimeStep            = atof(val);
  else if (strcmp(name, "tgt.initFrameTime")==0)  tgt.initFrameTime            = atof(val);
  else if (strcmp(name, "tgt.initVxb")==0)        tgt.initVxb                  = atof(val);
  else if (strcmp(name, "tgt.weaveStart")==0)     tgt.weaveStart               = atof(val);
  else if (strcmp(name, "tgt.weaveEnd")==0)       tgt.weaveEnd                 = atof(val);
  else if (strcmp(name, "tgt.weaveAmpl")==0)      tgt.weaveAmpl                = atof(val)*GRAVITY;
  else if (strcmp(name, "tgt.weavePeriod")==0)    tgt.weavePeriod              = atof(val);

  //
  // Fire Control radar inputs
  //
  else if (strcmp(name, "fcRdr.frameTimeStep")==0) fcRdr.frameTimeStep         = atof(val);
  else if (strcmp(name, "fcRdr.initFrameTime")==0) fcRdr.initFrameTime         = atof(val);
  else if (strcmp(name, "fcRdr.initPxf")==0)      fcRdr.initPxf                = atof(val);
  else if (strcmp(name, "fcRdr.initPyf")==0)      fcRdr.initPyf                = atof(val);
  else if (strcmp(name, "fcRdr.rinAng")==0)       fcRdr.rinAng                 = atof(val);
  else if (strcmp(name, "fcRdr.rinRdot")==0)      fcRdr.rinRdot                = atof(val);
  else if (strcmp(name, "fcRdr.rinRng")==0)       fcRdr.rinRng                 = atof(val);

  //
  // Fire Control TSE inputs
  //
  else if (strcmp(name, "fcTseO.frameTimeStep")==0) fcTseO.frameTimeStep       = atof(val);
  else if (strcmp(name, "fcTseO.initFrameTime")==0) fcTseO.initFrameTime       = atof(val);
  else if (strcmp(name, "fcTseO.initPosY")==0)    fcTseO.initPosY              = atof(val);
  else if (strcmp(name, "fcTseO.initPosYsigScale")==0) fcTseO.initPosYsigScale = atof(val);
  else if (strcmp(name, "fcTseO.initVelY")==0)    fcTseO.initVelY              = atof(val);
  else if (strcmp(name, "fcTseO.initVelYsig")==0) fcTseO.initVelYsig           = atof(val);
  else if (strcmp(name, "fcTseO.initOmega")==0)   fcTseO.initOmega             = atof(val);
  else if (strcmp(name, "fcTseO.initWsig")==0)    fcTseO.initWsig              = atof(val);
  else if (strcmp(name, "fcTseO.procNoise")==0)   fcTseO.procNoise             = atof(val);

  //
  // Fire Control TSE6 inputs
  //
  else if (strcmp(name, "fcTse6.frameTimeStep")==0) fcTse6.frameTimeStep       = atof(val);
  else if (strcmp(name, "fcTse6.initFrameTime")==0) fcTse6.initFrameTime       = atof(val);
  else if (strcmp(name, "fcTse6.singerTau")==0)   fcTse6.singerTau             = atof(val);
  else if (strcmp(name, "fcTse6.singerVar")==0)   fcTse6.singerVar             = atof(val);

  //
  // Missile inputs
  //
  else if (strcmp(name, "msl.frameTimeStep")==0)  msl.frameTimeStep            = atof(val);
  else if (strcmp(name, "msl.initFrameTime")==0)  msl.initFrameTime            = atof(val);
  else if (strcmp(name, "msl.initVxb")==0)        msl.initVxb                  = atof(val);
  else if (strcmp(name, "msl.launchTime")==0)     msl.launchTime               = atof(val);

  //
  // Seeker inputs
  //
  else if (strcmp(name, "skr.frameTimeStep")==0) skr.frameTimeStep             = atof(val);
  else if (strcmp(name, "skr.initFrameTime")==0) skr.initFrameTime             = atof(val);
  else if (strcmp(name, "skr.rinAng")==0) skr.rinAng                           = atof(val);
  else if (strcmp(name, "skr.fov")==0) skr.fov                                 = atof(val);
  else if (strcmp(name, "skr.detRng")==0) skr.detRng                           = atof(val);

  //
  // Projectile MSC TSE inputs
  //
  else if (strcmp(name, "mslTse6.frameTimeStep")==0) mslTse6.frameTimeStep     = atof(val);
  else if (strcmp(name, "mslTse6.initFrameTime")==0) mslTse6.initFrameTime     = atof(val);
  else if (strcmp(name, "mslTse6.datalinkStep")==0) mslTse6.datalinkStep       = atof(val);
  else if (strcmp(name, "mslTse6.nextUpLinkTime")==0) mslTse6.nextUpLinkTime   = atof(val);
  else if (strcmp(name, "mslTse6.singerTau")==0) mslTse6.singerTau             = atof(val);
  else if (strcmp(name, "mslTse6.singerVar")==0) mslTse6.singerVar             = atof(val);

  //
  // Guidance inputs
  //
  else if (strcmp(name, "gdn.frameTimeStep")==0)  gdn.frameTimeStep            = atof(val);
  else if (strcmp(name, "gdn.initFrameTime")==0)  gdn.initFrameTime            = atof(val);
  else if (strcmp(name, "gdn.gdnLawSelInt")==0)   gdn.gdnLawSelInt             = atoi(val);
  else if (strcmp(name, "gdn.kpn")==0)            gdn.kpn                      = atof(val);
  else if (strcmp(name, "gdn.tauCtrl")==0)        gdn.tauCtrl                  = atof(val);
  else if (strcmp(name, "gdn.useTruth")==0)       gdn.useTruth                 = get_boolean(val);

  //
  // Autopilot inputs
  //
  else if (strcmp(name, "ap.frameTimeStep")==0)   ap.frameTimeStep             = atof(val);
  else if (strcmp(name, "ap.initFrameTime")==0)   ap.initFrameTime             = atof(val);
  else if (strcmp(name, "ap.accLim")==0)          ap.accLim                    = atof(val);
  else if (strcmp(name, "ap.tau")==0)             ap.tau                       = atof(val);

  else
  {
    fprintf(stderr, "Unrecognized symbol %s, skipping...\n", name);
  }

  if ( strcmp(name, "fileStub") == 0 )
  {
    int tmpN;

    tmpN = strlen( fileStub );
    if (fileStub[tmpN-1] == '\n')
    {
      fileStub[tmpN-1]='\0';
    }
  }
}  // end parse_symbols

////////////////////////////////////////////////////////////////////////////////
//
// Help()
//
// Overview:
//
//  List the input parameters and their descriptions, then exit..
//
////////////////////////////////////////////////////////////////////////////////
void SimExec::Help()
{
  printf("peet help\n\n");
  printf("The sim is called from the command line and uses the following syntax\n\n");
  printf(" > peet arg1=val1 arg2=val2 arg3=val3 ...\n\n");
  printf("where arg* is one of the following:\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("   string        |             description              |   units   | value \n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Simulation Executive Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("ex.dt            | simulation time step                 |    sec    | %g \n", timeStep );
  printf("ex.file          | Take inputs from file                |  boolean  | \n");
  printf("ex.fileWtub      | Output filename header               |   string  | %s \n", fileStub );
  printf("ex.flyerThld     | Flyer miss distance threshold        |     m     | %g \n", flyerThld );
  printf("help             | Print input variables, units, default|    na     | \n");
  printf("ex.maxTtime      | Maximum simulation time              |    sec    | %g \n", maxTime );
  printf("ex.outputRate    | Simulation variable output rate      |     Hz    | %g \n", outputRate );
  printf("ex.pHitThld      | Miss threshold for pHit calculation  |     m     | %g \n", pHitThld );
  printf("ex.printAll      | Print all Monte Carlo misses         |  boolean  | %s \n", boolean_string(printAll) );
  printf("ex.runs          | runs per Monte Carlo set             |     1     | %d \n", runs );
  printf("ex.runStart      | First Monte Carlo run (1st run = 1)  |     1     | %d \n", runStart );
  printf("ex.tgo           | Intercept time from sim start        |    sec    | %g \n", tFinal );
  printf("ex.xa            | Crossing angle                       |    deg    | %g \n", xa*r2d );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Target Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("tgt.frameTimeStep| Target frame time step               |    sec    | %g \n", tgt.frameTimeStep );
  printf("tgt.initFrameTime| Target frame start time              |    sec    | %g \n", tgt.initFrameTime );
  printf("tgt.initVxb      | Target velocity                      |   m/sec   | %g \n", tgt.initVxb );
  printf("tgt.weaveStart   | Target weave start time              |    sec    | %g \n", tgt.weaveStart );
  printf("tgt.weaveEnd     | Target weave end time                |    sec    | %g \n", tgt.weaveEnd );
  printf("tgt.weaveAmpl    | Target weave amplitude               |   gees    | %g \n", tgt.weaveAmpl/GRAVITY );
  printf("tgt.weavePeriod  | Target weave period                  |    sec    | %g \n", tgt.weavePeriod );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Fire Control Radar Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("fcRdr.frameTimeStep| Fire Control radar frame time step |    sec    | %g \n", fcRdr.frameTimeStep );
  printf("fcRdr.initFrameTime| Fire Control radar frame start time|    sec    | %g \n", fcRdr.initFrameTime );
  printf("fcRdr.initPxf    | Initial position                     |     m     | %g \n", fcRdr.initPxf );
  printf("fcRdr.initPyf    | Initial position                     |     m     | %g \n", fcRdr.initPyf );
  printf("fcRdr.rinAng     | Range independent position noise     |    rad    | %g \n", fcRdr.rinAng );
  printf("fcRdr.rinRdot    | Range independent position noise     |     m     | %g \n", fcRdr.rinRdot );
  printf("fcRdr.rinRng     | Range independent position noise     |     m     | %g \n", fcRdr.rinRng) ;

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Fire Control TSE Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("fcTseO.frameTimeStep| Fire Control TSE frame time step  |    sec    | %g \n", fcTseO.frameTimeStep );
  printf("fcTseO.initFrameTime| Fire Control TSE frame start time |    sec    | %g \n", fcTseO.initFrameTime );
  printf("fcTseO.procNoise  | Fire Control TSE process noise      |   rad^2   | %g \n", fcTseO.procNoise );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Fire Control TSE6 Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("fcTse6.frameTimeStep| Fire Control TSE frame time step  |    sec    | %g \n", fcTse6.frameTimeStep );
  printf("fcTse6.initFrameTime| Fire Control TSE frame start time |    sec    | %g \n", fcTse6.initFrameTime );
  printf("fcTse6.singerTau| Fire Control TSE tuning parameter     |    sec    | %g \n", fcTse6.singerTau );
  printf("fcTse6.singerVar| Fire Control TSE tuning parameter     |    sec    | %g \n", fcTse6.singerVar );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Missile Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("msl.frameTimeStep| Missile frame time step              |    sec    | %g \n", msl.frameTimeStep );
  printf("msl.initFrameTime| Missile frame start time             |    sec    | %g \n", msl.initFrameTime );
  printf("msl.initVxb      | Missile velocity                     |   m/sec   | %g \n", msl.initVxb );
  printf("msl.launchTime   | Missile launch time                  |    sec    | %g \n", msl.launchTime );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Seeker Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("skr.frameTimeStep| Seeker frame time step               |    sec    | %g \n", skr.frameTimeStep );
  printf("skr.initFrameTime| Seeker frame start time              |    sec    | %g \n", skr.initFrameTime );
  printf("skr.detRng       | Seeker detection acquisition range   |     m     | %g \n", skr.detRng );
  printf("skr.fov          | Seeker field of view / regard        |    rad    | %g \n", skr.fov );
  printf("skr.rinAng       | Seeker range independent angle noise |    rad    | %g \n", skr.rinAng );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Projectile TSE6 Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("mslTse6.frameTimeStep| Seeker frame time step           |    sec    | %g \n", mslTse6.frameTimeStep );
  printf("mslTse6.initFrameTime| Seeker frame start time          |    sec    | %g \n", mslTse6.initFrameTime );
  printf("mslTse6.nextUpLinkTime| Datalink uplink interval        |    sec    | %g \n", mslTse6.datalinkStep );
  printf("mslTse6.singerTau| Process noise uning parameter        |    na     | %g \n", mslTse6.singerTau );
  printf("mslTse6.singerVar| Process noise uning parameter        |    na     | %g \n", mslTse6.singerVar );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Guidance Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("gdn.frameTimeStep| Guidance frame time step             |    sec    | %g \n", gdn.frameTimeStep );
  printf("gdn.initFrameTime| Guidance frame start time            |    sec    | %g \n", gdn.initFrameTime );
  printf("gdn.gdnLawSelInt | Guidance law selection 1=pn, 2=weave |    na     | %d \n", gdn.gdnLawSelInt );
  printf("gdn.kpn          | Pro Nav gain                         |    na     | %g \n", gdn.kpn);
  printf("gdn.tauCtrl      | Optimal guidance tuning parameter    |    sec    | %g \n", gdn.tauCtrl);
  printf("gdn.useTruth     | Use true target & missile state      |    na     | %s \n", boolean_string(gdn.useTruth) );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf(">Autopilot Model Parameters\n");
  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("ap.frameTimeStep| Autopilot frame time step             |    sec    | %g \n", ap.frameTimeStep );
  printf("ap.initFrameTime| Autopilot frame start time            |    sec    | %g \n", ap.initFrameTime );
  printf("ap.accLim       | Acceleration limit                    |  m/sec^2  | %g \n", ap.accLim );
  printf("ap.tau          | Total autopilot, aero, CAS time const |    sec    | %g \n", ap.tau );

  printf("-----------------+--------------------------------------+-----------+-------------\n");
  printf("\n");
  printf("Output is a single line of run set miss statistics:\n");
  printf("\n");
  exit(0);

}  // end help()

////////////////////////////////////////////////////////////////////////////////
//
// get_args( char *in, char *name, char *val )
//
// Overview:
//
//  Sub function of process command line
//
///////////////////////////////////////////////////////////////////////////////
void SimExec::get_args( char *in, char *name, char *val )
{
  int  i;
  int  length;

  char *ptr;

  length = strlen(in);
  if (length == 0)
  {
    strcpy(name, "");
    strcpy(val, "");
  }
  else
  {
    ptr = &in[0];

    i = 0;
    while ( (*ptr != '=') &&
            ( i < length) )
    {
      name[i] = *ptr;
      ptr++;
      i++;
    }

    name[i] = '\0';

    if (i == length)
    {
      val[0] = '\0';
    }
    else
    {
      ptr++;
      i = 0;

      while (*ptr!='\0')
      {
        val[i] = *ptr;
        ptr++;
        i++;
      }
      val[i] = '\0';
    }
  }

  ptr = NULL;

}  // end get_args()

////////////////////////////////////////////////////////////////////////////////
//
// get_boolean(char *str)
//
// Overview:
//
//  Retrieves a boolean from the command line
//
///////////////////////////////////////////////////////////////////////////////
int SimExec::get_boolean(char *str)
{
  if ( (strcmp(str, "0\n") == 0) ||
    (strcmp(str, "0") == 0) ||
    (strcmp(str, "false\n") == 0) ||
    (strcmp(str, "false") == 0) )
    return 0;

  if ( (strcmp(str, "1\n") == 0) ||
    (strcmp(str, "1") == 0) ||
    (strcmp(str, "true\n") == 0) ||
    (strcmp(str, "true") == 0) )
    return 1;

  printf("Error in input to get_boolean() - %s, exiting\n", str);

  exit(0);
}

///////////////////////////////////////////////////////////////////////////////
//
// get_list_index(char *val
//          char **list,
//          int max_index)
//
// Overview:
//
//  Retrieves a list from the command line
//
///////////////////////////////////////////////////////////////////////////////
int SimExec::get_list_index( char *val,
                             char **list,
                             int  max_index )
{
  int index_found = 0;
  int index = 0;
  int i, tmpN;

  if (isdigit(val[0]))
    return atoi(val);

  tmpN=strlen(val);
  if (val[tmpN-1]=='\n')
    val[tmpN-1]='\0';

  if (val[tmpN-1]=='\r')
    val[tmpN-1]='\0';

  // TBD: Not sure why this works, but seems necessary for unix runs
  //  val[tmpN-1]='\0';
  //  val[tmpN-2]='\0';

  while (!index_found && (index<max_index))
  {
    //    if ((strcmp(val, list[index]) == 0) || (((strncmp(val, list[index],strlen(val)-2) == 0)) & (InWindows < 1)) )
    if ((strcmp(val, list[index]) == 0) )
      index_found = 1;
    else
      index++;
  }

  if (!index_found)
  {
    printf("Invalid mode in get_list_index: %s chosen from { ", val);

    for (i=0; i < max_index; i++)
      printf("%s ", list[i]);

    printf("}\n");
    printf("... exiting\n");

    exit(0);

  }
  else
  {

    return index;
  }

}  // end get_list_index()

///////////////////////////////////////////////////////////////////////////////
//
// get_double( double *var,
//             char *val,
//             double scale_factor )
//
// Overview:
//
//  Retrieves a double from the command line
//
///////////////////////////////////////////////////////////////////////////////
double SimExec::get_double( double *var,
                            char   *val,
                            double scale_factor )
{
  *var = atof(val) * scale_factor;

  return *var;

}  // end get_double()


///////////////////////////////////////////////////////////////////////////////
//
// ResetAll()
//
// Overview:
//
//  Clear selected variables
//
///////////////////////////////////////////////////////////////////////////////
double SimExec::CalcStdDev( vectorDbl dataVec )
{
  int    idx;
  double mean = 0.0;
  int    qtyElem = dataVec.size();
  double stdDev = 0.0;

  if ( qtyElem != 1 && qtyElem != 0 )
  {
    for ( idx = 0; idx < qtyElem; ++idx )
    {
      mean += dataVec[idx];
    }
    mean /= qtyElem;

    for ( idx = 0; idx < qtyElem; ++idx )
    {
      stdDev += SQ( dataVec[idx] - mean );
    }
    stdDev = sqrt( stdDev / (static_cast<double>(qtyElem) - 1.0) );
  }
  return( stdDev );
}

///////////////////////////////////////////////////////////////////////////////
//
// ResetAll()
//
// Overview:
//
//  Clear selected variables
//
///////////////////////////////////////////////////////////////////////////////
void SimExec::ResetAll()
{
  deltaTime    = 0.0;
  done         = false;
  filename[0]  = '\0';
  maxMiss      = 0.0;
  medMiss      = 0.0;
  minMiss      = 0.0;
  miss         = 0.0;
  miss70       = 0.0;
  missX        = 0.0;
  missY        = 0.0;
  nextFrameTime = 0.0;
  outputTime   = 0.0;
  outputTimeStep = 0.0;
  pHit         = 0.0;
  pOutFile     = NULL;
  qtyFly       = 0;
  range        = 0.0;
  rDot         = 0.0;
  relPxf       = 0.0;
  relPyf       = 0.0;
  relVmag      = 0.0;
  relVxf       = 0.0;
  relVyf       = 0.0;
  simTime      = 0.0;
  stdMissX     = 0.0;
  stdMissY     = 0.0;

}  // end ResetAll()



//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
  
