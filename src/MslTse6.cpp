//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  \file   MslTse6.cpp

  \author David Bruce

  \brief  Projectile MSC Kalman Filter / Target State Estimator

*/
//############################################################################//

#include "MslTse6.hpp"


//
// C/C++ includes
//
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

//
// Reference includes
//
#include "FcRadar.hpp"
#include "FcTse6.hpp"
#include "FcTseO.hpp"
#include "Missile.hpp"
#include "Seeker.hpp"
#include "SimExec.hpp"
#include "Target.hpp"

//
// Other sim includes
//
//#include "genMath.h"
//#include "genMathStatLib.h"
//using namespace genMath;
#include "Matrix.hpp"
using namespace MATRIX;

#include "MathUtils.hpp"

const double SMALL = 1.0e-6;

//############################################################################//
//
// Function:  MslTse6()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
MslTse6::MslTse6() :
pMsl(0),
pExec(0),
pRdr(0),
pSkr(0),
pTgt(0),
pTse6(0),
pTseO(0),
MdlBase()
{
  //
  // Input parameter defaults
  //
  frameTimeStep = 1.0/64.0;
  initFrameTime = 0.0;

  datalinkStep = 1.0 / 64;        // Datalink uplink interval
  nextUpLinkTime = initFrameTime; // Next datalink uplink time
  singerTau = 0.3;                // Kalman process noise tuning parameter
  singerVar = 600.0;              // Kalman process noise tuning parameter

  //
  // Set matrix sizes
  //
  cartRel.Resize(6,1);
  truState.Resize(6,1);
  dlState.Resize(6,1);
  eye.Eye(6);
  HH.Resize(3,6);
  HT.Resize(6,3);
  innovation.Resize(6,1);
  KK.Resize(6,3);
  measVec.Resize(3,1);
  mscState.Resize(6,1);
  phi.Resize(6,6);
  PP.Resize(6,6);
  residual.Resize(3,1);
  RR.Resize(3,3);
   
}

//############################################################################//
//
// Function:  ~MslTse6()
//
// Purpose:   Destructor
//
//
//############################################################################//
MslTse6::~MslTse6()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void MslTse6::Initialize()
{
  int idx;

  ResetAll();

  simTime = GetSimTime();

  assert( pExec );
  assert( pMsl );
  assert( pRdr );
  assert( pSkr );
  assert( pTgt );
  assert( pTse6 );
  assert( pTseO );

  for (idx = 0; idx < 6; ++idx)
  {
    eye[idx][idx] = 1.0;
  }

  HH[0][0] = 1.0;
  HH[1][2] = 1.0;
  HH[2][3] = 1.0;

  HT = Transpose( HH );

  initFrameTime = pMsl->launchTime;

  nextFrameTime = initFrameTime;

  nextUpLinkTime = pMsl->launchTime;

}  // end initialize

