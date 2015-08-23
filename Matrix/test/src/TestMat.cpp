////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// TestMat.hpp
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "TestMat.hpp"

#include<exception>

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace MATRIX;

void TestMat::Execute(int argc, char *argv[])
{
  int qtyTests(44);
  int testNo(0);
  int start;
  int finish;

  printf("\n");

  switch (argc)
  {
    case 1:
    {
      for (testNo = 0; testNo < qtyTests; ++testNo)
      {
        RunTest(testNo);
      }
      break;
    }
  
    case 2:
    {
      if (strcmp("-h", argv[1]) == 0)
      {
        Help();
      }
      else
      {
        testNo = atoi(argv[1]);
        RunTest(testNo);
      }
      break;
    }

    case 3:
    {
      start = atoi(argv[2]);
      finish = atoi(argv[3]);
      for (testNo = start; testNo < finish; ++testNo)
      {
        RunTest(testNo);
      }
      break;
    }

    default:
    {
      Help();
      break;
    }
  }  // end switch (argc)

}  // end Execute()

void TestMat::Help()
{
  printf("Usage: \n");
  printf("'TestMat -h'            Returns this usage help. \n");
  printf("'TestMat'               Runs all the tests. \n");
  printf("'TestMat testNo'        Runs a single test. \n");
  printf("'TestMat start finish'  Runs a subset of tests. \n\n");
}

