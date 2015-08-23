//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       MslTse6.hpp

    \author     David Bruce

    \brief      Projectile MSC Kalman Filter / Target State Estimator

*/
//############################################################################//

#ifndef MSLTSE6_HPP
#define MSLTSE6_HPP

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
class FcTse6;
class FcTseO;
class Missile;
class Seeker;
class SimExec;
class Target;



//
// Fire control class
//
class MslTse6 : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double  initFrameTime; // sec     Time of first FrameUpdate

  double  datalinkStep;// sec       Uplink interval 
  double  nextUpLinkTime; // sec    Next uplink time
  double  singerTau;   // sec       Kalman process noise tuning parameter
  double  singerVar;   // (m/s^2)^2 Kalman process noise tuning parameter

  //
  // Other public scope variables
  //
  Matrix  cartRel;     // na        Cartesian state vector
  double  estAccX;     // m/sec^2   Estimated acceleration along LOS
  double  estAccY;     // m/sec^2   Estimated acceleration across LOS 
  double  estLosAng;   // rad       Estimated LOS angle               
  double  estLosr;     // rad/sec   Estimated LOS rate                
  double  estRng;      // m         Estimated range                   
  double  estRdot;     // m/sec     Estimated range rate               

  double  estTgtPxf;   // m         Estimated inertial position
  double  estTgtPyf;   // m         Estimated inertial position
  double  estTgtVxf;   // m/sec     Estimated inertial velocity
  double  estTgtVyf;   // m/sec     Estimated inertial velocity
  double  estTgtAxf;   // m/sec^2   Estimated inertial acceleration
  double  estTgtAyf;   // m/sec^2   Estimated inertial acceleration
  double  estTgtJxf;   // m/sec^3   Estimated inertial jerk
  double  estTgtJyf;   // m/sec^3   Estimated inertial jerk

  double  timeStamp;   // sec       Estimate time stamp

  double  truAccX;     // m/sec^2   True acceleration inertial X
  double  truAccY;     // m/sec^2   True acceleration inertial Y
  double  truAccLosX;  // m/sec^2   True acceleration along LOS 
  double  truAccLosY;  // m/sec^2   True acceleration across LOS 
  double  truLosAng;   // rad       True LOS angle               
  double  truLosr;     // rad/sec   True LOS rate                
  double  truRng;      // m         True range                   
  double  truRdot;;    // m/sec     True range rate              

  Matrix  truState;    // na        True MSC state

  double  dlOmega;
  double  dlOmega2;
  double  dlPosYdot;
  double  dlTgtJxf;
  double  dlTgtJyf; 
        
  double   tgtAxf;     // m/sec^2 True target inertial acceleraton
  double   tgtAyf;     // m/sec^2 True target inertial acceleraton
  double   tgtJxf;     // m/sec^3 True target inertial jerk
  double   tgtJyf;     // m/sec^3 True target inertial jerk
  double   tgtPxf;     // m       True target inertial position
  double   tgtPyf;     // m       True target inertial position
  double   tgtVxf;     // m/sec   True target inertial velocity
  double   tgtVyf;     // m/sec   True target inertial velocity


//############################################################################//
//
// Function:  MslTse6()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  MslTse6();

//############################################################################//
//
// Function: </b>  ~MslTse6()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~MslTse6();

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
// Function: SetFcTse6()
//
// Purpose:  Set the Fire Control MSC TSE
//
//############################################################################//
  void SetFcTse6( FcTse6 *pTse6_ )
  { pTse6 = pTse6_; }

//############################################################################//
//
// Function: SetFcTse6()
//
// Purpose:  Set the Fire Control MSC TSE
//
//############################################################################//
  void SetFcTseO( FcTseO *pTseO_ )
  { pTseO = pTseO_; }

//############################################################################//
//
// Function: SetMissile()
//
// Purpose:  Set the projectile reference pointer.
//
//############################################################################//
  void SetMissile( Missile *pMsl_ )
  { pMsl = pMsl_; }

//############################################################################//
//
// Function: SetFcRadar()
//
// Purpose:  Set the Fire Control Radar reference pointer.
//
//############################################################################//
  void SetFcRadar( FcRadar *pRdr_ )
  { pRdr = pRdr_; }

