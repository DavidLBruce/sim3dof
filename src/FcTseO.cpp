//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  \file   FcTseO.cpp

  \author David Bruce

  \brief  Fire control radar

*/
//############################################################################//

#include "FcTseO.hpp"


//
// C/C++ includes
//
#include <cmath>

//
// Reference includes
//
#include "SimExec.hpp"
#include "FcRadar.hpp"

//
// Other sim includes
//
//#include "genMath.h"
//using namespace genMath;
#include "Matrix.hpp"
using namespace MATRIX;

#include "MathUtils.hpp"

//############################################################################//
//
// Function:  FcTseO()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
FcTseO::FcTseO()
{
  //
  // Input parameter defaults
  //
  frameTimeStep = 1.0/64.0;
  initFrameTime = 0.0;

  initOmega = 4.0;
  initPosY = 0.0;
  initPosYsigScale = 1.4;
  initVelY = 0.0;
  initVelYsig = 5.0;
  initWsig = 2.0;
//  procNoise = 0.1e-3;
  procNoise = 0.0128;

  //
  // Set matrix sizes.
  //
  eye.Eye(3);
  HH.Resize(1,3);
  HT.Resize(3,1);
  innovation.Resize(3,1);
  KK.Resize(3,1);
  phi.Resize(3,3);
  PP.Resize(3,3);
  QQ.Resize(3,3);
  stateVec.Resize(3,1);

}

//############################################################################//
//
// Function:  ~FcTseO()
//
// Purpose:   Destructor
//
//
//############################################################################//
FcTseO::~FcTseO()
{
}

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
void FcTseO::Initialize()
{
  int idx;

  ResetAll();

  simTime = GetSimTime();

  nextFrameTime = initFrameTime;

  HH = 0.0;
  HH[0][0] = 1.0;

  for ( idx = 0; idx < 3; ++idx )
  {
    eye[idx][idx] = 1.0;
  }

}  // end initialize

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform frame update
//
//############################################################################//
void FcTseO::FrameUpdate()
{
  double angSig;
  double measAng;
  double measRng;
  double rngSig;

  simTime = GetSimTime();

  measAng = pRdr->measAng;
  angSig = pRdr->rinAng; 

  measRng = pRdr->measRng;
  rngSig = pRdr->rinRng;

  measPosY = measRng * measAng;
  rinPosY = measRng * angSig + measAng * rngSig; 

  if ( !initialized )
  {
    initialized = true;

    stateVec(0) = initPosY;
    stateVec(1) = initVelY;
    stateVec(2) = initOmega;

    PP[0][0] = SQ( initPosYsigScale * rinPosY );
    PP[1][1] = SQ( initVelYsig );
    PP[2][2] = SQ( initWsig );
  }

  PropStates( frameTimeStep );

  UpdateStates( measPosY );

  estPosY = stateVec(0);
  estPosYdot = stateVec(1);
  estOmega2 = stateVec(2);
  estOmega = sqrt( estOmega2 );

}  // end FrameUpdate()

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
void FcTseO::Update()
{
}

