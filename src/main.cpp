//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
/*
  main.cpp

  Overview:
  Simulation entry point

  Author:
  David Bruce

  Reference:
  P. Zarchan, "Fundamentals of Kalman Filtering:  A Practical Aproach", 2nd
  edition, 2005, American Institute of Aeronautics and Astronautics, Inc.,
  Reston, Virginia,
  

*/
//############################################################################//

#include "SimExec.hpp"


//############################################################################//
//
// main()()
//
// Overview:
//  Entry point of the simulation.
//
//############################################################################//
int main( int argc, char *argv[] )
{ 
  SimExec simExec;

  simExec.Execute( argc, argv );

  return 0;  

}  // end main()




//############################################################################//
//                                UNCLASSIFIED                                //
//############################################################################//
