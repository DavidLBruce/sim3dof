////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// TestMat.hpp
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef TESTMAT_HPP
#define TESTMAT_HPP

#include "Matrix.hpp"


class TestMat
{
public:

  TestMat()
  {}

  ~TestMat()
  {}

  void Execute(int argc, char *argv[]);

  void RunTest(int testNo);

  void Help();

private:

  Matrix mat1;

};


#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