//############################################################################//
//
// Function: SetSeeker()
//
// Purpose:  Set the Seeker reference pointer.
//
//############################################################################//
  void SetSeeker( Seeker *pSkr_ )
  { pSkr = pSkr_; }

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
  double   angSig;     // rad     Measured angle standard deviation
  double   dlAccX;     // m/sec^2 Datalink acceleration estimate
  double   dlAccY;     // m/sec^2 Datalink acceleration estimate
  double   dlLosAng;   // rad     Datalink LOS angle wrt the projectile
  double   dlLosr;     // rad/sec Datalink line of sight rate wrt the projectile
  double   dlRdot;     // m/sec   Datalink target range rate wrt the projectile
  double   dlRdotSig;  // m/sec   Datalink range rate noies sigma
  double   dlRng;      // m       Datalink range wrt the projectile
  double   dlRng2;     // m^2     Datalink range squared wrt the projectile
  double   dlRngSig;   // m       Datalink range noise sigma
  Matrix   dlState;    // na      Datalink intertial cartesian target state
  double   dlTgtAxf;   // m/sec^2 Datalink inertial target acceleration
  double   dlTgtAyf;   // m/sec^2 Datalink inertial target acceleration
  double   dlTgtPxf;   // m       Datalink inertial target positon
  double   dlTgtPyf;   // m       Datalink inertial target positon
  double   dlTgtVxf;   // m/sec   Datalink inertial target velocity
  double   dlTgtVyf;   // m/sec   Datalink inertial target velocity
  double   dlTimeStamp;// sec     Datalink time stamp
  Matrix   eye;        // na      Identity matrix
  Matrix   HH;         // na      Measurement matrix
  Matrix   HT;         // na      Transpose of the measurement matrix
  bool     initialized;// na      Filter has been initialized.
  Matrix   innovation; // na      Kalman innovation
  Matrix   KK;         // na      Kalman gains
  double   measAng;    // rad     Measure LOS angle
  double   measRdot;   // m/sec   Measured range rate
  double   measRelPxf; // m       Datalink target position wrt the projectile
  double   measRelPyf; // m       Datalink target position wrt the projectile
  double   measRelVxf; // m/sec   Datalink target velocity wrt the projectile
  double   measRelVyf; // m/sec   Datalink target velocity wrt the projectile
  double   measRng;    // m       Measured (datalink) range
  double   measRng2;   // m^2     measRng squared
  Matrix   measVec;    // na      Measurement vector
  Matrix   mscState;   // na      Modified spherical coordates state vector
  double   mslAxf;     // m/sec^2 True projectile inertial acceleraton
  double   mslAyf;     // m/sec^2 True projectile inertial acceleraton
  double   mslPxf;     // m       True projectile inertial position
  double   mslPyf;     // m       True projectile inertial position
  double   mslVxf;     // m/sec   True projectile inertial velocity
  double   mslVyf;     // m/sec   True projectile inertial velocity
  double   omega;      // rad/sec True target weave frequeny
  Matrix   phi;        // na      State transition matrix
  Matrix   PP;         // na      Estimation covariance matrix
  double   priorDlTime;// sec     Prior datalink integration time
  double   rDotSig;    // m/sec   Measured range rate standard deviation
  double   rdrPxf;     // m       True radar platform position
  double   rdrPyf;     // m       True radar platform position
  double   rdrVxf;     // m/sec   True radar platform velocity
  double   rdrVyf;     // m/sec   True radar platform velocity
  Matrix   residual;   // ns      Update residual
  double   rngSig;     // m       Measured range standard deviation
  Matrix   RR;         // na      Measurement covariance matrix
  double   simTime;    // sec     Simulation time
  double   truRelPxf;  // m       True target position wrt the projectile
  double   truRelPyf;  // m       True target position wrt the projectile
  double   truRelVxf;  // m/sec   True target velocity wrt the projectile
  double   truRelVyf;  // m/sec   True target velocity wrt the projectile
  double   truRelAxf;  // m/sec^2 True target acceleration wrt the projectile
  double   truRelAyf;  // m/sec^2 True target acceleration wrt the projectile
  double   truRng2;    // m^2     True range squared


  //
  // Reference pointers
  //
  SimExec  *pExec;     // Simulation executive
  Missile  *pMsl;      // Missile dynamics
  FcRadar  *pRdr;      // Fire control radar
  Seeker   *pSkr;      // Seeker
  Target   *pTgt;      // Target truth
  FcTse6   *pTse6;     // Fire control TSE
  FcTseO   *pTseO;     // Fire control weave TSE

//############################################################################//
//
// Function: inv33()
//
// Purpose:  Safely invert a 3x3 matrix using Gaussian elimination.
//
//############################################################################//
bool inv33(Matrix &matIn, Matrix &matOut );

};


#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