//############################################################################//
//
// Function: Framepdate()
//
// Purpose:  Perform frame update
//
//############################################################################//
void MslTse6::FrameUpdate()
{
  double cth;
  double sth;

  simTime = GetSimTime();

  //
  // Get true states
  //
  tgtPxf = pTgt->pxf;
  tgtPyf = pTgt->pyf;

  tgtVxf = pTgt->vxf;
  tgtVyf = pTgt->vyf;

  tgtAxf = pTgt->axf;
  tgtAyf = pTgt->ayf;

  tgtJxf = pTgt->jxf;
  tgtJyf = pTgt->jyf;

  mslPxf = pMsl->pxf;
  mslPyf = pMsl->pyf;

  mslVxf = pMsl->vxf;
  mslVyf = pMsl->vyf;

  mslAxf = pMsl->axf;
  mslAyf = pMsl->ayf;

  omega = pTgt->omega;

  truRelPxf = tgtPxf - mslPxf;
  truRelPyf = tgtPyf - mslPyf;

  truRelVxf = tgtVxf - mslVxf;
  truRelVyf = tgtVyf - mslVyf;

  truRelAxf = tgtAxf - mslAxf;
  truRelAyf = tgtAyf - mslAyf;

  truRng2 = truRelPxf*truRelPxf + truRelPyf*truRelPyf;

  truRng = sqrt( truRng2 );

  truLosAng = atan2( truRelPyf, truRelPxf );

  truLosr = ( truRelPxf * truRelVyf - truRelPyf * truRelVxf ) / truRng2;

  truRdot = ( truRelVxf * truRelPxf + truRelVyf * truRelPyf ) / truRng;

  cth = cos( truLosAng );
  sth = sin( truLosAng );

  truAccLosX = truRelAxf * cth - truRelAyf * sth;
  truAccLosY = truRelAxf * sth + truRelAyf * cth;

  truState(0) = truLosAng;

  truState(1) = truLosr;

  truState(2) = 1.0 / truRng;

  truState(3) = truRdot * truState(2);

  truState(4) = truAccLosX  * truState(2);

  truState(5) = truAccLosY  * truState(2);

  if ( simTime >= nextUpLinkTime - SMALL )
  {
    //
    // Get the datalink message.
    //
    nextUpLinkTime = simTime + datalinkStep;

    dlTimeStamp = pTse6->timeStamp;
    priorDlTime = dlTimeStamp;

    rdrPxf = pRdr->pxf;
    rdrPyf = pRdr->pyf;
    rdrVxf = pRdr->vxf;
    rdrVyf = pRdr->vyf;
        
    if ( !pTgt->weaveInit )
    {
      //
      // Target is not weaving.  The Fire control TSE6 estimates are good.
      //
      dlAccX = pTse6->estAccX;
      dlAccY = pTse6->estAccY;
      dlLosAng = pTse6->estLosAng;
      dlLosr = pTse6->estLosr;
      dlRng = pTse6->estRng;
      dlRdot = pTse6->estRdot;

      cth = cos( dlLosAng );
      sth = sin( dlLosAng );
      
      dlTgtPxf = rdrPxf + dlRng * cth;
      dlTgtPyf = rdrPyf + dlRng * sth;
      
      dlTgtVxf = rdrVxf + dlRdot * cth - dlRng * dlLosr * sth;
      
      dlTgtVyf = rdrVyf + dlRdot * sth + dlRng * dlLosr * cth;
      
      dlTgtAxf = dlAccX;
      dlTgtAyf = dlAccY;

      dlState(0) = dlTgtPxf;
      dlState(1) = dlTgtPyf;

      dlState(2) = dlTgtVxf;
      dlState(3) = dlTgtVyf;

      dlState(4) = dlTgtAxf;
      dlState(5) = dlTgtAyf;

    }  // end if ( !pTgt->weaveInit )
    else
    {
      dlOmega = pTseO->estOmega;

      dlOmega2 = pTseO->estOmega2;

      dlRng = pTse6->estRng;
      dlRdot = pTse6->estRdot;

      //
      // Small angle approximation
      //
      dlLosAng = pTseO->estPosY / dlRng;

      sth = sin( dlLosAng );
  
      cth = cos( dlLosAng );
      
      dlTgtPxf = rdrPxf + dlRng * cth;
      dlTgtPyf = rdrPyf + dlRng * sth;

      dlPosYdot = pTseO->estPosYdot;

      dlLosr = dlPosYdot / dlRng;
      
      dlTgtVxf = rdrVxf + dlRdot * cth - dlRng * dlLosr * sth;
      
      dlTgtVyf = rdrVyf + dlRdot * sth + dlRng * dlLosr * cth;

      dlTgtAxf = pTse6->cartRel(4);
      dlTgtAyf = -dlTgtPyf * dlOmega2;

      dlTgtJxf = 0.0;

      dlTgtJyf = dlTgtPyf * dlOmega2 * dlOmega;

    }  // end if ( !pTgt->weaveInit )
  }  // end if ( simTime >= nextUpLinkTime - SMALL )

//  if ( pSkr->timeStamp > dlTimeStamp )
  if ( pSkr->timeStamp != dlTimeStamp )
  {
    //
    // Propagate the datalink states up to the seeker time stamp
    //
    dlState(0) = dlTgtPxf;
    dlState(1) = dlTgtPyf;
                             
    dlState(2) = dlTgtVxf;
    dlState(3) = dlTgtVyf;
                             
    dlState(4) = dlTgtAxf;
    dlState(5) = dlTgtAyf;

    IntegrateRK2( dlState, pSkr->timeStamp - priorDlTime );
    priorDlTime = pSkr->timeStamp;

    dlTgtPxf = dlState(0);
    dlTgtPyf = dlState(1);

    dlTgtVxf = dlState(2);
    dlTgtVyf = dlState(3);

    dlTgtAxf = dlState(4);
    dlTgtAyf = dlState(5);

  }  // end if ( simTime >= nextUpLinkTime - SMALL ) else

  measRelPxf = dlTgtPxf - mslPxf;
  measRelPyf = dlTgtPyf - mslPyf;

  measRelVxf = dlTgtVxf - mslVxf;
  measRelVyf = dlTgtVyf - mslVyf;

  measRng2 = measRelPxf*measRelPxf + measRelPyf*measRelPyf;

  measRng = sqrt( measRng2 );

  measRdot = ( measRelVxf * measRelPxf + measRelVyf * measRelPyf ) / measRng;

  //
  // Get the Seeker measurements
  //
  measAng = pSkr->measAng;
  angSig = pSkr->rinAng; 

  //
  // Use the datalink range and range rate as pseudo radar measurements,
  // but use the more accurate seeker angle measurement. Use the Radar
  // range and range rate measurement noise instead of the TSE covariance
  // to compensate for the fact that the TSE output is corelated.
  //
  dlRngSig = pRdr->rinRng;
  dlRdotSig = pRdr->rinRdot;

  rngSig = dlRngSig;

  rDotSig = dlRdotSig;

  measVec(0) = measAng;
  measVec(1) = 1.0 / measRng;
  measVec(2) = measRdot / measRng;

  PropStates( pSkr->timeStamp - timeStamp );

  timeStamp = pSkr->timeStamp;

  UpdateStates();

  estLosAng = mscState(0);
  estLosr = mscState(1);
  estRng = 1.0 / mscState(2);
  estRdot = mscState(3) * estRng;
  estAccX = mscState(4) * estRng;
  estAccY = mscState(5) * estRng;

  estTgtPxf = mslPxf + cartRel(0);
  estTgtPyf = mslPyf + cartRel(1);

  estTgtVxf = mslVxf + cartRel(2);
  estTgtVyf = mslVyf + cartRel(3);

  estTgtAxf = mslAxf + cartRel(4);
  estTgtAyf = mslAyf + cartRel(5);

  estTgtJxf = 0.0;
  estTgtJyf = estTgtPyf * dlOmega2*dlOmega;

}  // end FrameUpdate()

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
void MslTse6::Update()
{
}

