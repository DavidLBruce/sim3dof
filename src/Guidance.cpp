//############################################################################//
//                                UNCLASSIFIED                                //
/*
  Guidance.cpp

  Overview:
  Missile guidance model

  Author:
  David Bruce

*/
//############################################################################//

#include "Guidance.hpp"

//
// C/C++ includes and namespaces
//
#include <cassert>
#include <math.h>
#include <stdio.h>


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
#include "FcRadar.hpp"
#include "FcTse6.hpp"
#include "FcTseO.hpp"
#include "SimExec.hpp"
#include "Missile.hpp"
#include "MslTse6.hpp"
#include "Target.hpp"


const double SMALL = 1.0e-6;


//############################################################################//
//
// Function:  Guidance()
//                                
// Purpose:   Constructor
//            Set default values of input variables.
//                                
//############################################################################//
Guidance::Guidance() :
pAp(0),
pExec(0),
pMsl(0),
pMslTse(0),
pRdr(0),
pTgt(0),
pTse6(0),
MdlBase()
{
  //
  // Default initialization of input variables
  //
  frameTimeStep = 1.0/64.0;  // sec     Frame time step
  initFrameTime = 0.0;       // sec     Time of first frame update

  gdnLawSelInt  = 1;         // na      Guidance law selection
                             //         1 = Proportion Navigation
                             //         2 = Weave guidance
  kpn           = 3.0;       // na      Pro Nav gain
  tauCtrl       = 0.4;       // sec     Optimal guidance tuning parameter
  useTruth      = false;     // na      Use truth for target state

}  // end Guidance()

