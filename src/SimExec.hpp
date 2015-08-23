//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  SimExec.hpp

  Overview:
  Simulation executive

  Author:
  David Bruce

*/
//############################################################################//

#ifndef SIM_EXEC_HPP
#define SIM_EXEC_HPP

#include "MdlBase.hpp"

//
// C/C++ includes and namespaces
//
#include <cstdio>
#include <vector>
using std::vector;

//
// Sim modules
//
#include "Autopilot.hpp"
#include "FcRadar.hpp"
#include "FcTseO.hpp"
#include "FcTse6.hpp"
#include "Guidance.hpp"
#include "Missile.hpp"
#include "MslTse6.hpp"
#include "Seeker.hpp"
#include "Target.hpp"


class SimExec : public MdlBase
{

public:

  //
  // Public scope variables
  //

  //
  // Input parameters
  //
  char   fileStub[80];      // na      Output file name stub
  double flyerThld;         // m       Threshold for flyer miss distance
  double maxTime;           // sec     Maximum amount of simulation time
  double outputRate;        // Hz      Trajectory data output rate
  double pHitThld;          // m       Miss threshold for pHit calculation
  bool   printAll;          // na      Flag directing to print all Monte
                            //         Carlo run miss data
  int    runs;              // na      Number of runs in the Monte Carlo set
  int    runStart;          // na      First run to be made
  double tFinal;            // sec     Initial tgo
  double timeStep;          // sec     Simulation frame time step
  double xa;                // rad     Crossing angle

  //
  // Other public scope variables
  //
  double miss;              // m       Miss distance
  double miss20;            // m       20th percentile miss distance
  double miss35;            // m       35th percentile miss distance
  double miss70;            // m       70th percentile miss distance
  double missX;             // m       Miss in X direction
  double missY;             // m       Miss in Y direction
  double *pSimTime;         // na      Pointer to simTime
  double range;             // m       Slant range from missile to target
  double rDot;              // m/sec   Range rate
  double relPxf;            // m       Position of target wrt missile
  double relPyf;            // m       Position of target wrt missile
  double relVmag;           // m/sec   Magnitude of relative velocity
  double relVxf;            // m/sec   Velocity of target wrt missile
  double relVyf;            // m/sec   Velocity of target wrt missile
  long   seed;              // na      Simulation random number seed
  double simTime;           // sec     Simulation time

//############################################################################//
//
// Function:  SimExec()
//                                
// Purpose:   Constructor
//                                
//############################################################################//
  SimExec();

//############################################################################//
//
// Function: </b>  ~SimExec()
//
// Purpose: </b>   Destructor
//
//############################################################################//
  virtual ~SimExec();

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
// Function: Update()
//
// Purpose:  Update the state
//
//############################################################################//
  virtual void Update();

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
// Function: Finalize()
//
// Purpose:  Close out the model.
//
//
//############################################################################//
  virtual void Finalize();

//############################################################################//
//
// Function: Execute()
//
// Purpose:  Run the simulation
//
//############################################################################//
  void Execute( int argc, char *argv[] );

//############################################################################//
//
// Function: Execute()
//
// Purpose:  Run the simulation
//
//############################################################################//
  void Integrate();

//############################################################################//
//
// Function: Output()
//
// Purpose:  Print selected variables
//
//############################################################################//
  void Output( bool printHeader,
               FILE *pOutFile );

private:

  //
  // Class scope typedefs
  //
  typedef vector<double> vectorDbl;

  //
  // Class scope functions
  //

//############################################################################//
//
// Help()
//
// Overview:
//
//  List the input parameters and their descriptions, then exit..
//
//############################################################################//
  void Help();


  void ResetAll();

  void process_command_line( int  argc,
                             char *argv[] );

  void get_args( char *in, 
                 char *name, 
                 char *val );

  int  get_boolean(char *str);

  int  get_list_index( char *val, 
                       char **list, 
                       int  max_index );

  double get_double( double *var, 
                     char *val, 
                     double scale_factor );

  void parse_symbols( char *name, char *val );

  void read_file( char *filename );

  const char *boolean_string( int val );

  void AddModel( MdlBase *pMdl, string mdlName );

  double CalcStdDev( vectorDbl dataVec );

  //
  // Class scope variables
  //
  FILE   *pOutFile;  // Output file pointer

  vector<MdlBase*> mdlVec;  // na      Vector of pointers to all sim modules
  MdlBase   *pMdl;          // na      Pointer to selected module    

  double    avgMissMag;     // m       Average of miss magnitudes
  double    avgMissX;       // m       Average of miss distance along X
  double    avgMissY;       // m       Average of miss distance along X
  double    avgXang;        // rad     Average crossing angle
  double    deltaTime;      // sec     The current simulation time step
  bool      done;           // na      Run completion flag
  char      filename[160];  // na      Name of the output data file
  int       hitCnt;         // na      Hit counter for pHit calculation
  double    maxMiss;        // m       Max miss distnace
  double    medMiss;        // m       Median miss distance
  double    minMiss;        // m       Min miss distance
  vectorDbl missVec;        // m       Collection of run set miss data
  vectorDbl missNonFlyers;  // m       Collection of run set miss data
  vectorDbl missXvec;       // m       Collection of run set miss data
  vectorDbl missYvec;       // m       Collection of run set miss data
  int       nonFlyerCnt;    // na      Quanty of non flyers
  double    outputTime;     // sec     Time of next run data output event
  double    outputTimeStep; // sec     Time step of run data output
  double    pHit;           // na      Probability of hit (m out of n)
  int       qtyFly;         // na      Quantity of miss > flyer threshold
  int       run;            // na      Current run number
  int       runIdx;         // na      Run counter
  int       runSeed;        // na      Run seed
  double    stdMissX;       // m       Standard deviation of miss along X
  double    stdMissY;       // m       Standard deviation of miss along Y
  double    Xang;           // rad     Crossing angle.  Supplement of angle
                            //         between tgt & msl velocity vectors

  //
  // Simulation modules
  //
  Autopilot  ap;            // Autopilot
  FcRadar    fcRdr;         // Fire control radar
  FcTseO     fcTseO;        // Fire control omega TSE
  FcTse6     fcTse6;        // Fire control MSC TSE
  Guidance   gdn;           // Guidance
  Missile    msl;           // Projectile
  MslTse6    mslTse6;       // Projectile TSE
  Seeker     skr;           // Seeker
  Target     tgt;           // Target

};

#endif

//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
  