//############################################################################//
//
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
void MslTse6::UpdateDerivatives()
{
}

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
void MslTse6::Finalize()
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
void MslTse6::Output( bool printHeader,
                     FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "mslTse6.estAccX" );
    fprintf( pOutFile, "%20s", "mslTse6.estAccY" );
    fprintf( pOutFile, "%20s", "mslTse6.estLosAng" );
    fprintf( pOutFile, "%20s", "mslTse6.estLosr" );
    fprintf( pOutFile, "%20s", "mslTse6.estRng" );
    fprintf( pOutFile, "%20s", "mslTse6.estRdot" );

    fprintf( pOutFile, "%20s", "mslTse6.estTgtPxf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtPyf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtVxf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtVyf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtAxf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtAyf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtJxf" );
    fprintf( pOutFile, "%20s", "mslTse6.estTgtJyf" );

    fprintf( pOutFile, "%20s", "mslTse6.measAng" );
    fprintf( pOutFile, "%20s", "mslTse6.measRng" );
    fprintf( pOutFile, "%20s", "mslTse6.measRdot" );

    fprintf( pOutFile, "%20s", "mslTse6.measVec0" );
    fprintf( pOutFile, "%20s", "mslTse6.measVec1" );
    fprintf( pOutFile, "%20s", "mslTse6.measVec2" );

    fprintf( pOutFile, "%20s", "mslTse6.mscState0" );
    fprintf( pOutFile, "%20s", "mslTse6.mscState1" );
    fprintf( pOutFile, "%20s", "mslTse6.mscState2" );
    fprintf( pOutFile, "%20s", "mslTse6.mscState3" );
    fprintf( pOutFile, "%20s", "mslTse6.mscState4" );
    fprintf( pOutFile, "%20s", "mslTse6.mscState5" );

    fprintf( pOutFile, "%20s", "mslTse6.timeStamp" );

    fprintf( pOutFile, "%20s", "mslTse6.P00" );
    fprintf( pOutFile, "%20s", "mslTse6.P11" );
    fprintf( pOutFile, "%20s", "mslTse6.P22" );
    fprintf( pOutFile, "%20s", "mslTse6.P33" );
    fprintf( pOutFile, "%20s", "mslTse6.P44" );
    fprintf( pOutFile, "%20s", "mslTse6.P55" );

    fprintf( pOutFile, "%20s", "mslTse6.residual0" );
    fprintf( pOutFile, "%20s", "mslTse6.residual1" );
    fprintf( pOutFile, "%20s", "mslTse6.residual2" );

    fprintf( pOutFile, "%20s", "mslTse6.cartState0" );
    fprintf( pOutFile, "%20s", "mslTse6.cartState1" );
    fprintf( pOutFile, "%20s", "mslTse6.cartState2" );
    fprintf( pOutFile, "%20s", "mslTse6.cartState3" );
    fprintf( pOutFile, "%20s", "mslTse6.cartState4" );
    fprintf( pOutFile, "%20s", "mslTse6.cartState5" );

    fprintf( pOutFile, "%20s", "mslTse6.truRelPxf" );
    fprintf( pOutFile, "%20s", "mslTse6.truRelPyf" );
    fprintf( pOutFile, "%20s", "mslTse6.truRelVxf" );
    fprintf( pOutFile, "%20s", "mslTse6.truRelVyf" );
    fprintf( pOutFile, "%20s", "mslTse6.truRelAxf" );
    fprintf( pOutFile, "%20s", "mslTse6.truRelAyf" );

    fprintf( pOutFile, "%20s", "mslTse6.truAccLosX" );
    fprintf( pOutFile, "%20s", "mslTse6.truAccLosY" );
    fprintf( pOutFile, "%20s", "mslTse6.truLosAng" );
    fprintf( pOutFile, "%20s", "mslTse6.truLosr" );
    fprintf( pOutFile, "%20s", "mslTse6.truRng" );
    fprintf( pOutFile, "%20s", "mslTse6.truRdot" );

    fprintf( pOutFile, "%20s", "mslTse6.truState0" );
    fprintf( pOutFile, "%20s", "mslTse6.truState1" );
    fprintf( pOutFile, "%20s", "mslTse6.truState2" );
    fprintf( pOutFile, "%20s", "mslTse6.truState3" );
    fprintf( pOutFile, "%20s", "mslTse6.truState4" );
    fprintf( pOutFile, "%20s", "mslTse6.truState5" );
  }
  else
  {
    fprintf( pOutFile, "%20.6e", estAccX );
    fprintf( pOutFile, "%20.6e", estAccY );
    fprintf( pOutFile, "%20.6e", estLosAng );
    fprintf( pOutFile, "%20.6e", estLosr );
    fprintf( pOutFile, "%20.6e", estRng );
    fprintf( pOutFile, "%20.6e", estRdot );

    fprintf( pOutFile, "%20.6e", estTgtPxf );
    fprintf( pOutFile, "%20.6e", estTgtPyf );
    fprintf( pOutFile, "%20.6e", estTgtVxf );
    fprintf( pOutFile, "%20.6e", estTgtVyf );
    fprintf( pOutFile, "%20.6e", estTgtAxf );
    fprintf( pOutFile, "%20.6e", estTgtAyf );
    fprintf( pOutFile, "%20.6e", estTgtJxf );
    fprintf( pOutFile, "%20.6e", estTgtJyf );

    fprintf( pOutFile, "%20.6e", measAng );
    fprintf( pOutFile, "%20.6e", measRng );
    fprintf( pOutFile, "%20.6e", measRdot );

    fprintf( pOutFile, "%20.6e", measVec(0) );
    fprintf( pOutFile, "%20.6e", measVec(1) );
    fprintf( pOutFile, "%20.6e", measVec(2) );

    fprintf( pOutFile, "%20.6e", mscState(0) );
    fprintf( pOutFile, "%20.6e", mscState(1) );
    fprintf( pOutFile, "%20.6e", mscState(2) );
    fprintf( pOutFile, "%20.6e", mscState(3) );
    fprintf( pOutFile, "%20.6e", mscState(4) );
    fprintf( pOutFile, "%20.6e", mscState(5) );

    fprintf( pOutFile, "%20.6e", timeStamp );

    fprintf( pOutFile, "%20.6e", PP[0][0] );
    fprintf( pOutFile, "%20.6e", PP[1][1] );
    fprintf( pOutFile, "%20.6e", PP[2][2] );
    fprintf( pOutFile, "%20.6e", PP[3][3] );
    fprintf( pOutFile, "%20.6e", PP[4][4] );
    fprintf( pOutFile, "%20.6e", PP[5][5] );

    fprintf( pOutFile, "%20.6e", residual(0) );
    fprintf( pOutFile, "%20.6e", residual(1) );
    fprintf( pOutFile, "%20.6e", residual(2) );

    fprintf( pOutFile, "%20.6e", cartRel(0) );
    fprintf( pOutFile, "%20.6e", cartRel(1) );
    fprintf( pOutFile, "%20.6e", cartRel(2) );
    fprintf( pOutFile, "%20.6e", cartRel(3) );
    fprintf( pOutFile, "%20.6e", cartRel(4) );
    fprintf( pOutFile, "%20.6e", cartRel(5) );

    fprintf( pOutFile, "%20.6e", truRelPxf );
    fprintf( pOutFile, "%20.6e", truRelPyf );
    fprintf( pOutFile, "%20.6e", truRelVxf );
    fprintf( pOutFile, "%20.6e", truRelVyf );
    fprintf( pOutFile, "%20.6e", truRelAxf );
    fprintf( pOutFile, "%20.6e", truRelAyf );

    fprintf( pOutFile, "%20.6e", truAccLosX );
    fprintf( pOutFile, "%20.6e", truAccLosY );
    fprintf( pOutFile, "%20.6e", truLosAng );
    fprintf( pOutFile, "%20.6e", truLosr );
    fprintf( pOutFile, "%20.6e", truRng );
    fprintf( pOutFile, "%20.6e", truRdot );

    fprintf( pOutFile, "%20.6e", truState(0) );
    fprintf( pOutFile, "%20.6e", truState(1) );
    fprintf( pOutFile, "%20.6e", truState(2) );
    fprintf( pOutFile, "%20.6e", truState(3) );
    fprintf( pOutFile, "%20.6e", truState(4) );
    fprintf( pOutFile, "%20.6e", truState(5) );

  }
}  // end Output()