//############################################################################//
//
// Function: </b>  ~Guidance()
//
// Purpose: </b>   Destructor
//
//############################################################################//
Guidance::~Guidance()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void Guidance::Initialize()
{
  ResetAll();

  simTime = GetSimTime();

  assert( pAp );
  assert( pExec );
  assert( pMsl );
  assert( pMslTse );
  assert( pRdr );
  assert( pTgt );
  assert( pTse6 );

  gdnLawSel = static_cast<GdnLawSel>( gdnLawSelInt );

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
void Guidance::FrameUpdate()
{
  double axl;     // m/sec^2 Absolute target acceleration in LOS frame
  double ayl;     // m/sec^2 Absolute target acceleration in LOS frame
  double cth;     // na      Cosine of LOS angle
  double den;     // na      Intermediate variable
  double estPosYdot; // m/sec Estimated cross range rate
  double jxl;     // m/sec^3 Absolute target jerk in LOS frame
  double jyl;     // m/sec^3 Absolute target jerk in LOS frame
  double mslAxl;  // m/sec^2 Missile acceleration in LOS frame
  double mslAyl;  // m/sec^2 Missile acceleration in LOS frame
  double num;     // na      Intermediate variable
  double omega;   // rad/sec Weave radian frequency
  double omega2;  // na      Square of omega        
  double omega3;  // na      Cube of omega
  double pxl;     // m       Relative position in LOS frame
  double pyl;     // m       Relative position in LOS frame
  double sth;     // na      Sine of LOS angle
  double tgtJxf;  // m/sec^3 Target absolute jerk
  double tgtJyf;  // m/sec^3 Target absolute jerk
  double vxl;     // m/sec   Relative velocity in LOS frame
  double vyl;     // m/sec   Relative velocity in LOS frame
  double xx;      // na      Intermediate variable
  double x2;      // na      Intermediate variable
  double x3;      // na      Intermediate variable

  simTime = GetSimTime();

  if ( useTruth )
  {
    estAccX = pMslTse->truAccX;
    estAccY = pMslTse->truAccY;
    estLosAng = pMslTse->truLosAng;
    estLosr = pMslTse->truLosr;
    estRng = pMslTse->truRng;
    estRdot = pMslTse->truRdot;

    tgtPxf = pTgt->pxf;
    tgtPyf = pTgt->pyf;

    tgtVxf = pTgt->vxf;
    tgtVyf = pTgt->vyf;

    tgtAxf = pTgt->axf;
    tgtAyf = pTgt->ayf;

    tgtJxf = pTgt->jxf;
    tgtJyf = pTgt->jxf;

    omega = pTgt->omega;
  }
  else
  {
    tgtPxf = pMslTse->estTgtPxf;
    tgtPyf = pMslTse->estTgtPyf;
    
    tgtVxf = pMslTse->estTgtVxf;
    tgtVyf = pMslTse->estTgtVyf;
    
    tgtAxf = pMslTse->estTgtAxf;
    tgtAyf = pMslTse->estTgtAxf;

    omega = pMslTse->dlOmega;

    omega2 = pMslTse->dlOmega2;

    estAccX = pMslTse->estAccX;
    estAccY = pMslTse->estAccY;
    estLosAng = pMslTse->estLosAng;
    estLosr = pMslTse->estLosr;
    estRng = pMslTse->estRng;
    estRdot = pMslTse->estRdot;

    tgtJxf = pMslTse->estTgtJxf;
    tgtJyf = pMslTse->estTgtJyf;
  }

  //
  // Calculate target state wrt missile
  //
  // Using truth for missile state
  //
  relPxf = tgtPxf - pMsl->pxf;
  relPyf = tgtPyf - pMsl->pyf;

  relVxf = tgtVxf - pMsl->vxf;
  relVyf = tgtVyf - pMsl->vyf;

  if ( estRdot < 0.0 )
  {
    velClose = -estRdot;

    tgo = -estRng / estRdot;
  }
  // Else leave tgo & Vc at the last calculation

  switch ( gdnLawSel )
  {
    case glPN:
    {
      //
      // Lateral proportional navigation command.  Note 
      // inertial losr = body losr in this planar sim.
      // The body frame and the inertial frame are in the same plane.
      //
      aCmdYb = kpn * velClose * estLosr;

      break;

    }  // end case glPN:

    case glWEAVE:
    {
      //
      // Optimal weaving target guide law from Zarchan
      //
 
      //
      // Rotate variables into LOS frame     
      //
      cth = cos( estLosAng );
      sth = sin( estLosAng );

      pxl = relPxf * cth - relPyf * sth;
      pyl = relPxf * sth + relPyf * cth;
      pyl = relPyf;

      vxl = relVxf * cth - relPyf * sth;
      vyl = relVxf * sth + relPyf * cth;
      vyl = relVyf;

      axl = tgtAxf * cth - tgtAyf * sth;
      ayl = tgtAxf * sth + tgtAyf * cth;

      jxl = tgtJxf * cth - tgtJyf * sth;
      jyl = tgtJxf * sth + tgtJyf * cth;

      mslAxl = pMsl->axf * cth - pMsl->ayf * sth;
      mslAyl = pMsl->axf * sth + pMsl->ayf * cth;

      xx = tgo / tauCtrl;

      x2 = xx * xx;
      x3 = x2 * xx;

      num = 6.0*x2 * ( exp(-xx) - 1 + xx );

      den = 2.0*x3 - 6.0*x2 + 6.0*xx + 3.0 - 
            12.0*xx * exp(-xx) - 3.0 * exp(-2.0*xx);

      kWeave = num / den;

      //
      // Position and velocity terms are relative.  Accelearation and jerk terms
      // are absolute.
      //
      aCmdYb = pyl + tgo * vyl;

      omega2 = omega * omega;
      omega3 = omega2 * omega;

      aCmdYb += ( 1.0 - cos(omega * tgo ) ) * ayl / omega2;

      aCmdYb += ( omega * tgo - sin(omega * tgo) ) * jyl / omega3;

      aCmdYb -= mslAyl * tauCtrl*tauCtrl * ( exp(-xx) - 1 + xx );

      aCmdYb *= kWeave / ( tgo * tgo );

      break;

    }  // end case glWEAVE:
  }  // end switch ( gdnLawSel )
} // end FrameUpdate()

//############################################################################//
//
// Function: Update
//
// Purpose:  Update the state
//
//############################################################################//
void Guidance::Update()
{
  simTime = GetSimTime();
  
}  // end Update()

//############################################################################//
//
// Function: UpdateDerivatives()
//
// Purpose:  Update the state
//
//############################################################################//
void Guidance::UpdateDerivatives()
{
  simTime = GetSimTime();

}  // end UpdateDerivatives()

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out model
//
//############################################################################//
void Guidance::Finalize()
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
void Guidance::Output( bool printHeader,
                       FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "gdn.aCmdYb" );
    fprintf( pOutFile, "%20s", "gdn.losr" );
    fprintf( pOutFile, "%20s", "gdn.rDot" );
    fprintf( pOutFile, "%20s", "gdn.relPxf" );
    fprintf( pOutFile, "%20s", "gdn.relPyf" );
    fprintf( pOutFile, "%20s", "gdn.relVxf" );
    fprintf( pOutFile, "%20s", "gdn.relVyf" );
    fprintf( pOutFile, "%20s", "gdn.rng" );
  }
  else
  {
    fprintf(pOutFile, "%20.6e", aCmdYb );
    fprintf(pOutFile, "%20.6e", estLosr );
    fprintf(pOutFile, "%20.6e", estRdot );
    fprintf(pOutFile, "%20.6e", relPxf );
    fprintf(pOutFile, "%20.6e", relPyf );
    fprintf(pOutFile, "%20.6e", relVxf );
    fprintf(pOutFile, "%20.6e", relVyf );
    fprintf(pOutFile, "%20.6e", estRng );

  } // end if ( printHeader ) else
}  // end Output()

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void Guidance::ResetAll()
{
  aCmdYb         = 0.0;
  estAccX        = 0.0;
  estAccY        = 0.0;
  estLosAng      = 0.0;
  estLosr        = 0.0;
  estRng         = 0.0;
  estRdot        = 0.0;
  gdnLawSel      = glNULL;
  initialized    = false;
  kWeave         = 0.0;
  launchTime     = 0.0;
  relPxf         = 0.0;        
  relPyf         = 0.0;        
  relVxf         = 0.0;        
  relVyf         = 0.0;        
  simTime        = 0.0;
  tgo            = 0.0;       
  tgtAxf         = 0.0;        
  tgtAyf         = 0.0;        
  tgtPxf         = 0.0;        
  tgtPyf         = 0.0;        
  tgtVxf         = 0.0;        
  tgtVyf         = 0.0;        
  velClose       = 0.0;      

}  // end ResetAll()


//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
