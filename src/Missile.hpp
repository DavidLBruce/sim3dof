//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
//                                                                            //
//                    Copyright (c) 2013, Raytheon Company                    //
//                              All Rights Reserved                           //
//                                                                            //
//                              Raytheon Company                              //
//                              P.O. Box 11337                                //
//                              Tucson, AZ 85734-1337                         //
//                                                                            //
//############################################################################//
/*
  Missile.hpp

  Overview:
  Missile model

  Author:
  David Bruce

*/
//############################################################################//

#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "MdlBase.hpp"



//
// Forward declarations
//
class Autopilot;
class SimExec;


//
// Missile Class
//
class Missile : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double initFrameTime; // sec     Time of first FrameUpdate

  double initVxb;       // m/sec   Initial velocity
  double launchTime;    // sec     Launch time

  //
  // Other public scope vaariables
  //
  double axb;           // m/sec^2 Axial acceleration   
  double axf;           // m/sec^2 Inertial acceleration
  double ayb;           // m/sec^2 Lateral acceleration 
  double ayf;           // m/sec^2 Inertial acceleration
  double pxf;           // m       Inertial position
  double pyf;           // m       Inertial position
  double theta;         // rad     Flight path angle (or heading)
  double vxb;           // m/sec   Axial velocity   
  double vxf;           // m/sec   Inertial velocity
  double vyb;           // m/sec   Lateral velocity 
  double vyf;           // m/sec   Inertial velocity

//############################################################################//
//
// Function:  Missile()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  Missile();

//############################################################################//
//
// Function: </b>  ~Missile()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~Missile();

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
  // Class scope variables
  //
  bool   initialized;
  double simTime;

  //
  // Reference pointers
  //
  Autopilot *pAp;
  SimExec   *pExec;

};


#endif

//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