//
// Class scope functions
//

//############################################################################//
//
// Function: PropStates()
//
// Purpose:  Propagate the states to the measurement time
//
//############################################################################//
  void MslTse6::PropStates( double deltaTime )
{
  double alphaR;
  double cCoeff;
  double invRng2;
  double Q44;
  double Q55;

  if ( !initialized )
  {
    initialized = true;

    mscState(0) = measVec(0);    // LOS angle
    mscState(1) = 0.0;           // LOS rate
    mscState(2) = measVec(1);    // Inverse range
    mscState(3) = measVec(2);    // rDot / range
    mscState(4) = 0.0;           // Acceleration along LOS / range
    mscState(5) = 0.0;           // Acceleration across LOS / range

    invRng2 = SQ( mscState(2) );

    PP[0][0] = SQ( angSig );
    
    PP[1][1] = SQ( SQ(rDotSig) * measVec(2) );

    PP[2][2] = SQ( rngSig * invRng2 );

    PP[3][3] = SQ( SQ(rDotSig) * measVec(1) ) +
               SQ( measRdot * rngSig * invRng2 );

    PP[2][3] = SQ( rngSig * measVec(1) ) * measVec(1) * measVec(2);
    PP[3][2] = PP[2][3];

    PP[4][4] = singerVar * invRng2;
    PP[5][5] = PP[4][4];

  }  // end if ( !initialized )

  //
  // Propagate the state vector in cartesian coordinates
  //
  MscToCart( mscState,
             cartRel );


  IntegrateRK2( cartRel, deltaTime );

  CartToMsc( cartRel,
             mscState );

  //
  // Propagate the estimation covariance matrix
  //
  //
  // State transition Jacobian
  //
  phi = 0.0;

  phi[0][0] = 1.0;
  phi[0][1] = deltaTime;

  phi[1][1] = 1.0 - 2.0 * mscState(3) * deltaTime;
  phi[1][3] = -2.0 * mscState(1) * deltaTime;
  phi[1][5] = deltaTime;

  phi[2][2] = 1.0 - mscState(3) * deltaTime;
  phi[2][3] = -mscState(2) * deltaTime;

  phi[3][1] = 2.0 * mscState(1) * deltaTime;
  phi[3][3] = 1.0 - 2.0 * mscState(3) * deltaTime;
  phi[3][4] = deltaTime;

  phi[4][3] = -mscState(4) * deltaTime;
  phi[4][4] = 1.0 - (1.0 / singerTau + mscState(3)) * deltaTime;
 
  phi[5][3] = -mscState(5) * deltaTime;
  phi[5][5] = phi[4][4];

  //
  // Process noise matrix
  //
  cCoeff = 2.0 * SQ( mscState(2) ) * deltaTime / ( 3.0 * singerTau );

  alphaR = cCoeff * singerVar;
    
  Q44 = alphaR * ( SQ(phi[4][4]) + phi[4][4] + 1.0 );

  Q55 = Q44;

  //
  // Propagate the estimation covariance matrix.
  //
  PP = phi * PP * Transpose( phi );
  PP[4][4] += Q44;
  PP[5][5] += Q55;
 
}  // end PropStates()

