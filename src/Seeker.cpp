//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Seeker.cpp

  Author:
  David Bruce

  Overview:
  Projectile seeker

*/
//############################################################################//

#include "Seeker.hpp"


//
// C/C++ includes
//
#include <cmath>

//
// Reference includes
//
#include "Missile.hpp"
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
// Function:  Seeker()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
Seeker::Seeker() :
pExec(0),
pMsl(0),
pTgt(0),
MdlBase()
{
  //
  // Input parameter defaults
  //
  frameTimeStep = 1.0/180.0; // sec    Frame update interval
  initFrameTime = 0.0;       // sec    Time of first FrameUpdate

  detRng = 10.0e3;           // m      Detection / acquisition range  
  fov = 20.0 * DEG2RAD;      // rad    Field of view     
  rinAng = 0.2e-3;           // rad    Range independent angle noise

  //
  // Other variable initialization
  //

}

//############################################################################//
//
// Function:  ~Seeker()
//
// Purpose:   Destructor
//
//
//############################################################################//
Seeker::~Seeker()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void Seeker::Initialize()
{
  ResetAll();

  simTime = GetSimTime();

  initFrameTime = pMsl->launchTime;

  nextFrameTime = initFrameTime;

  pxf = pMsl->pxf;
  pyf = pMsl->pyf;

}  // end Initialize()

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform frame update
//
//############################################################################//
void Seeker::FrameUpdate()
{
  simTime = GetSimTime();

  pxf = pMsl->pxf;
  pyf = pMsl->pyf;

  vxf = pMsl->vxf;
  vyf = pMsl->vyf;

  truPosX = pTgt->pxf;
  truPosY = pTgt->pyf;

  truVelX = pTgt->vxf;
  truVelY = pTgt->vyf;

  relPosX = truPosX - pxf;
  relPosY = truPosY - pyf;

  relVelX = truVelX - vxf;
  relVelY = truVelY - vyf;

  truAng = atan2( relPosY, relPosX );

  truRng = sqrt( SQ(relPosX) + SQ(relPosY) );

  truRdot = ( relVelX * relPosX + relVelY * relPosY ) / truRng;

  if ( truRng < detRng )
  {
    measAng = gaussian( truAng, rinAng, &pExec->seed );
    measValid = true;      
  }
  else
  {
    measAng = 0.0;
    measValid = false;      
  }
  
  timeStamp = simTime;
              
}  // end FrameUpdate()

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
void Seeker::Update()
{
}

//############################################################################//
//
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
void Seeker::UpdateDerivatives()
{
}

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
void Seeker::Finalize()
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
void Seeker::Output( bool printHeader,
                      FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "skr.measAng");
    fprintf( pOutFile, "%20s", "skr.measValid");
    fprintf( pOutFile, "%20s", "skr.rinAng");
    fprintf( pOutFile, "%20s", "skr.timeStamp");


    fprintf( pOutFile, "%20s", "skr.truAng");
    fprintf( pOutFile, "%20s", "skr.truRdot");
    fprintf( pOutFile, "%20s", "skr.truRng");

    fprintf( pOutFile, "%20s", "skr.vxf");
    fprintf( pOutFile, "%20s", "skr.vyf");
  }
  else
  {
    fprintf( pOutFile, "%20.6e", measAng );
    fprintf( pOutFile, "%20.6d", measValid );
    fprintf( pOutFile, "%20.6e", rinAng );
    fprintf( pOutFile, "%20.6e", timeStamp );

    fprintf( pOutFile, "%20.6e", truAng );
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
void Seeker::ResetAll()
{
  measAng   = 0.0;
  measValid = false;
  pxf       = 0.0;
  pyf       = 0.0;
  relPosX   = 0.0;
  relPosY   = 0.0;
  relVelX   = 0.0;
  relVelY   = 0.0;
  simTime   = 0.0;
  timeStamp = 0.0; 
  truAng    = 0.0;
  truPosX   = 0.0;
  truPosY   = 0.0;
  truRdot   = 0.0;
  truRng    = 0.0;
  truVelX   = 0.0;
  truVelY   = 0.0;
  vxf       = 0.0;
  vyf       = 0.0; 
  
}


//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

