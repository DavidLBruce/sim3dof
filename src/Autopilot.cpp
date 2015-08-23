//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Autopilot.cpp

  Overview:
  Missile Autopilot model.  3rd order transfer function.

  Author:
  David Bruce

*/
//############################################################################//

#include "Autopilot.hpp"

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
//#include "genMathStatLib.h"

#include "MathUtils.hpp"

//
// Reference objects
//
#include "Guidance.hpp"
#include "Missile.hpp"
#include "SimExec.hpp"
//#include "Navigation.hpp"


//############################################################################//
//
// Function:  Autopilot()
//                                
// Purpose:   Constructor
//            Set default values of input variables.
//                                
//############################################################################//
Autopilot::Autopilot() :
MdlBase()
{
  //
  // Default initialization of input variables
  //
  frameTimeStep = 1.0/64.0;  // sec     Frame time step
  initFrameTime = 0.0;       // sec     Time of first frame update

  accLim = 300.0;            // m/sec^2 Acceleration limit
  tau = 0.2;                 // sec     Control system time constant
                             //         Autopilot, Aero, & CAS
  //
  // Register states for integration
  //
  RegisterState( x0, x0d );
  RegisterState( x1, x1d );
  RegisterState( x2, x2d );

}  // end Autopilot()

//############################################################################//
//
// Function: </b>  ~Autopilot()
//
// Purpose: </b>   Destructor
//
//############################################################################//
Autopilot::~Autopilot()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void Autopilot::Initialize()
{
  ResetAll();

  simTime = GetSimTime();

  launchTime = pMsl->launchTime;

  nextFrameTime = launchTime;

}  // End Initialize()

//############################################################################//
//
// Function: FrameUpdate
//
// Purpose:  Perform frame update
//
//############################################################################//
void Autopilot::FrameUpdate()
{
  aCmdYb = pGdn->aCmdYb;

  aCmdYbLim = maxi( -accLim, mini(aCmdYb, accLim) );

} // end FrameUpdate()

//############################################################################//
//
// Function: Update
//
// Purpose:  Update the state
//
//############################################################################//
void Autopilot::Update()
{
  simTime = GetSimTime();

  ayb = x0;

}  // end Update()

//############################################################################//
//
// Function: UpdateDerivatives()
//
// Purpose:  Update the state
//
//############################################################################//
void Autopilot::UpdateDerivatives()
{
  double tauPrime;

  simTime = GetSimTime();

  //
  // Third order transfer function.  Ref Zarchan 3rd ed. page 98.
  // Three identical single order systems in series.
  //
  tauPrime = tau / 3.0 / 1.09;

  x0d = -1.0/tauPrime * x0 + 1.0/tauPrime * x1;

  x1d = -1.0/tauPrime * x1 + 1.0/tauPrime * x2;

  x2d = -1.0/tauPrime * x2 + 1.0/tauPrime * aCmdYbLim;

}  // end UpdateDerivatives()

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out model
//
//############################################################################//
void Autopilot::Finalize()
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
void Autopilot::Output( bool printHeader,
                        FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "ap.accLim" );
    fprintf( pOutFile, "%20s", "ap.aCmdYbLim");
    fprintf( pOutFile, "%20s", "ap.ayb" );
    fprintf( pOutFile, "%20s", "ap.tau" );
  }
  else
  {
    fprintf(pOutFile, "%20.6e", accLim );
    fprintf(pOutFile, "%20.6e", aCmdYbLim );
    fprintf(pOutFile, "%20.6e", ayb );
    fprintf(pOutFile, "%20.6e", tau );

  } // end if ( printHeader ) else
}  // end Output()

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void Autopilot::ResetAll()
{
  ayb         = 0.0;
  aCmdYb      = 0.0;
  aCmdYbLim   = 0.0;
  launchTime  = 0.0;
  simTime     = 0.0;
  x0          = 0.0;
  x0d         = 0.0;
  x1          = 0.0;
  x1d         = 0.0;
  x2          = 0.0;
  x2d         = 0.0;

}  // end ResetAll()


//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