//############################################################################//
//
// Function: UpdateStates()
//
// Purpose:  Update the states with a new measurement
//
//############################################################################//
void MslTse6::UpdateStates()
{
  Matrix   invTemp33(3,3);
  double   sigmaInvRng;
  double   sigmaInvTgo2;
  Matrix   temp0(3,3);
  Matrix   temp1(6,6);
  bool     test;

  //
  // Measurement covariance matrix
  //
  sigmaInvRng = rngSig * SQ(measVec(1));

  sigmaInvTgo2 = ( SQ(rDotSig) + SQ(rngSig * measVec(2)) ) * SQ( measVec(1) );

  RR[0][0] = SQ( angSig );
  RR[1][1] = SQ( sigmaInvRng );
  RR[2][2] = sigmaInvTgo2;
  RR[1][2] = SQ( rngSig ) * measVec(2) * SQ( measVec(1) ) * measVec(1);
  RR[2][1] = RR[1][2];

  //
  // Calculate the Kalman gains.
  //
  //  K = P * H' * [ H * P * H' + R]^-1
  //
  temp0 = HH * PP * HT + RR;  

//  test = temp0.Invert();

  test = inv33( temp0, invTemp33 );
  if ( !test )
  {
    cout << "MslTse::UpdateStates() temp33 matrix inversion fail @ time = " 
             << simTime << endl;
    exit( 1 );
  }
  temp0 = invTemp33;


  KK = PP * HT  * temp0;

  //
  // Update the state with the measurement
  //
  // residual = Z - H * x
  //
  residual = measVec - HH * mscState;

  innovation = KK * residual;

  mscState += innovation;

  //
  // Update the estimation covariance
  //
  temp1 = eye - KK * HH;

  PP = temp1 * PP * Transpose( temp1 ) + KK * RR * Transpose( KK );

}  // end UpdateStates()