void TestMat::RunTest(int testNo)
{
  int col(0);
  int idx(0);
  int row(0);
  double val(0.0);
  double *pVal(NULL);

  try
  {
    switch (testNo)
    {
      //
      // Default constructor
      //
      case 0:
      {
        printf("Test 0:  Default matrix: \n");
        mat1.Print("%5.2f");
    
        break;
      }

      //
      // Resize() function
      //    
      case 1:
      {
        printf("Test 1:  Default 3x3 matrix: \n");
        mat1.Resize(3,3);
        mat1.Print("%5.2f");
    
        break;
      }

      //
      // GetPtr() function
      //
      case 2:
      {
        printf("Test 2: mat.GetPtr(3,3) = 0:8 \n");

        mat1.Resize(3,3);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            pVal = mat1.GetPtr(row,col);
            *pVal = static_cast<double>(idx);
            ++idx;
          }
        }
    
        mat1.Print("%5.2f");
    
        break;
      }

      //
      // () operator for 2d matrix    
      //
      case 3:
      {
        printf("Test 3: mat(3,3) = 0:8 \n");

        mat1.Resize(3,3);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        mat1.Print("%5.2f");
    
        break;
      }

      //
      // () operator for 1d vector
      //    
      case 4:
      {
        printf("Test 4: vec(idx) = 0:2 \n");

        mat1.Resize(3,1);
    
        idx = 0;
        for (idx = 0; idx < 3; ++idx)
        {
          mat1(idx) = static_cast<double>(idx);
        }
    
        mat1.Print("%5.2f");
    
        break;
      }

      //
      // GetRow() function
      //    
      case 5:
      {
        printf("Test 5: Get row 1 \n");
        printf("Also tests copy constructor and assignment operator \n\n");
    
        Matrix row1;
    
        mat1.Resize(3,4);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            pVal = mat1.GetPtr(row,col);
            *pVal = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat.GetPtr(3,3) = 0:11 \n");
        mat1.Print("%6.2f");
    
        row1 = mat1.GetRow(1);
    
        printf("\n");
        printf("Get row 1 \n");
        row1.Print("%6.2f");
    
        break;       
      }
    
      //
      // GetCol() function
      //
      case 6:
      {
        printf("Test 6: Get col 1 \n");
        printf("Also tests copy constructor and assignment operator \n\n");
    
        Matrix col1;
    
        mat1.Resize(3,4);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat(3,4) = 0:11 \n");
        mat1.Print("%6.2f");
    
        col1 = mat1.GetCol(1);
    
        printf("\n");
        printf("Get col 1 \n");
        col1.Print("%6.2f");
    
        break;       
      }
    
      //
      // Scalar assignment operator
      //
      case 7:
      {
        printf("Test 7: mat = 1.0 \n");
    
        mat1.Resize(3,4);
    
        mat1 = 1.0;

        mat1.Print("%6.2f");

        break;
      }
      
      //
      //  Binary + operator
      //
      case 8:
      {
        printf("Test 8: mat3 = mat1 + mat2 \n");
    
        Matrix mat2(3,4);
        Matrix mat3;
    
        mat1.Resize(3,4);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0:11 \n");
        mat1.Print("%6.2f");
        printf("\n");
    
        idx = 11;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            mat2(row,col) = static_cast<double>(idx);
            --idx;
          }
        }
    
        printf("mat2(3,4) = 11:-1:0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat3 = mat1 + mat2;

        printf("mat3 = mat1 + mat2 \n");
        mat3.Print("%6.2f");

        break;
      }

      //
      //  Binary - operator
      //
      case 9:
      {
        printf("Test 9: mat3 = mat1 - mat2 \n");
    
        Matrix mat2(3,4);
        Matrix mat3;
    
        mat1.Resize(3,4);
    
        idx = 0;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0:11 \n");
        mat1.Print("%6.2f");
        printf("\n");
    
        idx = 11;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 4; ++col)
          {
            mat2(row,col) = static_cast<double>(idx);
            --idx;
          }
        }
    
        printf("mat2(3,4) = 11:-1:0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat3 = mat1 - mat2;

        printf("mat3 = mat1 - mat2 \n");
        mat3.Print("%6.2f");

        break;
      }

      //
      //  Binary / operator
      //
      case 10:
      {
        printf("Test 10: Element X element division mat3 = mat1 / mat2 \n");
    
        Matrix mat2(3,4, 2.0);
        Matrix mat3;
    
        mat1.Resize(3,4, 6.0);
    
        printf("mat1.Resize(3,4, 6.0) \n");
        mat1.Print("%6.2f");
        printf("\n");
    
        printf("mat2(3,4, 2.0) \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat3 = mat1 / mat2;

        printf("mat3 = mat1 / mat2 \n");
        mat3.Print("%6.2f");

        break;
      }

      //
      //  Binary matrix * operator
      //
      case 11:
      {
        printf("Test 11: mat3 = mat1 * mat2 \n");
    
        Matrix mat2(3,2);
        Matrix mat3;
    
        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");
    
        idx = 5;
        for (int row = 0; row < 3; ++row)
        {
          for (col = 0; col < 2; ++col)
          {
            mat2(row,col) = static_cast<double>(idx);
            --idx;
          }
        }
    
        printf("mat2(3,2) = 5:-1:0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat3 = mat1 * mat2;

        printf("mat3 = mat1 * mat2 \n");
        mat3.Print("%6.2f");

        break;
      }

      //
      //  Binary vector vector dot product
      //
      case 12:
      {
        printf("Test 12: mat1 = vec1' * vec2 \n");
    
        Matrix vec1(1,3);
        Matrix vec2(3,1);
    
        for (int col = 0; col < 3; ++col)
        {
          vec1(col) = 2.0 * static_cast<double>(col);
        }
    
        printf("vec1(1,3) = 0:3 \n");
        vec1.Print("%6.2f");
        printf("\n");
    
        for (int row = 0; row < 3; ++row)
        {
          vec2(row) = static_cast<double>(row);
        }
    
        printf("vec2(3,1) = 0 : 2 : 6 \n");
        vec2.Print("%6.2f");
        printf("\n");

        mat1 = vec1 * vec2;

        printf("mat1 = vec1 * vec2 \n");
        mat1.Print("%6.2f");

        break;
      }

      //
      //  Binary vector vector matrix product
      //
      case 13:
      {
        printf("Test 13: mat1 = vec1 * vec2' \n");
    
        Matrix vec1(3,1, 2.0);
        Matrix vec2(1,3, 3.0);
    
        for (int row = 0; row < 3; ++row)
        {
          vec1(row) = static_cast<double>(row);
        }
    
        printf("vec1(3,1) = 0 : 2) \n");
        vec1.Print("%6.2f");
        printf("\n");

        for (int col = 0; col < 3; ++col)
        {
          vec2(col) = 2.0 * static_cast<double>(col);
        }
    
        printf("vec2(1,3) = 0 : 2 : 4) \n");
        vec2.Print("%6.2f");
        printf("\n");
    
        mat1 = vec1 * vec2;

        printf("mat1 = vec1 * vec2 \n");
        mat1.Print("%6.2f");

        break;
      }

      //
      //  + binary operator for scalar
      //
      case 14:
      {
        printf("Test 14: mat2 = mat1 + 2.0 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = mat1 + 2.0;

        printf("mat2 = mat1 + 2.0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  - binary operator for scalar
      //
      case 15:
      {
        printf("Test 15: mat2 = mat1 - 2.0 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = mat1 - 2.0;

        printf("mat2 = mat1 - 2.0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  * binary operator for scalar
      //
      case 16:
      {
        printf("Test 16: mat2 = mat1 * 2.0 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = mat1 * 2.0;

        printf("mat2 = mat1 * 2.0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  / binary operator with a scalar
      //
      case 17:
      {
        printf("Test 17: mat2 = mat1 / 2.0 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(2 * idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0 : 2 : 10 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = mat1 / 2.0;

        printf("mat2 = mat1 / 2.0 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  [] operator
      //
      case 18:
      {
        printf("Test 18: mat1[2][3] = 0 : 5 \n");

        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1[row][col] = static_cast<double>(idx);
            ++idx;
          }
        }
    
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      //
      //  Invert(), Inv() functions
      //
      //  Example from Cheney & Kincaid, p. 258
      //
      case 19:
      {
        printf("Test 19: mat2 = Invert(mat1) \n");

        Matrix mat2;
        Matrix mat3;

        mat1.Resize(4,4);

        mat1(0,0) =   3.0;
        mat1(0,1) = -13.0;
        mat1(0,2) =   9.0;
        mat1(0,3) =   3.0;

        mat1(1,0) =  -6.0;
        mat1(1,1) =   4.0;
        mat1(1,2) =   1.0;
        mat1(1,3) = -18.0;

        mat1(2,0) =   6.0;
        mat1(2,1) =  -2.0;
        mat1(2,2) =   2.0;
        mat1(2,3) =   4.0;  
    
        mat1(3,0) =  12.0;
        mat1(3,1) =  -8.0;
        mat1(3,2) =   6.0;
        mat1(3,3) =  10.0;  
    
        printf("mat1 = rand(3,3) (from Octave) \n");
        mat1.Print("%7.2f");
        printf("\n");

        printf("mat2 = Invert(mat1) \n");

        mat2 = Invert(mat1);

        mat2.Print("%7.2f");
        printf("\n");

        mat3 = mat2 * mat1;

        printf("mat3 = mat2 * mat1 (= eye(3,3)) \n");

        mat3.Print("%7.2f");
        printf("\n");

        break;
      }

      case 20:
      {
        printf("Test 20: mat1 += mat2 \n");
    
        Matrix mat2(3,4);
        int    idx(0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            mat2(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("mat2(3,4) = 0 : 11 \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat1 += mat2;

        printf("mat1 += mat2 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 21:
      {
        printf("Test 21: mat1 -= mat2 \n");
    
        Matrix mat2(3,4);
        int    idx(0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            mat2(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("mat2(3,4) = 0 : 11 \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat1 -= mat2;

        printf("mat1 -= mat2 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 22:
      {
        printf("Test 22 Element x element multiplication: mat1 *= mat2 \n");
    
        Matrix mat2(3,4, 2.0);

        mat1.Resize(3,4, 3.0);

        printf("mat1.Resize(3,4, 3.0) \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("mat2(3,4, 2.0) \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat1 *= mat2;

        printf("mat1 *= mat2 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 23:
      {
        printf("Test 23 Element x element division: mat1 /= mat2 \n");
    
        Matrix mat2(3,4, 2.0);

        mat1.Resize(3,4, 6.0);

        printf("mat1.Resize(3,4, 6.0) \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("mat2(3,4, 2.0) \n");
        mat2.Print("%6.2f");
        printf("\n");

        mat1 /= mat2;

        printf("mat1 /= mat2 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 24:
      {
        printf("Test 24: mat1 += scalar \n");
    
        int    idx(0);
        double scalar(2.0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("scalar = 2 \n");
        printf("%6.2f \n", scalar);
        printf("\n");

        mat1 += scalar;

        printf("mat1 += 2.0 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 25:
      {
        printf("Test 25: mat1 -= scalar \n");
    
        int    idx(0);
        double scalar(2.0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("scalar = 2 \n");
        printf("%6.2f \n", scalar);
        printf("\n");

        mat1 -= scalar;

        printf("mat1 -= 2.0 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 26:
      {
        printf("Test 26: mat1 *= scalar \n");
    
        int    idx(0);
        double scalar(2.0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("scalar = 2 \n");
        printf("%6.2f \n", scalar);
        printf("\n");

        mat1 *= scalar;

        printf("mat1 *= 2.0 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      case 27:
      {
        printf("Test 27: mat1 /= scalar \n");
    
        int    idx(0);
        double scalar(2.0);

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = 2.0 * static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 2 : 22 \n");
        mat1.Print("%6.2f");
        printf("\n");

        printf("scalar = 2 \n");
        printf("%6.2f \n", scalar);
        printf("\n");

        mat1 /= scalar;

        printf("mat1 /= 2.0 \n");
        mat1.Print("%6.2f");
        printf("\n");

        break;
      }

      //
      //  + binary operator for lhs scalar
      //
      case 28:
      {
        printf("Test 28: mat2 = 2.0 + mat1 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = 2.0 + mat1;

        printf("mat2 = 2.0 + mat1 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  - binary operator for lhs scalar
      //
      case 29:
      {
        printf("Test 29: mat2 = 2.0 - mat1 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = 2.0 - mat1;

        printf("mat2 = 2.0 - mat1 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  * binary operator for lhs scalar
      //
      case 30:
      {
        printf("Test 30: mat2 = 2.0 * mat1 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 0;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 0:5 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = 2.0 * mat1;

        printf("mat2 = 2.0 * mat1 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  / binary operator for lhs scalar
      //
      case 31:
      {
        printf("Test 31: mat2 = 1.0 / mat1 \n");
    
        Matrix mat2;

        mat1.Resize(2,3);
    
        idx = 1;
        for (int row = 0; row < 2; ++row)
        {
          for (col = 0; col < 3; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(2,3) = 1:6 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = 1.0 / mat1;

        printf("mat2 = 1.0 / mat1 \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }
    
      //
      //  Dot product
      //
      case 32:
      {
        printf("Test 32: scalar = DotProduct(vec1, vec2) \n");
        printf("Also tests scalar = vec1.DotProduct(vec2) \n");
    
        Matrix vec1(3,1, 2.0);
        Matrix vec2(1,3, 3.0);
        double ans;
    
        for (int row = 0; row < 3; ++row)
        {
          vec1(row) = static_cast<double>(row);
        }
    
        printf("vec1(3,1) = 0 : 2) \n");
        vec1.Print("%6.2f");
        printf("\n");

        for (int col = 0; col < 3; ++col)
        {
          vec2(col) = 2.0 * static_cast<double>(col);
        }
    
        printf("vec2(1,3) = 0 : 2 : 4) \n");
        vec2.Print("%6.2f");
        printf("\n");
    
        ans = DotProduct(vec1, vec2);

        printf("ans = DotProduct(vec1, vec2) \n");
        printf("%6.2f \n", ans);

        break;
      }

      case 33:
      {
        printf("Test 33: mat2 = Transpose(mat1) \n");
        printf("Also tests mat2 = mat1.Transpose() \n");
    
        int    idx(0);
        Matrix mat2;

        mat1.Resize(3,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,4) = 0 : 11 \n");
        mat1.Print("%6.2f");
        printf("\n");

        mat2 = Transpose(mat1);

        printf("mat2 = Transpose(mat1) \n");
        mat2.Print("%6.2f");
        printf("\n");

        break;
      }

      case 34:
      {
        printf("Test 34: IsSymmetric(mat1) \n");
        printf("Also tests mat1.IsSymmetric() \n");
    
        int    idx(0);
        Matrix mat2;
        bool   test(false);

        mat1.Resize(4,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < row; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            mat1(col,row) = mat1(row,col);
            ++idx;
          }
        }
    
        printf("mat1(4,4) \n");
        mat1.Print("%6.2f");
        printf("\n");

        test = IsSymmetric(mat1);

        printf("test = IsSymetric(mat1) \n");
        printf("%6s \n", test ? "true" : "false");
        printf("\n");

        break;
      }

      case 35:
      {
        printf("Test 35: IsSymmetric(mat1) \n");
        printf("Also tests mat1.IsSymmetric() \n");
    
        int    idx(0);
        Matrix mat2;
        bool   test(false);

        mat1.Resize(4,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < row; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            mat1(col,row) = mat1(row,col);
            ++idx;
          }
        }
        mat1[3][2] = 4.99;
    
        printf("mat1(4,4) \n");
        mat1.Print("%6.2f");
        printf("\n");

        test = IsSymmetric(mat1);

        printf("test = IsSymetric(mat1) \n");
        printf("%6s \n", test ? "true" : "false");
        printf("\n");

        break;
      }

      case 36:
      {
        printf("Test 36: IsSymmetric(mat1) \n");
        printf("Also tests mat1.IsSymmetric() \n");
    
        int    idx(0);
        Matrix mat2;
        bool   test(false);

        mat1.Resize(4,4);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < row; ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            mat1(col,row) = mat1(row,col);
            ++idx;
          }
        }
        mat1[0][1] = 0.01;
    
        printf("mat1(4,4) \n");
        mat1.Print("%6.2f");
        printf("\n");

        test = IsSymmetric(mat1);

        printf("test = IsSymetric(mat1) \n");
        printf("%6s \n", test ? "true" : "false");
        printf("\n");

        break;
      }

      //
      // Matrix determinant
      //
      case 37:
      {
        printf("Test 37: mat1.Det() \n");
        printf("Also tests mat1.Minor(row, col) \n");
    
        int    idx(0);
        double test;

        mat1.Resize(3,3);

        for (int row = 0; row < mat1.NumRows(); ++row)
        {
          for (int col = 0; col < mat1.NumCols(); ++col)
          {
            mat1(row,col) = static_cast<double>(idx);
            ++idx;
          }
        }
    
        printf("mat1(3,3) = 0 : 8 \n");
        mat1.Print("%6.2f");
        printf("\n");

        test = mat1.Det();

        printf("test = mat1.Det() = 0.0? \n");
        printf("%6.2f \n", test);
        printf("\n");

        break;
      }

      //
      // Matrix determinant
      //
      case 38:
      {
        printf("Test 38: mat1.Det() \n");
        printf("Also tests mat1.Minor(row, col) \n");
    
        int    idx(0);
        double test;

        mat1.Resize(3,3);

        mat1(0,0) = 0.841210;
        mat1(0,1) = 0.842947;
        mat1(0,2) = 0.779456;

        mat1(1,0) = 0.360883;
        mat1(1,1) = 0.046654;
        mat1(1,2) = 0.088181;

        mat1(2,0) = 0.602632;
        mat1(2,1) = 0.522139;
        mat1(2,2) = 0.433142;
    
        printf("mat1 = rand(3,3) \n");
        mat1.Print("%10.6f");
        printf("\n");

        test = mat1.Det();

        printf("test = mat1.Det() = 0.016257? \n");
        printf("%10.6f \n", test);
        printf("\n");

        break;
      }

      //
      // Cholesky square root factorization 
      //
      case 39:
      {
        printf("Test 39: mat1.Chol_LLt() \n");
    
        Matrix Lmat;
        bool posDef(false);

        mat1.Resize(3,3);

        mat1(0,0) = 25.0;
        mat1(0,1) = 15.0;
        mat1(0,2) = -5.0;

        mat1(1,0) = 15.0;
        mat1(1,1) = 18.0;
        mat1(1,2) = 0.0;

        mat1(2,0) = -5.0;
        mat1(2,1) = 0.0;
        mat1(2,2) = 11.0;
    
        printf("mat1 =  \n");
        mat1.Print("%10.6f");
        printf("\n");

        Lmat = mat1.Chol_LLt(posDef);

        printf("Lmat = mat1.Det(posDef) = [5, 0, 0; 3, 3, 0; -1, 1, 3]? \n");
        Lmat.Print("%10.6f");
        if (posDef)
        {
          printf("%s \n", "posDef = true \n");
        }
        else
        {
          printf("%s \n", "posDef = false \n");
        }
        printf("\n");

        break;
      }

      //
      // Cholesky square root factorization 
      //
      case 40:
      {
        printf("Test 40: mat1.Chol_UtU() \n");
    
        Matrix Lmat;
        bool posDef(false);

        mat1.Resize(3,3);

        mat1(0,0) = 25.0;
        mat1(0,1) = 15.0;
        mat1(0,2) = -5.0;

        mat1(1,0) = 15.0;
        mat1(1,1) = 18.0;
        mat1(1,2) = 0.0;

        mat1(2,0) = -5.0;
        mat1(2,1) = 0.0;
        mat1(2,2) = 11.0;
    
        printf("mat1 =  \n");
        mat1.Print("%10.6f");
        printf("\n");

        Lmat = mat1.Chol_UtU(posDef);

        printf("Lmat = mat1.Chol_UtU(posDef) = [5, 3, -1; 0, 3, 1; 0, 0, 3]? \n");
        Lmat.Print("%10.6f");
        if (posDef)
        {
          printf("%s \n", "posDef = true \n");
        }
        else
        {
          printf("%s \n", "posDef = false \n");
        }
        printf("\n");

        break;
      }

      //
      // Cholesky square root factorization 
      //
      case 41:
      {
        printf("Test 41: mat1.Chol_UtU() \n");
    
        Matrix Lmat;
        bool posDef(false);

        mat1.Resize(4,4);

        mat1(0,0) = 1.0;
        mat1(0,1) = 2.0;
        mat1(0,2) = 3.0;
        mat1(0,3) = 4.0;

        mat1(1,0) = 2.0;
        mat1(1,1) = 29.0;
        mat1(1,2) = 36.0;
        mat1(1,3) = 43.0;

        mat1(2,0) = 3.0;
        mat1(2,1) = 36.0;
        mat1(2,2) = 109.0;
        mat1(2,3) = 126.0;
    
        mat1(3,0) = 4.0;
        mat1(3,1) = 43.0;
        mat1(3,2) = 126.0;
        mat1(3,3) = 246.0;
    
        printf("mat1 =  \n");
        mat1.Print("%10.6f");
        printf("\n");

        Lmat = mat1.Chol_UtU(posDef);

        printf("Lmat = mat1.Chol_UtU(posDef) = 1:10? \n");
        Lmat.Print("%10.6f");
        if (posDef)
        {
          printf("%s \n", "posDef = true \n");
        }
        else
        {
          printf("%s \n", "posDef = false \n");
        }
        printf("\n");

        break;
      }

      //
      // Identity matrix
      //
      case 42:
      {
        printf("Test 42: mat1.Eye(size) \n");
    
        Matrix mat2;
    
        mat2.Eye(4);

        printf("mat2.Eye(4) \n");
        mat2.Print("%5.2f");
        printf("\n");

        break;
      }

      //
      // Identity matrix
      //
      case 43:
      {
        printf("Test 42: mat1.Eye() \n");
    
        Matrix mat2;
    
        mat1.Resize(3,3);

        printf("mat1 =  \n");
        mat1.Print("%5.2f");
        printf("\n");

        mat1.Eye();

        printf("mat1.Eye() \n");
        mat1.Print("%5.2f");
        printf("\n");

        break;
      }

      default:
      {
        printf("Test %d does not exist. \n", testNo);
      }
    }  // end switch (testNo)
  }  // end try
  catch(std::exception &ex)
  {
    printf("\n");
    printf("%s \n", ex.what());
    printf("\n");

    exit(1);
  }
  printf("\n");

}  // end RunTests()

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

