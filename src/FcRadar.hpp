//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       FcRadar.hpp

    \author     David Bruce

    \brief      Fire control radar

*/
//############################################################################//

#ifndef FCRADAR_HPP
#define FCRADAR_HPP

#include "MdlBase.hpp"

//
// Forward declarations
//
class SimExec;
class Target;

//
// Fire control class
//
class FcRadar : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double initFrameTime; // sec Time of first FrameUpdate

  double initPxf;  // m       Initial position of radar
  double initPyf;  // m       Initial position of radar

  double rinAng ;  // rad     Range independent angle noise
  double rinRdot;  // m/sec   Range independent range rate noise
  double rinRng;   // m       Range independent range noise
                   //         noise
  //
  // Other public scope variables
  //
  double measAng;  // rad     Measured angle
  double measRdot; // m/sec   Measured range rate
  double measRng;  // m       Measured range

  double pxf;      // m       Radar inertial position
  double pyf;      // m       Radar inertial position

  double timeStamp;// sec     Radar measurement time stamp

  double truAng;   // rad     Angle
  double truRdot;  // m/sec   Range rate
  double truRng;   // m       Range

  double vxf;      // m       Radar inertial velocity
  double vyf;      // m       Radar inertial velocity

//############################################################################//
//
// Function:  FcRadar()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  FcRadar();

//############################################################################//
//
// Function: </b>  ~FcRadar()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~FcRadar();

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
// Function: UpdateDerivatives()                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
  virtual void UpdateDerivatives();

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
  void SetTarget( Target *pTgt_ )
  { pTgt = pTgt_; }


private:

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
  void ResetAll();

  //
  // Class scope variables
  //
  double relPosX;  // m      Target position wrt radar
  double relPosY;  // m      Target position wrt radar
  double relVelX;  // m/sec  Target velocity wrt radar
  double relVelY;  // m/sec  Target velocity wrt radar
  double simTime;  // sec    Simulation time
  double truPosX;  // m      True target positon wrt Radar
  double truPosY;  // m      True target positon wrt Radar
  double truVelX;  // m      True target velocity wrt Radar
  double truVelY;  // m      True target velocity wrt Radar

  //
  // Reference pointers
  //
  SimExec *pExec;
  Target  *pTgt;

};

#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

