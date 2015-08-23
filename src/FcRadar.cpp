//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  FcRadar.cpp

  Author:
  David Bruce

  Overview:
  Fire control radar

*/
//############################################################################//

#include "FcRadar.hpp"


//
// C/C++ includes
//
#include <cmath>

//
// Reference includes
//
#include "SimExec.hpp"
#include "Target.hpp"

//
// Other sim includes
//
#include "MathUtils.hpp"
//#include "genMath.h"
//#include "genMathConstants.h"
//using namespace genMath;


//############################################################################//
//
// Function:  FcRadar()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
FcRadar::FcRadar()
{
  //
  // Input parameter defaults
  //
  frameTimeStep = 1.0/64.0;
  initFrameTime = 0.0;

  initPxf = -10e3;;
  initPyf = 0.0;

  rinAng = 500.0e-6; // rad   Range independent angle noise
  rinRng = 3.0;      // m     Range independent range noise
  rinRdot = 3.0;     // m/sec Range independent range rate noise

  //
  // Other variable initialization
  //

}

//############################################################################//
//
// Function:  ~FcRadar()
//
// Purpose:   Destructor
//
//
//############################################################################//
FcRadar::~FcRadar()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void FcRadar::Initialize()
{
  ResetAll();

  simTime = GetSimTime();

  nextFrameTime = initFrameTime;

  pxf = initPxf;
  pyf = initPyf;

}  // end Initialize()

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform frame update
//
//############################################################################//
void FcRadar::FrameUpdate()
{
  simTime = GetSimTime();

  truPosX = pTgt->pxf;
  truPosY = pTgt->pyf;

  truVelX = pTgt->vxf - vxf;
  truVelY = pTgt->vyf - vyf;

  relPosX = truPosX - pxf;
  relPosY = truPosY - pyf;

  relVelX = truVelX;
  relVelY = truVelY;

  truAng = atan2( relPosY, relPosX );

  truRng = sqrt( SQ(relPosX) + SQ(relPosY) );

  truRdot = ( relVelX * relPosX + relVelY * relPosY ) / truRng;
  
  measAng = gaussian( truAng, rinAng, &pExec->seed );
              
  measRdot = gaussian( truRdot, rinRdot, &pExec->seed );

  measRng = gaussian( truRng, rinRng, &pExec->seed );

  timeStamp = simTime;
              
}  // end FrameUpdate()

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
void FcRadar::Update()
{
}

//############################################################################//
//
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
void FcRadar::UpdateDerivatives()
{
}

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
void FcRadar::Finalize()
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
void FcRadar::Output( bool printHeader,
                      FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "fcRdr.measAng");
    fprintf( pOutFile, "%20s", "fcRdr.measRdot");
    fprintf( pOutFile, "%20s", "fcRdr.measRng");
    fprintf( pOutFile, "%20s", "fcRdr.timeStamp");

    fprintf( pOutFile, "%20s", "fcRdr.pxf");
    fprintf( pOutFile, "%20s", "fcRdr.pyf");

    fprintf( pOutFile, "%20s", "fcRdr.rinAng");
    fprintf( pOutFile, "%20s", "fcRdr.rinRdot");
    fprintf( pOutFile, "%20s", "fcRdr.rinRng");

    fprintf( pOutFile, "%20s", "fcRdr.truAng");
    fprintf( pOutFile, "%20s", "fcRdr.truPosY");
    fprintf( pOutFile, "%20s", "fcRdr.truRdot");
    fprintf( pOutFile, "%20s", "fcRdr.truRng");

    fprintf( pOutFile, "%20s", "fcRdr.vxf");
    fprintf( pOutFile, "%20s", "fcRdr.vyf");
  }
  else
  {
    fprintf( pOutFile, "%20.6e", measAng );
    fprintf( pOutFile, "%20.6e", measRdot );
    fprintf( pOutFile, "%20.6e", measRng );
    fprintf( pOutFile, "%20.6e", timeStamp );

    fprintf( pOutFile, "%20.6e", pxf );
    fprintf( pOutFile, "%20.6e", pyf );

    fprintf( pOutFile, "%20.6e", rinAng );
    fprintf( pOutFile, "%20.6e", rinRdot );
    fprintf( pOutFile, "%20.6e", rinRng );

    fprintf( pOutFile, "%20.6e", truAng );
    fprintf( pOutFile, "%20.6e", truPosY );
    fprintf( pOutFile, "%20.6e", truRdot );
    fprintf( pOutFile, "%20.6e", truRng );
    fprintf( pOutFile, "%20.6e", vxf );
    fprintf( pOutFile, "%20.6e", vyf );
  }
}  // end Output()


//
// Class scope functions
//

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void FcRadar::ResetAll()
{
  measAng   = 0.0;
  measRng   = 0.0;
  measRdot  = 0.0;
  pxf       = 0.0;
  pyf       = 0.0; 
  truAng    = 0.0;
  truPosX   = 0.0;
  truPosY   = 0.0;
  truRng    = 0.0;
  truRdot   = 0.0;
  vxf       = 0.0;
  vyf       = 0.0; 
  
}


//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

