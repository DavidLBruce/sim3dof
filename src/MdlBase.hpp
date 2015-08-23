//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/**
    \file       MdlBase.hpp

    \author     David Bruce

    \brief      Model base class.

*/
//############################################################################//

#ifndef MDLBASE_HPP
#define MDLBASE_HPP

//
// C/C++ includes and namespaces
//
#include <cstdio>
#include <vector>
#include <fstream>
using std::ofstream;

#include <string>
using std::string;

//
// Global scope constants
//
const double PI =             3.14159265358979323846264;
const double TWOPI =          6.28318530717958647692528;
const double HALF_PI =        PI / 2.0;
const double EXP =            2.718281828459045235360287471352662;

const double DEG2RAD =        0.017453292519943295769236907684886;
const double RAD2DEG =        57.295779513082320876798154814105;
const double r2d =            RAD2DEG;
const double d2r =            DEG2RAD;
const double FT2M =           0.3048;
const double M2FT =           3.2808398950131233598025842779222;

const double BOLTZ =          1.3806503e-23;        // (m^2 kg)/(s^2 K)
const double SPEED_OF_LIGHT = 299792458.0;          // m/s
const double FOURPI3_DB =     20.93509609410364014; // 10*log(4*pi^3)
const double GRAVITY =        9.81;
const double LBF2N =          4.448222;

class MdlBase
{
public:

  typedef enum _GUIDLAW_TYPES_
  {
    TRUTH_GL,
    PN
  } GUIDLAW_TYPES;
  
  typedef enum _SENSOR_TYPES_
  {
    EO_ONLY,
    EO_AND_LADAR,
    SENSOR_TRUTH
  } SENSOR_TYPES;

  typedef enum _thruster_type_
  {
    DACST,
    AXIAL
  } ENGINE_TYPE;
  
  typedef enum _aftype_
  {
    SIMPLE_APAF,    // 0 = simple cascaded first-order model
    NMP_APAF,       // 1 = nonminimum phase model
    MP_APAF,        // 2 = minimum phase model
    DA_APAF,        // 3 = divert assisted model
    HIFI_APAF,      // 4 = Curt Mracek's hifi AP/AF model
    BMI_APAF,       // 5 = body motion isolation test AP/AF model
    FO_DIV,         // 6 = parameterized divert and attitude control system
  } AF_TYPE;
  
  typedef enum _theta_control_flag_
  {
    AEROCONTROL,
    TVC,
    ACS,
    ACSPOPPERS,
    TVCACS
  } PITCH_AP;
  
  typedef enum _acs_flag_
  {
    THIRD_ORD,
    FIRST_ORD
  } ACS_AP;

  //
  // Public variables
  //  
  double frameTimeStep;
  string mdlName;
  double nextFrameTime;
  double *pSimTime;
  bool   intEn;



//############################################################################//
//
// Function: MdlBase
//                                                                       
// Purpose:  Constructor                                           
//                                                                           
//
//############################################################################//
  MdlBase();

//############################################################################//
//
// Function: MdlBase
//                                                                       
// Purpose:  Destructor
//                                                                           
//############################################################################//
  virtual ~MdlBase();

//############################################################################//
//
// Function: Initialize                                            
//                                                                       
// Purpose:  Init model                                         .  
//                                                                           
//############################################################################//
  virtual void Initialize();

//############################################################################//
//
// Function: UpdateDerivatives                                     
//                                                                       
// Purpose:  Calculate the derivatives                          .  
//                                                                           
//############################################################################//
  virtual void UpdateDerivatives();

//############################################################################//
//
// Function: Update
//
// Purpose:  Update the state
//
//############################################################################//
  virtual void Update();

//############################################################################//
//
  // Function: FrameUpdate()
//
// Purpose:  Perform frame updates
//
//############################################################################//
  virtual void FrameUpdate();

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print model output
//
//############################################################################//
  virtual void Output( bool printHeader, FILE *pOutFile );

//############################################################################//
//
// Function: Finalize
//
// Purpose:  Close out the model.
//
//############################################################################//
  virtual void Finalize();

//############################################################################//
//
// Function:   getStates
//
// Purpose:    State accessor
//
//############################################################################//
  void GetStates(std::vector<double *> &states);

//############################################################################//
//
// Function: </b>  setDerivatives
//
// Purpose: </b>   Derivatives accessor
//
//############################################################################//
  void GetDerivatives(std::vector<double *> &derivs);

//############################################################################//
//
// Function: setStates
//
// Purpose:  Set the states
//
//############################################################################//
  void SetStates(std::vector<double *> &states);

//############################################################################//
//
// Function: RegisterState
//
// Purpose:  Register the states
//
//############################################################################//
  void RegisterState(double &state, double &deriv);

//############################################################################//
//
// Function: Integrate()  
//                                    
// Purpose:  RK4 integrator.
//                                    
//
//############################################################################//
  void Integrate( double   &timeStep );

//############################################################################//
//
// Function: SetSimTime
//
// Purpose:  Set the simulation time pointer
//
//############################################################################//
  void SetSimTime( double *pSimTime_ )
  {
    pSimTime = pSimTime_;
  }

//############################################################################//
//
// Function: GetTime()
//
// Purpose:  Get the simulation time
//
//############################################################################//
  double GetSimTime()
  {
    return *pSimTime;
  }

protected:

private:

  std::vector<double *> m_state;
  std::vector<double *> m_deriv;

  std::vector<double> xn;
  std::vector<double> k1;
  std::vector<double> k2;
  std::vector<double> k3;
  std::vector<double> k4;

  bool initComp;
};

#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
  
  