//############################################################################//
//
// Function: IntegrateRK2()
//
// Purpose:  2nd order Runge Kutta integrator
//
//############################################################################//
void MslTse6::IntegrateRK2( Matrix &XX, double deltaTime  )
{
  double  half = 0.5;
  Matrix  stateDerivK(6,1);
  Matrix  stateDerivKp1(6,1);
  Matrix  stateIncrement(6,1);
  Matrix  stateTemp(6,1);

  //
  // Get derivative at current time
  //
  stateDerivK = CalcStateDeriv( XX );

  //
  // Get derivative at next time step
  //
  stateIncrement = stateDerivK * deltaTime;

  stateTemp = XX + stateIncrement;

  stateDerivKp1 = CalcStateDeriv( stateTemp );

  //
  // The final state increment is calculated from the average of the current
  // and next state derivatives.
  //
  stateIncrement = (stateDerivK + stateDerivKp1) * deltaTime * half;

  XX += stateIncrement;

}  // end IntegrateRK2()

//############################################################################//
//
// Function: CalcStateDeriv()
//
// Purpose:  Calculate the state derivative vector
//
//   posY = X0 = A * sin( omega * t )
//
//   velY = X1 = A * omega * cos( omega * t )
//
//   accY = -X2*X0 = -A * omega^2 * sin( omega * t )
//  
//############################################################################//
Matrix  MslTse6::CalcStateDeriv( Matrix  &XX )
{
  Matrix  deriv(6,1);
                             
  deriv(0) = XX(2);  // d/dt(posX) = velX
  deriv(1) = XX(3);  // d/dt(posY) = velY
  deriv(2) = XX(4);  // d/dt(velX) = accX
  deriv(3) = XX(5);  // d/dt(velY) = accY
  deriv(4) = 0.0;    // d/dt(accX) = 0.0
  deriv(5) = 0.0;    // d/dy(accY) = 0.0

  return( deriv );

} // end CalcStateDeriv()

