//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       MdlBase.cpp

    \author     David Bruce

    \brief      Model base class.

*/
//############################################################################//


#include "MdlBase.hpp"

//
// C/C++ includes and namespaces
//
#include <vector>
#include <fstream>
using std::ofstream;



//############################################################################//
///
/// <b> Function: </b>  MdlBase()
///                                                                           
/// <b> Purpose: </b>   Constructor                                           
///                                                                           
///
//############################################################################//
MdlBase::MdlBase()
{
  nextFrameTime = 999.0e9;
  frameTimeStep = 999.0e9;
  mdlName = "";
  pSimTime = 0;
  initComp = false;
  intEn = false;
}

//############################################################################//
///
/// <b> Function: </b>  ~MdlBase()
///                                                                           
/// <b> Purpose: </b>   Destructor
///                                                                           
///
//############################################################################//
MdlBase::~MdlBase()
{
}

//############################################################################//
///
/// <b> Function: </b>  Initialize()                                            
///                                                                           
/// <b> Purpose: </b>   Init model                                         .  
///                                                                           
///
//############################################################################//
void MdlBase::Initialize()
{
}

//############################################################################//
///
/// <b> Function: </b>  UpdateDerivatives()                                     
///                                                                           
/// <b> Purpose: </b>   Calculate the derivatives                          .  
///                                                                           
///
//############################################################################//
void MdlBase::UpdateDerivatives()
{
}

//############################################################################//
///
/// <b> Function: </b>  Update()
///
/// <b> Purpose: </b>   Update the states
///
///
//############################################################################//
void MdlBase::Update()
{
}

//############################################################################//
//
// Function: FrameUpdate()
//
// Purpose:  Perform frame updates
//
//############################################################################//
void MdlBase::FrameUpdate()
{
}

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print model output
//
//############################################################################//
void MdlBase::Output( bool printHeader, FILE *pOutFile )
{
}

//############################################################################//
///
/// <b> Function: </b>  Finalize()
///
/// <b> Purpose: </b>   Close out the model.
///
///
//############################################################################//
void MdlBase::Finalize()
{
  pSimTime = 0;
}

//############################################################################//
///
/// <b> Function: </b>  GetStates()
///
/// <b> Purpose: </b>   Get the states
///
///
//############################################################################//
void MdlBase::GetStates(std::vector<double *> &states)
{
  states = m_state;
}

//############################################################################//
///
/// <b> Function: </b>  GetDerivatives()
///
/// <b> Purpose: </b>   Derivatives accessor
///
///
//############################################################################//
void MdlBase::GetDerivatives(std::vector<double *> &derivs)
{
  derivs = m_deriv;
}

//############################################################################//
///
/// <b> Function: </b>  SetStates()
///
/// <b> Purpose: </b>   Set the states
///
///
//############################################################################//
void MdlBase::SetStates(std::vector<double *> &states)
{
  for (unsigned int i = 0; i < states.size();  ++i)
  {
    *m_state[i] = *states[i];
  }
}

//############################################################################//
///
/// <b> Function: </b>  RegisterState()
///
/// <b> Purpose: </b>   Register the states
///
///
//############################################################################//
void MdlBase::RegisterState(double &state, double &deriv)
{
  double *ptr;
  ptr = &state; 
  m_state.push_back(ptr);

  ptr = &deriv; 
  m_deriv.push_back(ptr);

  intEn = true;
}

//############################################################################//
//
// Function: Integrate()  
//                                    
// Purpose:  RK4 integrator.
//                                    
//
//############################################################################//
void MdlBase::Integrate(double   &timeStep )
{
  //   
  // Determine step interval and step dependent values
  //
  double hh = timeStep;
  double h_half = hh / 2.0;
  double tempSimTime = *pSimTime;

  //
  // Define local derivaitve and state containers
  //
  std::vector<double *> pDeriv;
  std::vector<double *> pState;

  //
  // Get current states and store values
  //
  GetStates( pState );

  if ( !initComp )
  {
    initComp = true;

    //
    // Allocate container memory with sizes of state vector
    //
    xn.insert( xn.end(), pState.size(), 0.0 );
    k1.insert( k1.end(), pState.size(), 0.0 );
    k2.insert( k2.end(), pState.size(), 0.0 );
    k3.insert( k3.end(), pState.size(), 0.0 );
    k4.insert( k4.end(), pState.size(), 0.0 );
  }

  for( unsigned int idx = 0; idx < pState.size(); ++idx )
  {
     xn[idx] = *pState[idx];
  }

  //
  // Integrate
  // Step 1
  //
  UpdateDerivatives();
  GetDerivatives( pDeriv );

  for( unsigned int idx = 0; idx < pDeriv.size(); ++idx )
  {
    k1[idx] = *pDeriv[idx] * hh;

    //
    // Compute states for next step while we are here
    //
    *pState[idx] = xn[idx] + k1[idx] / 2.0;
  }

  // 
  // Advance clock half step for Stage 2 and Stage 3
  //
  *pSimTime += h_half;

  //
  // Step 2
  // Apply state step and get new derivatives
  //
  SetStates(pState);
  UpdateDerivatives();
  GetDerivatives(pDeriv);

  for( unsigned int idx = 0; idx < pDeriv.size(); ++idx )
  {
    k2[idx] = *pDeriv[idx] * hh;

    //
    // Compute states for next step while we are here
    //
    *pState[idx] = xn[idx] + k2[idx]/2.0;
  }

  //
  // Step 3
  // Apply state step and get new derivatives
  //
  SetStates(pState);
  UpdateDerivatives();
  GetDerivatives(pDeriv);

  for(unsigned int idx = 0; idx < pDeriv.size(); ++idx)
  {
    k3[idx] = *pDeriv[idx]*hh;

    //
    // Compute states for next step while we are here
    //
    *pState[idx] = xn[idx] + k3[idx];
  }

  //
  // Advance clock another half step (for a complete step) for Stage 4
  //
  *pSimTime += h_half;

  //
  // Step 4
  // Apply state step and get new derivatives
  //
  SetStates(pState);
  UpdateDerivatives();
  GetDerivatives(pDeriv);

  for(unsigned int idx = 0; idx < pDeriv.size(); ++idx)
  {
    k4[idx] = *pDeriv[idx]*hh;

    //
    // Compute states
    //
    *pState[idx] = xn[idx] + k1[idx]/6.0 + k2[idx]/3.0 + k3[idx]/3.0 + 
                                                               k4[idx]/6.0;
  }
  SetStates(pState);

  //
  // Reset clock for this module
  //
  *pSimTime = tempSimTime;
}


//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
  
  
