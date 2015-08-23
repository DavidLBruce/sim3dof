//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Guidance.hpp

  Overview:
  Missile Guidance model

  Author:
  David Bruce

*/
//############################################################################//

#ifndef GUIDANCE_HPP
#define GUIDANCE_HPP

#include "MdlBase.hpp"



//
// Forward declarations
//
class Autopilot;
class FcRadar;
class FcTse6;
class Missile;
class MslTse6;
class SimExec;
class Target;
//class Navigation;


//
// Guidance Class
//
class Guidance : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double initFrameTime; // sec     Time of first FrameUpdate

  int    gdnLawSelInt;  // na      Guidance law selection
                        //         1 = Proportion Navigation
                        //         2 = Weave guidance
  double kpn;           // na      Pro Nav gain
  bool   useTruth;      // na      Use truth for target state
  double tauCtrl;       // sec     Optimal guidance tuning parameter

  //
  // Other public scope vaariables
  //
  double aCmdYb;
  double tgo;

//############################################################################//
//
// Function:  Guidance()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  Guidance();

//############################################################################//
//
// Function: </b>  ~Guidance()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~Guidance();

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
// Function: FrameUpdate
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
// Purpose:  Set the Simulation Executive reference pointer.
//
//############################################################################//
  void SetSimExec( SimExec *pExec_ )
  { pExec = pExec_; }

//############################################################################//
//
// Function: SetMissile()
//
// Purpose:  Set the Missile reference pointer.
//
//############################################################################//
  void SetMissile( Missile *pMsl_ )
  { pMsl = pMsl_; }

//############################################################################//
//
// Function: SetMslTse()
//
// Purpose:  Set the Missile reference pointer.
//
//############################################################################//
  void SetMslTse( MslTse6 *pMslTse_ )
  { pMslTse = pMslTse_; }

//############################################################################//
//
// Function: SetRadar()
//
// Purpose:  Set the Radar reference pointer.
//
//############################################################################//
  void SetRadar( FcRadar *pRdr_ )
  { pRdr = pRdr_; }

//############################################################################//
//
// Function: SetTarget()
//
// Purpose:  Set the Target reference pointer.
//
//############################################################################//
  void SetTarget( Target *pTgt_ )
  { pTgt = pTgt_; }

//############################################################################//
//
// Function: SetTse6()
//
// Purpose:  Set the 6 state Target State Estimator reference pointer.
//
//############################################################################//
  void SetTse6( FcTse6 *pTse6_ )
  { pTse6 = pTse6_; }

//############################################################################//
//
// Function: SetAutopilot()
//
// Purpose:  Set the Autopilot reference pointer.
//
//############################################################################//
  void SetAutopilot( Autopilot *pAp_ )
  { pAp = pAp_; }

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
  // Class scope typedefs
  // 
  typedef enum GDNLAWSEL
  {
    glNULL = 0,
    glPN,
    glWEAVE
  }GdnLawSel;

  //
  // Class scope variables
  //
  double    estAccX;       // m/sec^2 Estimated acceleration along LOS
  double    estAccY;       // m/sec^2 Estimated acceleration across LOS
  double    estLosAng;     // rad     Estimated LOS angle
  double    estLosr;       // rad/sec Estimated LOS rate
  double    estRng;        // m       Estimated range
  double    estRdot;       // m/sec   Estimated range rate
  GdnLawSel gdnLawSel;
  bool      initialized;   // na      Initializatio has happened
  double    kWeave;        // na      Weave guidance gain
  double    launchTime;    // sec     Launch time
//  double    nextUpLinkTime;// sec     Time of next datalink uplink
//  double    rDot;          // m/sec   Range rate wrt missile
  double    relPxf;        // m       Relative position
  double    relPyf;        // m       Relative position
  double    relVxf;        // m/sec   Relative velocity
  double    relVyf;        // m/sec   Relative velocity
//  double    rdrPxf;        // m       Radar inertial position
//  double    rdrPyf;        // m       Radar inertial position
//  double    rdrVxf;        // m       Radar inertial velocity
//  double    rdrVyf;        // m       Radar inertial velocity
//  double    rng;           // m       Range
//  double    rng2;          // m^2     Range sqruared
  double    simTime;       // sec     Simulation time
  double    tgtAxf;        // m/sec^2 Target acceleration
  double    tgtAyf;        // m/sec^2 Target acceleration
  double    tgtPxf;        // m       Target position
  double    tgtPyf;        // m       Target position
  double    tgtVxf;        // m/sec   Target velocity
  double    tgtVyf;        // m/sec   Target velocity
  double    velClose;      // m/sec   Closing velocity

  //
  // Reference pointers
  //
  Autopilot *pAp;
  SimExec   *pExec;
  Missile   *pMsl;
  MslTse6   *pMslTse;
  FcRadar   *pRdr;
  Target    *pTgt;
  FcTse6    *pTse6;   // Tuned to optimize position estimate
};


#endif

//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
