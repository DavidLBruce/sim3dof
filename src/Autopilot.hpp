//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Autopilot.hpp

  Overview:
  Missile Autopilot model.    3rd order transfer function.

  Author:
  David Bruce

*/
//############################################################################//

#ifndef AUTOPILOT_HPP
#define AUTOPILOT_HPP

#include "MdlBase.hpp"



//
// Forward declarations
//
class Guidance;
class Missile;
class SimExec;
//class Navigation;


//
// Autopilot Class
//
class Autopilot : public MdlBase
{
public:

  //
  // Public scope variables
  //
  
  //
  // User inputs
  //
  double initFrameTime; // sec     Time of first FrameUpdate

  double accLim;        // m/sec^2 Acceleration limit
  double tau;           // sec     Time constant

  //
  // Other public scope vaariables
  //
  double aCmdYbLim;     // m/sec^2 Limited acceleration command
  double ayb;           // m/sec^2 Achieved lateral acceleration

//############################################################################//
//
// Function:  Autopilot()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  Autopilot();

//############################################################################//
//
// Function: </b>  ~Autopilot()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~Autopilot();

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
// Function: SetTse6()
//
// Purpose:  Set the omega Target State Estimator reference pointer.
//
//############################################################################//
  void SetGuidance( Guidance *pGdn_ )
  { pGdn = pGdn_; }

//############################################################################//
//
// Function: SetTse6()
//
// Purpose:  Set the omega Target State Estimator reference pointer.
//
//############################################################################//
  void SetMissile( Missile *pMsl_ )
  { pMsl = pMsl_; }

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
  double aCmdYb;
  double launchTime;
  double simTime;
  double x0;
  double x0d;
  double x1;
  double x1d;
  double x2;
  double x2d;


  //
  // Reference pointers
  //
  SimExec  *pExec;
  Guidance *pGdn;
  Missile  *pMsl;

};


#endif

//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
