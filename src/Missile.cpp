//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Missile.cpp

  Overview:
  Missile model

  Author:
  David Bruce

*/
//############################################################################//

#include "Missile.hpp"

//
// C/C++ includes and namespaces
//
#include <stdio.h>
#include <math.h>


//
// Sim includes and namespaces
//
//#include "genMath.h"
//using namespace genMath;
//#include "genMathConstants.h"


//
// Reference objects
//
#include "Autopilot.hpp"
#include "SimExec.hpp"



//############################################################################//
//
// Function:  Missile()
//                                
// Purpose:   Constructor
//            Set default values of input variables.
//                                
//############################################################################//
Missile::Missile() :
MdlBase()
{
  //
  // Default initialization of input variables
  //
  frameTimeStep = 999.0e9;
  initFrameTime = 999.0e9;

//  initVxb = 270.0;
//  launchTime = 3.0;

  initVxb = 1.0e3;
  launchTime = 30.0;

  //
  // Register states for integration
  //
  RegisterState( pxf, vxf);
  RegisterState( pyf, vyf);

  RegisterState( vxf, axf);
  RegisterState( vyf, ayf);

}  // end Missile()

//############################################################################//
//
// Function: </b>  ~Missile()
//
// Purpose: </b>   Destructor
//
//############################################################################//
Missile::~Missile()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void Missile::Initialize()
{
  ResetAll();

  simTime = GetSimTime();
  
  nextFrameTime = initFrameTime;

  vxb = 0.0;
  vyb = 0.0;

  vxf = vxb;
  vyf = vxb;

  pxf = -( pExec->tFinal - launchTime ) * initVxb;
  pyf = 0.0;

}  // End Initialize()

//############################################################################//
//
// Function: FrameUpdate
//
// Purpose:  Perform frame update
//
//############################################################################//
void Missile::FrameUpdate()
{
  simTime = GetSimTime();
}

//############################################################################//
//
// Function: Update
//
// Purpose:  Update the state
//
//############################################################################//
void Missile::Update()
{
  simTime = GetSimTime();
  
  if ( (simTime > launchTime) && !initialized )
  {
    initialized = true;

    vxb = initVxb;
    vyb = 0.0;

    vxf = vxb;
    vyf = vyb;

  }
}  // end Update()

//############################################################################//
//
// Function: UpdateDerivatives()
//
// Purpose:  Update the state
//
//############################################################################//
void Missile::UpdateDerivatives()
{
  double cth;
  double sth;

  simTime = GetSimTime();

  ayb = pAp->ayb;

  theta = atan2( vyf, vxf );
  cth = cos( theta );
  sth = sin( theta );

  axf = axb * cth - ayb * sth;
  ayf = axb * sth + ayb * cth;

}  // end UpdateDerivatives()

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out model
//
//############################################################################//
void Missile::Finalize()
{
  simTime = GetSimTime();
} 

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print selected variables
//
//############################################################################//
void Missile::Output( bool printHeader,
                     FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "msl.axb" );
    fprintf( pOutFile, "%20s", "msl.axf" );
    fprintf( pOutFile, "%20s", "msl.ayb" );
    fprintf( pOutFile, "%20s", "msl.ayf" );
    fprintf( pOutFile, "%20s", "msl.launchTime" );
    fprintf( pOutFile, "%20s", "msl.pxf" );
    fprintf( pOutFile, "%20s", "msl.pyf" );
    fprintf( pOutFile, "%20s", "msl.theta" );
    fprintf( pOutFile, "%20s", "msl.vxb" );
    fprintf( pOutFile, "%20s", "msl.vxf" );
    fprintf( pOutFile, "%20s", "msl.vyb" );
    fprintf( pOutFile, "%20s", "msl.vyf" );
  }
  else
  {
    fprintf(pOutFile, "%20.6e", axb);
    fprintf(pOutFile, "%20.6e", axf);
    fprintf(pOutFile, "%20.6e", ayb);
    fprintf(pOutFile, "%20.6e", ayf);
    fprintf(pOutFile, "%20.6e", launchTime);
    fprintf(pOutFile, "%20.6e", pxf);
    fprintf(pOutFile, "%20.6e", pyf);
    fprintf(pOutFile, "%20.6e", theta);
    fprintf(pOutFile, "%20.6e", vxb);
    fprintf(pOutFile, "%20.6e", vxf);
    fprintf(pOutFile, "%20.6e", vyb);
    fprintf(pOutFile, "%20.6e", vyf);

  } // end if ( printHeader ) else
}  // end Output()

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void Missile::ResetAll()
{
  axb         = 0.0;
  axf         = 0.0;
  ayb         = 0.0;
  ayf         = 0.0;
  initialized = false;
  pxf         = 0.0;
  pyf         = 0.0;
  simTime     = 0.0;
  theta       = 0.0;
  vxb         = 0.0;
  vxf         = 0.0;
  vyb         = 0.0;
  vyf         = 0.0;

}  // end ResetAll()


//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
