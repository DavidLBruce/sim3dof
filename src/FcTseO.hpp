//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       FcTseO.hpp

    \author     David Bruce

    \brief      Fire control radar

*/
//############################################################################//

#ifndef FCTSE_HPP
#define FCTSE_HPP

#include "MdlBase.hpp"

//
// Sim includes
//
//#include "genMath.h"
#include "Matrix.hpp"

//
// Forward declarations
//
class SimExec;
class FcRadar;


//
// Fire control class
//
class FcTseO : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double initFrameTime;   // sec     Time of first FrameUpdate

  double initOmega;       // rad/sec Initial radian frequency estimate
  double initPosY;        // m       Initial position estimate
  double initPosYsigScale; // m      Initial position std dev scaling
  double initVelY;        // m/sec   Initial velocity estimate
  double initVelYsig;     // m/sec   Initial velocity std dev
  double initWsig;        // rad/sec Initial radian frequency std dev
  double procNoise;       // na      Omega squared process noise

  //
  // Other public scope variables
  //
  double estPosY;         // m       Estimated position
  double estPosYdot;      // m/sec   Estimated velocity
  double estOmega;        // rad/sec Estimated radian frequency 
  double estOmega2;       // rad/sec Estimated omega squared

//############################################################################//
//
// Function:  FcTseO()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  FcTseO();

//############################################################################//
//
// Function: </b>  ~FcTseO()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~FcTseO();

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
// Purpose:  Set the target reference pointer.
//
//############################################################################//
  void SetRadar( FcRadar *pRdr_ )
  { pRdr = pRdr_; }


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
  void UpdateStates( double measPosY );

//############################################################################//
//
// Function: IntegrateRK2()
//
// Purpose:  2nd order Runge Kutta integrator
//
//############################################################################//
  Matrix IntegrateRK2( Matrix &XX, double timeStep );

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
// Function: ResetAll()
//
// Purpose:  Clear selected variables
//
//############################################################################//
  void ResetAll();

  //
  // Class scope variables
  //
  Matrix   eye;        // na     Identity matrix
  Matrix   HH;         // na     Measurement matrix
  Matrix   HT;         // na     Transpose of the measurement matrix
  bool     initialized;// na     Filter has been initialized
  Matrix   innovation; // na     Kalman innovation
  Matrix   KK;         // na     Kalman gains
  double   measPosY;   // m      Measured Y position
  Matrix   phi;        // na     State transition matrix
  Matrix   PP;         // na     Estimation covariance matrix
  double   residual;   // m      Update residual     
  double   rinPosY;    // m      Std dev of Y position measurement
  double   RR;         // na     Measurement covariance matrix
  Matrix   QQ;         // na     Process noise matrix
  double   simTime;    // sec    Simulation time
  Matrix   stateVec;    // na     State vector

  //
  // Reference pointers
  //
  SimExec *pExec;
  FcRadar *pRdr;

};


#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