//############################################################################//
//
// Function: MscToCart()
//
// Purpose:  Convert from modified spherical coordinates to cartesian
//
//############################################################################//
void MslTse6::MscToCart( Matrix &mscState,
                         Matrix &cartRel )
{
  double range = 1.0 / mscState(2);
  double rDot = mscState(3) * range;

  double cth = cos( mscState(0) );
  double sth = sin( mscState(0) );

  double thDot = mscState(1);

  cartRel(0) = range * cth;
  cartRel(1) = range * sth;
  
  cartRel(2) = rDot * cth - range * thDot * sth;
  cartRel(3) = rDot * sth + range * thDot * cth;

  cartRel(4) = range * ( mscState(4) * cth - mscState(5) * sth );
  cartRel(5) = range * ( mscState(4) * sth + mscState(5) * cth );

}  // end MSCtoCart()

//############################################################################//
//
// Function: CartToMsc()
//
// Purpose:  Convert from modified spherical coordinates to cartesian
//
//############################################################################//
void MslTse6::CartToMsc( Matrix &cartRel,
                         Matrix &mscState )
{
  double range2 = SQ( cartRel(0) ) + SQ( cartRel(1) );

  double range = sqrt( range2 );

  double rDot = ( cartRel(0) * cartRel(2) + cartRel(1) * cartRel(3) ) / range;

  double losAng = atan2(cartRel(1), cartRel(0) );

  double losr = ( cartRel(0) * cartRel(3) - cartRel(1) * cartRel(2) ) / range2;

  double cth = cos( losAng );
  double sth = sin( losAng );

  double Ar =  cartRel(4) * cth + cartRel(5) * sth;
  double Ac = -cartRel(4) * sth + cartRel(5) * cth;

  mscState(0) = losAng;
  mscState(1) = losr;
  mscState(2) = 1.0 / range;
  mscState(3) = mscState(2) * rDot;
  mscState(4) = Ar / range;
  mscState(5) = Ac / range;

}  // end CartToMSC()

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void MslTse6::ResetAll()
{
  angSig      = 0.0;
  cartRel     = 0.0;
  dlAccX      = 0.0;
  dlAccY      = 0.0;
  dlLosAng    = 0.0;
  dlLosr      = 0.0;
  dlRdot      = 0.0;
  dlRdotSig   = 0.0;
  dlRng       = 0.0;
  dlRng2      = 0.0;
  dlRngSig    = 0.0;
  dlState     = 0.0;
  dlTgtAxf    = 0.0;
  dlTgtAyf    = 0.0;
  dlTgtPxf    = 0.0;
  dlTgtPyf    = 0.0;
  dlTgtVxf    = 0.0;
  dlTgtVyf    = 0.0;
  dlTimeStamp = 0.0;
  estAccX     = 0.0;
  estAccY     = 0.0;
  estLosAng   = 0.0;
  estLosr     = 0.0;
  estRng      = 0.0;
  estRdot     = 0.0;
  eye         = 0.0;
  HH          = 0.0;
  HT          = 0.0;
  initialized = false;
  innovation  = 0.0;
  KK          = 0.0;
  measAng     = 0.0;
  measRng     = 0.0;
  measRdot    = 0.0;
  measRelPxf  = 0.0;
  measRelPyf  = 0.0;
  measRelVxf  = 0.0;
  measRelVyf  = 0.0;
  measVec     = 0.0;
  mscState    = 0.0;
  mslAxf      = 0.0;
  mslAyf      = 0.0;
  mslPxf      = 0.0;
  mslPyf      = 0.0;
  mslVxf      = 0.0;
  mslVyf      = 0.0;
  omega       = 0.0;
  phi         = 0.0;
  PP          = 0.0;
  priorDlTime = 0.0;
  rDotSig     = 0.0;
  rdrPxf      = 0.0;
  rdrPyf      = 0.0;
  rdrVxf      = 0.0;
  rdrVyf      = 0.0;
  residual    = 0.0;
  rngSig      = 0.0;
  RR          = 0.0;
  simTime     = 0.0;
  tgtAxf      = 0.0;
  tgtAyf      = 0.0;
  tgtJxf      = 0.0;
  tgtJyf      = 0.0;
  tgtPxf      = 0.0;
  tgtPyf      = 0.0;
  tgtVxf      = 0.0;
  tgtVyf      = 0.0;
  timeStamp   = 0.0;
  truAccX     = 0.0;
  truAccY     = 0.0;
  truAccLosX  = 0.0;
  truAccLosY  = 0.0;
  truLosAng   = 0.0;
  truLosr     = 0.0;
  truRelPxf   = 0.0;
  truRelPyf   = 0.0;
  truRelVxf   = 0.0;
  truRelVyf   = 0.0;
  truRelAxf   = 0.0;
  truRelAyf   = 0.0;
  truRng2     = 0.0;
  truRng      = 0.0;
  truRdot     = 0.0;
  truState    = 0.0;

}  // end ResetAll()


//############################################################################//
//
// Function: inv33()
//
// Purpose:  Safely invert a 3x3 matrix using Gaussian elimination.
//
//############################################################################//
bool MslTse6::inv33(Matrix &matIn, Matrix &matOut )
{
  bool test(true);
  
  matOut = matIn.Inv();

  return( test ); 
}

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