//############################################################################//
//
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
void FcTseO::UpdateDerivatives()
{
}

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
void FcTseO::Finalize()
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
void FcTseO::Output( bool printHeader,
                    FILE *pOutFile )
{
  simTime = GetSimTime();

  if ( printHeader )
  {
    fprintf( pOutFile, "%20s", "fcTseO.measPosY");
    fprintf( pOutFile, "%20s", "fcTseO.residual");
    fprintf( pOutFile, "%20s", "fcTseO.RR00");
    fprintf( pOutFile, "%20s", "fcTseO.estPosY");
    fprintf( pOutFile, "%20s", "fcTseO.estPosYdot");
    fprintf( pOutFile, "%20s", "fcTseO.estOmega2");
    fprintf( pOutFile, "%20s", "fcTseO.estOmega");
    fprintf( pOutFile, "%20s", "fcTseO.P00");
    fprintf( pOutFile, "%20s", "fcTseO.P11");
    fprintf( pOutFile, "%20s", "fcTseO.P22");
    fprintf( pOutFile, "%20s", "fcTseO.K0");
    fprintf( pOutFile, "%20s", "fcTseO.K1");
    fprintf( pOutFile, "%20s", "fcTseO.K2");
  }
  else
  {
    fprintf( pOutFile, "%20.6e", measPosY );
    fprintf( pOutFile, "%20.6e", residual );
    fprintf( pOutFile, "%20.6e", RR );
    fprintf( pOutFile, "%20.6e", estPosY );
    fprintf( pOutFile, "%20.6e", estPosYdot );
    fprintf( pOutFile, "%20.6e", estOmega2 );
    fprintf( pOutFile, "%20.6e", estOmega );
    fprintf( pOutFile, "%20.6e", PP[0][0] );
    fprintf( pOutFile, "%20.6e", PP[1][1] );
    fprintf( pOutFile, "%20.6e", PP[2][2] );
    fprintf( pOutFile, "%20.6e", KK[0][0] );
    fprintf( pOutFile, "%20.6e", KK[1][0] );
    fprintf( pOutFile, "%20.6e", KK[2][0] );
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
  void FcTseO::PropStates( double timeStep )
{
  double timeStep2 = timeStep * timeStep;
  double timeStep3 = timeStep2 * timeStep;

  double x2 = stateVec(0) * stateVec(0);

  phi[0][0] = 1.0;
  phi[0][1] = timeStep;
  phi[0][2] = 0.0;

  phi[1][0] = -stateVec(2) * timeStep;
  phi[1][1] = 1.0;
  phi[1][2] = -stateVec(0) * timeStep;

  phi[2][0] = 0.0;
  phi[2][1] = 0.0;
  phi[2][2] = 1.0;

  QQ[0][0] = 0.0;
  QQ[0][1] = 0.0;
  QQ[0][2] = 0.0;

  QQ[1][0] = 0.0;
  QQ[1][1] = x2 * timeStep3 * procNoise / 3.0;
  QQ[1][2] = -0.5 * stateVec(0) * timeStep2 * procNoise;

  QQ[2][0] = 0.0;
  QQ[2][1] = -0.5 * stateVec(0) * timeStep2 * procNoise;
  QQ[2][2] = timeStep * procNoise;


  //
  // Propagate the state vector
  //
  stateVec = IntegrateRK2( stateVec, timeStep );

  //
  // Propagate the estimation covariance matrix
  //
  PP = phi * PP * Transpose(phi) + QQ;

}


//############################################################################//
//
// Function: UpdateStates()
//
// Purpose:  Update the states with a new measurement
//
//############################################################################//
void FcTseO::UpdateStates( double measPosY )
{
  double   temp0;
  Matrix   temp1(3,3);
  Matrix   temp2(3,3);

//  RR = SQ( rinPosY );
// TBD RR is wrong, but it works better.  Maybe process noise or initial PP
// is too low?
  RR = rinPosY;

  //
  // Calculate the Kalman gains.
  //

//  temp0 = HH * PP * HT + RR;  
  temp0 = PP[0][0] + RR;  

  //
  //  KK = PP * HT  / temp0;
  //
  KK[0][0] = PP[0][0] / temp0;
  KK[1][0] = PP[1][0] / temp0;
  KK[2][0] = PP[2][0] / temp0;
//  KK = PP * HT  / temp0;

  //
  // Update the state with the measurement
  //
  residual = measPosY - stateVec(0);

  innovation = KK * residual;

  stateVec += innovation;

  //
  // Update the estimation covariance
  //
  //  temp1 = eye - KK * HH
  // 
  temp1 = 0.0;
  temp1[0][0] = KK[0][0];
  temp1[1][0] = KK[1][0];
  temp1[2][0] = KK[2][0];

  temp1 = eye - temp1;

  //
  // temp2 = KK * RR * KT
  //
  temp2 = 0.0;
  temp2[0][0] = KK[0][0] * KK[0][0];
  temp2[0][1] = KK[0][0] * KK[1][0];
  temp2[0][2] = KK[0][0] * KK[2][0];

  temp2[1][0] = KK[1][0] * KK[0][0];
  temp2[1][1] = KK[1][0] * KK[1][0];
  temp2[1][2] = KK[1][0] * KK[2][0];

  temp2[2][0] = KK[2][0] * KK[0][0];
  temp2[2][1] = KK[2][0] * KK[1][0];
  temp2[2][2] = KK[2][0] * KK[2][0];

  temp2 = temp2 * RR;

  PP = temp1 * PP * Transpose( temp1 ) + temp2;

}  // end UpdateStates()

//############################################################################//
//
// Function: IntegrateRK2()
//
// Purpose:  2nd order Runge Kutta integrator
//
//############################################################################//
Matrix FcTseO::IntegrateRK2( Matrix &XX, double deltaTime  )
{
  double   half = 0.5;
  Matrix   stateDerivK(3,1);
  Matrix   stateDerivKp1(3,1);
  Matrix   stateIncrement(3,1);
  Matrix   stateTemp(3,1);

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

  return( XX + stateIncrement );

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
Matrix FcTseO::CalcStateDeriv( Matrix &XX )
{
  Matrix deriv(3,1);
                             // posY = A*sin(w*t)

  deriv(0) = XX(1);          // d/dt( posY ) = velY = w*A*cos(w*t) 

  deriv(1) = -XX(2) * XX(0); // d/dt( velY ) = -w^2*posY = -w^2*A*sin(w*t)

  deriv(2) = 0.0;

  return( deriv );
}

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
void FcTseO::ResetAll()
{
  estPosY     = 0.0;
  estPosYdot  = 0.0;
  estOmega    = 0.0;
  estOmega2   = 0.0;
  eye         = 0.0;
  HH          = 0.0;
  HT          = 0.0;
  initialized = false;
  innovation  = 0.0;
  KK          = 0.0;
  measPosY    = 0.0;
  phi         = 0.0;
  PP          = 0.0;
  residual    = 0.0;
  rinPosY     = 0.0;
  RR          = 0.0;
  QQ          = 0.0;
  simTime     = 0.0;
  stateVec    = 0.0;

}  // end ResetAll()


//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

