//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
  Seeker.hpp

  Author:
  David Bruce

  Overview:
  Projectile seeker


*/
//############################################################################//

#ifndef SEEKER_HPP
#define SEEKER_HPP

#include "MdlBase.hpp"

//
// Forward declarations
//
class Missile;
class SimExec;
class Target;

//
// Fire control class
//
class Seeker : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //

  // Type  Variable        Units  Description
  double detRng;        // m      Detection / acquisition range  
  double fov;           // rad    Field of view / regard
  double initFrameTime; // sec    Time of first FrameUpdate
  double rinAng ;       // rad    Range independent angle noise

  //
  // Other public scope variables
  //
  double measAng;       // rad     Measured angle
  bool   measValid;     // na      Measurement is valid
  double timeStamp;     // sec     Seeker measurement time stamp
  double truAng;        // rad     Tru Line of sight Angle


//############################################################################//
//
// Function:  Seeker()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  Seeker();

//############################################################################//
//
// Function: </b>  ~Seeker()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~Seeker();

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
// Function: SetMissile()
//
// Purpose:  Set the projectile reference pointer.
//
//############################################################################//
  void SetMissile( Missile *pMsl_ )
  { pMsl = pMsl_; }

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
  double pxf;      // m       Projectile inertial position
  double pyf;      // m       Projectile inertial position
  double relPosX;  // m       Target position wrt projectile
  double relPosY;  // m       Target position wrt projectile
  double relVelX;  // m/sec   Target velocity wrt projectile
  double relVelY;  // m/sec   Target velocity wrt projectile
  double simTime;  // sec     Simulation time
  double truPosX;  // m       True target positon 
  double truPosY;  // m       True target positon 
  double truRdot;  // m/sec   True target range rate wrt Projectile
  double truRng;   // m       True target range wrt Projectile
  double truVelX;  // m       True target velocity
  double truVelY;  // m       True target velocity
  double vxf;      // m       Projectile inertial velocity
  double vyf;      // m       Projectile inertial velocity

  //
  // Reference pointers
  //
  SimExec *pExec;
  Missile *pMsl;
  Target  *pTgt;

};

#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//

