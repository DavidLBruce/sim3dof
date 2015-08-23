//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       FcTse6.hpp

    \author     David Bruce

    \brief      Fire control radar

*/
//############################################################################//

#ifndef FCTSE6_HPP
#define FCTSE6_HPP

#include "MdlBase.hpp"

//
// Sim includes
//
//#include "genMath.h"
#include "Matrix.hpp"

//
// Forward declarations
//
class FcRadar;
class SimExec;
class Target;



//
// Fire control class
//
class FcTse6 : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double  initFrameTime; // sec     Time of first FrameUpdate

  double  singerTau;   // sec       Kalman process noise tuning parameter
  double  singerVar;   // (m/s^2)^2 Kalman process noise tuning parameter

  //
  // Other public scope variables
  //
  Matrix  cartRel;     // na     Cartesian state vector

  double  estAccX;     // m/sec^2   Estimated acceleration along LOS
  double  estAccY;     // m/sec^2   Estimated acceleration across LOS 
  double  estLosAng;   // rad       Estimated LOS angle               
  double  estLosr;     // rad/sec   Estimated LOS rate                
  double  estRng;      // m         Estimated range                   
  double  estRdot;     // m/sec     Estimated range rate               

  double  timeStamp;   // sec       Time stamp of measurement

  double  truAccX;     // m/sec^2   True acceleration inertial X
  double  truAccY;     // m/sec^2   True acceleration inertial Y
  double  truAccLosX;  // m/sec^2   True acceleration along LOS 
  double  truAccLosY;  // m/sec^2   True acceleration across LOS 
  double  truLosAng;   // rad       True LOS angle               
  double  truLosr;     // rad/sec   True LOS rate                
  double  truRng;      // m         True range                   
  double  truRdot;;    // m/sec     True range rate              

  Matrix  truState;    // na        True MSC state

//############################################################################//
//
// Function:  FcTse6()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  FcTse6();

//############################################################################//
//
// Function: </b>  ~FcTse6()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~FcTse6();

//############################################################################//
//
// Function: Initialize()
//                                
// Purpose:  Initialize the model 
//                                
//############################################################################//
  virtual void Initialize();

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform frame update
//
//############################################################################//
  virtual void FrameUpdate();

//############################################################################//
//
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
  virtual void UpdateDerivatives();

//############################################################################//
//
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
  virtual void Update();

//############################################################################//
//
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//############################################################################//
  virtual void Finalize();

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print selected variables
//
//############################################################################//
  void Output( bool printHeader,
               FILE *outfile );

//############################################################################//
//
// Function: SetRadar()
//
// Purpose:  Set the radar reference pointer.
//
//############################################################################//
  void SetRadar( FcRadar *pRdr_ )
  { pRdr = pRdr_; }

//############################################################################//
//
// Function: SetSimExec()
//
// Purpose:  Set the SimExec reference pointer.
//
//############################################################################//
  void SetSimExec( SimExec *pExec_ )
  { pExec = pExec_; }

//############################################################################//
//
// Function: SetTarget()
//
// Purpose:  Set the SimExec reference pointer.
//
//############################################################################//
  void SetTarget( Target *pTgt_ )
  { pTgt = pTgt_; }

//############################################################################//
//
// Function: SetTarget()
//
// Purpose:  Set the target reference pointer.
//
//############################################################################//
  void SetRadar( Target *pTgt_ )
  { pTgt = pTgt_; }


private:

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
  void PropStates( double timeStep );

//############################################################################//
//
// Function: UpdateStates()
//
// Purpose:  Update the states with a new measurement
//
//############################################################################//
  void UpdateStates();

//############################################################################//
//
// Function: IntegrateRK2()
//
// Purpose:  2nd order Runge Kutta integrator
//
//############################################################################//
  void IntegrateRK2( Matrix &XX, double timeStep );

//############################################################################//
//
// Function: StateDeriv()
//
// Purpose:  Calculate the state derivative
//
//############################################################################//
  Matrix CalcStateDeriv( Matrix &XX );

//############################################################################//
//
// Function: MscToCart()
//
// Purpose:  Convert from modified spherical coordinates to cartesian
//
//############################################################################//
  void MscToCart( Matrix &mscState,
                  Matrix &cartRel );

//############################################################################//
//
// Function: CartToMsc()
//
// Purpose:  Convert from modified spherical coordinates to cartesian
//
//############################################################################//
  void CartToMsc( Matrix &cartRel,
                  Matrix &mscState );

//############################################################################//
//
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
  void ResetAll();

  //
  // Class scope variables
  //
  double   angSig;     // rad    Measured angle standard deviation
  Matrix   eye;        // na     Identity matrix
  Matrix   HH;         // na     Measurement matrix
  Matrix   HT;         // na     Transpose of the measurement matrix
  bool     initialized;// na     Filter has been initialized.
  Matrix   innovation; // na     Kalman innovation
  Matrix   KK;         // na     Kalman gains
  double   measAng;    // rad    Measure LOS angle
  double   measRng;    // m      Measured range
  double   measRdot;   // m/sec  Measured range rate
  Matrix   measVec;    // na     Measurement vector
  Matrix   mscState;   // na     Modified spherical coordates state vector
  Matrix   phi;        // na     State transition matrix
  Matrix   PP;         // na     Estimation covariance matrix
  double   rDotSig;    // m/sec  Measured range rate standard deviation
  Matrix   residual;   // ns     Update residual
  double   rngSig;     // m      Measured range standard deviation
  Matrix   RR;         // na     Measurement covariance matrix
  double   simTime;    // sec    Simulation time

  //
  // Reference pointers
  //
  SimExec  *pExec;     // Simulation executive
  FcRadar  *pRdr;      // Radar
  Target   *pTgt;      // Target truth
};

//############################################################################//
//
// Function: inv33()
//
// Purpose:  Safely invert a 3x3 matrix using Gaussian elimination.
//
//############################################################################//
bool inv33(Matrix &matIn, Matrix &matOut );

#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

