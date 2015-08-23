//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  Target.hpp

  Overview:
  Target model

  Author:
  David Bruce

*/
//############################################################################//

#ifndef TARGET_HPP
#define TARGET_HPP

#include "MdlBase.hpp"



//
// Forward declarations
//
class SimExec;


//
// Target Class
//
class Target : public MdlBase
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
  double weaveStart;    // sec     Time to start weave
  double weaveEnd;      // sec     Time to stop weave
  double weaveAmpl;     // m/sec^2 Weave acceleration magnitude
  double weavePeriod;   // sec     Weave period

  //
  // Other public scope vaariables
  //
  double axb;           // m/sec^2 Axial acceleration   
  double axf;           // m/sec^2 Inertial acceleration
  double ayb;           // m/sec^2 Lateral acceleration 
  double ayf;           // m/sec^2 Inertial acceleration
  double jxf;           // m/sec^3 Inertial jerk
  double jyf;           // m/sec^3 Inertial acceleration
  double omega;         // rad/sec Weave radian frequency
  double pxf;           // m       Inertial position
  double pyf;           // m       Inertial position
  double theta;         // rad     Flight path angle (or heading)
  double vxb;           // m/sec   Axial velocity   
  double vxf;           // m/sec   Inertial velocity
  double vyb;           // m/sec   Lateral velocity 
  double vyf;           // m/sec   Inertial velocity
  bool   weaveInit;     // na      Weave has been initialized

//############################################################################//
//
// Function:  Target()
//                                
// Purpose:   Constructor
//                                
//
//############################################################################//
  Target();

//############################################################################//
//
// Function: </b>  ~Target()
//
// Purpose: </b>   Destructor
//
//
//############################################################################//
  virtual ~Target();

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
  double simTime;
  double weavePhase;

  //
  // Reference pointers
  //
  SimExec *pExec;

};


#endif

//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
