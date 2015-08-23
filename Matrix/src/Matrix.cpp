////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Matrix.cpp
//
// Overview:
//  Double precision matrix class.  Unaray and binary arithemtic, inverse,
//  transpose, determinate, Cholesky square root are supported.
//  Constructors:
//    Matrix mat;
//    Matrix mat(3,3);
//    Matrix mat(3,3,1.0);
//  The first form creates a default 1x1 matrix initialized to zero.
//  The first form creates a 3x3 matrix initialized to zero.
//  The third form overides the default initialization to zero. 
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "Matrix.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
using std::string;


#include <cmath>
#include <cstdio>

//#define debugEn

//
// Default constructors
//
Matrix::Matrix(int    _rows, 
               int    _cols,
               double initVal)
{
  Construct(_rows, _cols, initVal);
}

//
// Copy constructor
//
Matrix::Matrix(const Matrix &mat)
{
  double *ptr1(mat.pData);
  double *ptr2(NULL);
  int _rows(mat.rows);
  int _cols(mat.cols);

  Construct(_rows, _cols);
  ptr2 = pData;

  for (int idx = 0; idx < nElem; ++idx)
  {
    *ptr2++ = *ptr1++;
  }
}

//
// Create a new Matrix and initialize all elements.
//
void Matrix::Construct(int    _rows,
                       int    _cols,
                       double initVal)
{
  rows = _rows;
  cols = _cols;
  nElem = rows * cols;

  pData = new double[nElem];

  if (pData == NULL)
  {
    throw(MEMERR());
  }

  Set(initVal);
}

//
// Create a new Matrix but don't initialize elements.
//
void Matrix::Construct(int _rows,
                       int _cols)
{
  rows = _rows;
  cols = _cols;
  nElem = rows * cols;

  pData = new double[nElem];

  if (pData == NULL)
  {
    throw(MEMERR());
  }
}

//
// Destructor
//
Matrix::~Matrix()
{
  delete[] pData;
  pData = NULL;
}

//
// Resize the matrix and initialize its elements.
//
void Matrix::Resize(int _rows, int _cols, double initVal)
{
  if (pData != NULL)
  {
    delete [] pData;
  }

  rows = _rows;
  cols = _cols;
  nElem = _rows * _cols;
  {
    Construct(rows, cols, initVal);
  }
}

//
// Set all the elements to val.
//
void Matrix::Set(double val)
{
  for (int idx = 0; idx < nElem; ++idx)
  {
    pData[idx] = val;
  }
}

//
// Check that the row and col indices are in bounds of the Matrix
//
bool Matrix::CheckDim(int row, int col)
{
  bool pass(false);

  if ((row >= 0) &&
      (row < rows) &&
      (col >=0 ) &&
      (col < cols))
  {
    pass = true;
  }

  return pass;
}

//
// Check that the vector index is in bounds and the Matrix is a 1d vector,
// either row or column.
//
bool Matrix::CheckDim(int idx)
{
  bool pass(false);

  if ((idx >= 0) &&
      (idx < nElem) &&
      ((rows == 1 ) ||
      (cols == 1)))
  {
    pass = true;
  }

  return pass;
}

//
// Get a pointer to an element.
//
double* Matrix::GetPtr(int row, int col)
{
  bool pass = CheckDim(row, col);
  double *ptr(NULL);

  if (pass)
  {
    ptr = pData + row * cols + col;
  }
  else
  {
    throw(DIMERR());
  }

  return ptr;
}

//
// Get a pointer to an element.
//
double* Matrix::GetPtr(int idx)
{
  bool pass = CheckDim(idx);
  double *ptr(NULL);

  if (pass)
  {
    ptr = pData + idx;
  }
  else
  {
    throw(DIMERR());
  }

  return ptr;
}

//
// Print the elements of the matrix
//
void Matrix::Print(const char *pFormat)
{
  string format(pFormat);
  Print(format);
}

//
// Print the elements of the matrix
//
void Matrix::Print(char *pFormat)
{
  string format(pFormat);
  Print(format);
}

//
// Print the elements of the matrix
//
void Matrix::Print(string format)
{
  double *pElem(pData);

  if (rows > 1)
  {
    for (int row = 0; row < rows; ++row)
    {
      for (int col = 0; col < cols; ++col)
      {
        printf(format.c_str(), *pElem);
        ++pElem;
      }
      printf("\n");
    }
  }
  else
  {
    for (int col = 0; col < cols; ++col)
    {
      printf(format.c_str(), *pElem);
      ++pElem;
    }
    printf("\n");
  }
}

//
// Return a row as a new matrix
//
Matrix Matrix::GetRow(int row)
{
  if ((row < 0) ||
      (row >= rows))
  {
    throw(DIMERR());
  }

  Matrix result(1, cols);
  double *ptr1(pData + row * cols);
  double *ptr2(result.pData);

  for (int col = 0; col < cols; ++col)
  {
    *ptr2++ = *ptr1++;
  }
  
  return result;
}

//
// Return a column as a new matrix
//
Matrix Matrix::GetCol(int col)
{
  if ((col < 0) ||
      (col >= cols))
  {
    throw(DIMERR());
  }

  Matrix result(rows, 1);
  double *ptr1(pData + col);
  double *ptr2(result.pData);

  for (int row = 0; row < rows; ++row)
  {
    *ptr2++ = *ptr1;
    ptr1 += cols;
  }
  
  return result;
}

//
// Operator to access an element of a 1d matrix
//
double& Matrix::operator()(int idx)
{
  bool pass = CheckDim(idx);

  if (!pass)
  {
    throw(DIMERR());
  }

  double &rVal(*GetPtr(idx));

  return rVal;
}

//
// Operator to access an element of a 2d matrix
//
double& Matrix::operator()(int row, int col)
{
  bool pass = CheckDim(row, col);

  if (!pass)
  {
    throw(DIMERR());
  }

  double &rVal(*GetPtr(row, col));

  return rVal;
}

//
// Operator to access an element of a 2d matrix using C [][] brackets.  Returns
// a pointer to the first element in a row.
//
double* Matrix::operator[](int row)
{
  if ((row < 0) || (row > rows - 1))
  {
    throw(DIMERR());
  }

  double *pRow(pData + row * cols);

  return pRow;
}

//
// Assignment operator
//
//Matrix& Matrix::operator= (const Matrix& mat)  // Didn't work
//Matrix& Matrix::operator= (Matrix& mat)        // Didn't work
Matrix Matrix::operator= (Matrix mat)
{
  double *ptr1 = mat.pData;
  double *ptr2(NULL);
  int    _rows = mat.NumRows();
  int    _cols = mat.NumCols();

  Construct(_rows, _cols);
  ptr2 = pData;

  for (int idx = 0; idx < nElem; ++idx)
  {
    *ptr2++ = *ptr1++;
  }
  return *this;
}

//
// Scalar ssignment operator                        // Didn't work
//
// Matrix Matrix::operator= (const double scalar)
// {
//   Matrix result(rows, cols, scalar);
//
//   return result;
// }

//
// Scalar assignment operator
//
Matrix Matrix::operator= (double scalar)
{
  Construct(rows, cols, scalar);

  return *this;
}

//
// Binary addition
//
Matrix Matrix::operator+ (const Matrix rhs) const
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  Matrix result(rows, cols);

  for (int idx = 0; idx < nElem; ++idx)
  {
    result.pData[idx] = pData[idx] + rhs.pData[idx];
  }
  return result;
}

//
// Binary subtraction
//
Matrix Matrix::operator- (const Matrix rhs) const
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  Matrix result(rows, cols);

  for (int idx = 0; idx < nElem; ++idx)
  {
    result.pData[idx] = pData[idx] - rhs.pData[idx];
  }
  return result;
}

//
// Binary element X element division
//
Matrix Matrix::operator/ (const Matrix rhs) const
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  Matrix result(rows, cols);

  for (int idx = 0; idx < nElem; ++idx)
  {
    if (rhs.pData[idx] == 0.0)
    {
      throw(DIVBYZERO());
    }

    result.pData[idx] = pData[idx] / rhs.pData[idx];
  }
  return result;
}

//
// Binary matrix multiplication
//
Matrix Matrix::operator* (const Matrix rhs) const
{
  if (cols != rhs.rows)
  {
    throw(DIMERR()); 
  }

  Matrix result(rows, rhs.cols);

  double *pResElem(result.pData);
  double *pRhsCol(rhs.pData);
  double *pLhsRow(pData);

  for (int lhsRow = 0; lhsRow < rows; ++lhsRow)
  {
    for (int rhsCol = 0; rhsCol < rhs.cols; ++rhsCol)
    {
      pLhsRow = pData + lhsRow * cols;
      pRhsCol = rhs.pData + rhsCol;
      for (int resIdx = 0; resIdx < cols; ++resIdx)
      {
        *pResElem += *pLhsRow++ * *pRhsCol;
        pRhsCol += rhs.cols; 
      }
      ++pResElem;
    }
  }
  return result;
}

//
// Binary matrix addition by a rhs scalar
//
Matrix Matrix::operator+ (const double rhs) const
{
  Matrix result(rows, cols);

  double *pResElem(result.pData);

  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pResElem++ = *pElem++ + rhs;
  }

  return result;
}

//
// Binary matrix subtraction by a rhs scalar
//
Matrix Matrix::operator- (const double rhs) const
{
  Matrix result(rows, cols);

  double *pResElem(result.pData);

  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pResElem++ = *pElem++ - rhs;
  }

  return result;
}

Matrix Matrix::operator* (const double rhs) const
{
  Matrix result(rows, cols);

  double *pResElem(result.pData);

  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pResElem++ = *pElem++ * rhs;
  }

  return result;
}

//
// Binary matrix division by a scalar
//
Matrix Matrix::operator/ (const double rhs) const
{
  if (rhs == 0.0)
  {
    throw(DIVBYZERO());
  }

  Matrix result(rows, cols);

  double *pResElem(result.pData);

  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pResElem++ = *pElem++ / rhs;
  }

  return result;
}

//
// Incremental binary matrix addition
//
void Matrix::operator+= (const Matrix rhs)
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  double *pElem(pData);
  double *pRhsElem(rhs.pData); 

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ += *pRhsElem++;
  }
}

//
// Incremental binary matrix subtraction
//
void Matrix::operator-= (const Matrix rhs)
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  double *pElem(pData);
  double *pRhsElem(rhs.pData); 

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ -= *pRhsElem++;
  }
}

//
// Incremental binary matrix element X emelment multiplication
//
void Matrix::operator*= (const Matrix rhs)
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  double *pElem(pData);
  double *pRhsElem(rhs.pData); 
    if (*pRhsElem == 0.0)
    {
      throw(DIVBYZERO());
    }

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ *= *pRhsElem++;
  }
}

//
// Incremental binary matrix element X emelment division
//
void Matrix::operator/= (const Matrix rhs)
{
  if ((rows != rhs.rows) ||
      (cols != rhs.cols))
  {
    throw(DIMERR()); 
  }

  double *pElem(pData);
  double *pRhsElem(rhs.pData); 

  for (int idx = 0; idx < nElem; ++idx)
  {
    if (*pRhsElem == 0.0)
    {
      throw(DIVBYZERO());
    }
    *pElem++ /= *pRhsElem++;
  }
}

//
// Incremental binary matrix addition by a scalar
//
void Matrix::operator+= (const double rhs)
{
  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ += rhs;
  }
}

//
// Incremental binary matrix subtraction by a scalar
//
void Matrix::operator-= (const double rhs)
{
  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ -= rhs;
  }
}

//
// Incremental binary matrix multiplication by a scalar
//
void Matrix::operator*= (const double rhs)
{
  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ *= rhs;
  }
}

//
// Incremental binary matrix division by a scalar
//
void Matrix::operator/= (const double rhs)
{
  if (rhs == 0.0)
  {
    throw(DIVBYZERO());
  }

  double *pElem(pData);

  for (int idx = 0; idx < nElem; ++idx)
  {
    *pElem++ /= rhs;
  }
}

//
//  Return the inverse of the matrix.  Uses Gaussian elimination with 
//  scaled pivoting.  More robust than simple Gaussian elimination or LU
//  decomposition.
//
//  ref:  Cheney, W. and Kincaid, D. "Numerical Mathemeatics and Computing,
//        4th ed.", Brooks/Cole Publishing Co., pp 240-293
//
Matrix Matrix::Inv()
{
  //
  // Check to make sure the matrix is square.
  //
  if (rows != cols)
  {
    throw(DIMERR());
  }

  int    col(0);
  double factor(0.0);        // xmult
  int    identElem(0);       //
  int    pivotRow(0);        // lSubk
  int    pivotRowTrial(0);   //
  double *pRhs(NULL);        //
  double *pRowMaxes(NULL);   // sSubi
  int    *pRowOrder(NULL);   // l
  double ratio(0.0);         // r
  double ratioMax(0.0);      // rmax
  int    row(0);             // i1
  double rowMax(0.0);        //
  double sum(0.0);           //
  int    temp(0);            //
  Matrix result(rows, cols); //
  int    rowIdx;             //
  int    rowRatioMax;        // 
  int    rowMaxIdx;          //
  int    scaleRow;           // lSubi
  int    zeroColIdx(0);      // k

  #ifdef debugEn
  FILE*fp = fopen("Debug.txt", "w");
  #endif
  //
  // Forward elimination with scaled pivoting.  This algorithm calculates an
  // upper triangular matrix of coefficients called upTriang.  The resulting
  // system is:
  //
  //   upTriang * x = b'
  //
  // The upper triangular system is easily solved for x by back substitution,
  // starting at the last row.
  //
  // The solution to the upper triangular system x also solves the original 
  // linear system:
  //
  //   Ax = b
  //
  // Later, sucessive columns of the identity matrix will be substituted for
  // b, resulting in successive columns of the inv(A) matrix.
  //
  //   A*inv(A) = I
  //
  // Gaussian elimination takes one equation of a system of linear equations
  // as a pivot row.  For each of the remaining rows a scaled version of the 
  // pivot row is subtracted, such that one element of the selected row is 
  // zeroed.
  //
  // Then a new pivot row is selected and the process is repeated eliminating
  // a new element from the remaining rows.
  //
  // In order to reduce the chance for errors due to finite arithmetic, the
  // sequence of pivot rows is selected such that the initial scale ratio is
  // largest.  Successive pivot rows are selected in order of decreasing 
  // scale ratio.  Rather than reorder the system of equitions, a vector is 
  // created to maintain the order in which the pivot rows are selected. 
  // The order in which the pivot rows are selected is stored in the rowOrder
  // vector.
  //
  pRowOrder = new int[rows];

  //
  // The abs(maximum) of each row is stored in pRowMaxws vector.
  //
  pRowMaxes = new double[rows];

  //
  // Create a copy of the matrix.  The algorithm will convert the new matrix
  // into upper triangular form as it proceeds.
  //
  Matrix upTriang(*this);

  //
  // Find the largest coefficient in each row.
  //
  for (row = 0; row < rows; ++row)
  {
    //
    // Initialize the pivot row order as the initial matrix row order.  This
    // order will be modified as the algorithm progresses.
    //
    pRowOrder[row] = row;

    rowMax = 0.0;
    for (col = 0; col < cols; ++col)
    {
      rowMax = Max(rowMax, fabs(upTriang[row][col]));
    }
    pRowMaxes[row] = rowMax;
  }

  #ifdef debugEn
  fprintf(fp, "p05 pRowMaxes = %7.2f %7.2f %7.2f %7.2f \n", 
               pRowMaxes[0], pRowMaxes[1], pRowMaxes[2], pRowMaxes[3]);
  fprintf(fp, "pRowOrder = %d %d %d %d \n", pRowOrder[0], pRowOrder[1],
                                    pRowOrder[2], pRowOrder[3]);
  #endif

  //
  // Step through the system along the diagonal, selecting the appropriate pivot
  // element for each column.
  //
  for (zeroColIdx = 0; zeroColIdx < cols - 1; ++zeroColIdx)
  {
    #ifdef debugEn
    fprintf(fp, "p10 zeroColIdx (k) = %d \n", zeroColIdx);
    #endif

    //
    // Find the row with the largest scale ratio for each column.  This will be
    // the pivot row for that column.
    //
    ratioMax = 0.0;

    for (rowIdx = zeroColIdx; rowIdx < rows; ++rowIdx)
    {
      //
      // See if the next row should be the pivot row for this pivot index by
      // selecting the highest scale ratio.
      //
      pivotRowTrial = pRowOrder[rowIdx];

      //
      // The denomiator in the ratio calculation below should never be zero
      // since it was chosen to be the max relative to the row.
      //
      if (pRowMaxes[pivotRowTrial] == 0.0)
      {
        throw(SINGERR());
      }

      ratio = fabs(upTriang[pivotRowTrial][zeroColIdx] / 
                                                  pRowMaxes[pivotRowTrial]);

      if (ratio > ratioMax)
      {
        ratioMax = ratio;

        rowRatioMax = rowIdx;
      }
    } // end for (rowIdx = zeroColIdx; rowIdx < rows; ++rowIdx)

    #ifdef debugEn
    fprintf(fp, "p20 ratioMax = %7.2f \n", ratioMax);
    fprintf(fp, "p30 rowRatioMax (j) = %d \n", rowRatioMax); 
    #endif

    //
    // Sort the pivot row list by interchanging the newly found pivot row with
    // the current row pointed to by the pivot index.
    //
    temp = pRowOrder[zeroColIdx];
    pRowOrder[zeroColIdx] = pRowOrder[rowRatioMax];
    pRowOrder[rowRatioMax] = temp;
    
    #ifdef debugEn
    fprintf(fp, "p40: lSubj <--> lSubk \n");
    fprintf(fp, "p50: pRowOrder = %d %d %d %d \n", pRowOrder[0], pRowOrder[1],
                                    pRowOrder[2], pRowOrder[3]);
    #endif

    pivotRow = pRowOrder[zeroColIdx];
    
    #ifdef debugEn
    fprintf(fp, "p30 pivotRow (j) = %d \n", pivotRow); 
    #endif

    for (rowIdx = zeroColIdx + 1; rowIdx < rows; ++rowIdx)
    {
      scaleRow = pRowOrder[rowIdx];
      
      #ifdef debugEn
      fprintf(fp, "p70 scaleRow = row being scaled %d \n", scaleRow);
      #endif
  
      //
      // The denomenator in the factor calculation should never be zero since it
      // was chosen to be the max relative to the row max.
      //
      if (upTriang[pivotRow][zeroColIdx] == 0.0)
      {
        throw(SINGERR());
      }
      factor = upTriang[scaleRow][zeroColIdx] / upTriang[pivotRow][zeroColIdx];
    
      //
      // Store the scaling factor in the upper triangular matrix for use later.
      // This makes the book keeping easy since it can be accessed using the 
      // row and zeroColIdx indeces.  Of course the matrix is no linger strictly 
      // upper traingular.
      //
      upTriang[scaleRow][zeroColIdx] = factor;
    
      //
      // Zero the elements in the pivot column in the remaining rows to create
      // the upper triangular matrix.
      //
      for (col = zeroColIdx + 1; col < cols; ++col)
      {
        upTriang[scaleRow][col] -= factor * upTriang[pivotRow][col];
    
      }

      #ifdef debugEn
      fprintf(fp, "p80 row = %d; factor = %7.2f;", scaleRow, factor);
      fprintf(fp, " upTriang(%d,:) = %7.2f %7.2f %7.2f %7.2f \n", row,
                  upTriang[scaleRow][0], upTriang[scaleRow][1], 
                  upTriang[scaleRow][2], upTriang[scaleRow][3]);
      #endif
    
    } // end for (rowIdx = zeroColIdx + 1; rowIdx < rows; ++rowIdx)
  }  // for (zeroColIdx = 0; zeroColIdx < cols - 1; ++zeroColIdx)

  #ifdef debugEn
  fprintf(fp, "p90 upTriang = \n");
  for (int dbRow = 0; dbRow < 4; ++dbRow)
  {
    for (int dbCol = 0; dbCol < 4; ++dbCol)
    {
      fprintf(fp, "%7.2f ", upTriang[dbRow][dbCol]); 
    }
    fprintf(fp, "\n");
  }
  #endif

  //
  // We now have an upper triangular representation of A in 
  //  A * X = b
  // But we have not operated on b.  Furthermore, what we want is inv(A) as in
  //  A * inv(A) = I
  // We'll brek this problem up into columns of inv(A) and I
  //  A * inv(A)(i) = I(i)
  // So we need to apply forward elimination processing to the columns of I.
  // Then proceed to back substitution
  //
  // The modified right hand side system of equations is stored in pRhs.
  //
  //   A * X = b (rhs)
  //
  pRhs = new double[rows];

  for (identElem = 0; identElem < rows; ++identElem)
  {
    //
    // Extract the appropriate column of the identify matrix for use as 
    // the right hand side of the system of equations.
    //
    for (row = 0; row < rows; ++row)
    {
      pRhs[row] = 0.0;
    }
    pRhs[identElem] = 1.0;

    //
    // Apply the same scaling used on the left hand side of the system
    // to the right hand side.
    //
    for (zeroColIdx = 0; zeroColIdx < rows-1; ++zeroColIdx)
    {
      pivotRow = pRowOrder[zeroColIdx];
      
      for (rowIdx = zeroColIdx + 1; rowIdx < rows; ++rowIdx)
      {
        row = pRowOrder[rowIdx];

        factor = upTriang[row][zeroColIdx];

        pRhs[row] -= factor * pRhs[pivotRow];
      }
    }

    //
    // Back substitution starting a the bottome of the triangular matrix.
    //
    row = pRowOrder[rows - 1];

    if (upTriang[row][rows-1] == 0.0)
    {
      throw(SINGERR());
    }

    result[rows-1][identElem] = pRhs[row] / upTriang[row][rows-1];

    for (rowIdx = rows-2; rowIdx >= 0; --rowIdx)
    {
      row = pRowOrder[rowIdx];
      sum = pRhs[row];
      
      for (col = rowIdx + 1; col < cols; ++col)
      {
        sum = sum - upTriang[row][col] * result[col][identElem];
      }
     
      if (upTriang[row][rowIdx] == 0.0)
      {
      throw(SINGERR());
      }
      result[rowIdx][identElem] = sum / upTriang[row][rowIdx];
    }
  }  // end for (identElem = 0; identElem < rows; ++identElem)

  delete [] pRhs;
  delete [] pRowOrder;
  delete [] pRowMaxes;

  #ifdef debugEn
  fclose(fp);
  #endif

  return(result);
}

//
// DotProduct method.
//
double Matrix::DotProduct(Matrix arg)
{
  double result(0.0);

  if ((arg.nElem != nElem) &&
      ((arg.rows != 1) || (arg.cols != 1)) &&
      ((rows != 1) || (cols != 1)))
  {
    throw (DIMERR());
  }

  for (int idx = 0; idx < nElem; ++idx)
  {
    result += pData[idx] * arg.pData[idx];
  }

  return result;
}

//
// Transpose method
//
Matrix Matrix::Transpose()
{
  Matrix result(cols, rows);

  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      result[col][row] = (*this)[row][col];
    }
  }
  return result;
}

bool Matrix::IsSymmetric(double tol)
{
  int    col(0);
  bool   done(false);
  double eps(0.0);
  bool   result(true);
  int    row(0);

  std::numeric_limits<double> lim;

  eps = lim.min();

  if (tol == 0.0)
  {
    //
    // The default value of tol is 0.0, so no tolerance was provided.  Use
    // machine epsilon.
    //
    tol = eps;
  }

  if (rows != cols)
  {
    result = false;
  }
  else
  {
    while (!done)
    {
      if (row >= rows)
      {
        done = true;
      }

      while ((col <= row) && !done)
      {
        if (!WithinTolerance((*this)[row][col], (*this)[col][row], tol))
        {
          result = false;
          done = true;
        }
        ++col;
      }
      col = 0;
      ++row;
    }
  }
  return result;
}

//
// Check whether a matrix is positive definite
//
bool Matrix::IsPosDef()
{
  bool result;

  //
  // A matrix is positive definite if all leading principal minors are positive.
  //


  return result;
}

//
// Calculate the determinate of the matrix. The row with the most zero elements
// is selected.  Then the minor, Mij, for each element aij of tha row is
// calculated.  The cofactor of aij is Aij = (-1)^(i+j) * Mij.  The deternimte
// is the sum across the selected row of aij * Aij
//
// Ref:  J.H. Mathews, K.D. Fink, "Numerical Methods Using MATLAB", 3rd ed.,
//       Prentice Hall, 1999, p. 113
//
double Matrix::Det()
{
  if (rows != cols)
  {
    throw(DIMERR());
  }

  double cofactor;
  int    col(0);
  bool   done(false);
  int    maxZeroRow(0);
  double minor(0.0);
  int    prevZeroCnt(0);
  double result(0.0);
  int    row(0);
  double sign(0.0);
  int    zeroCnt(0);

  //
  // Handle trivial cases.
  //
  if (rows == 1)
  {
    result = *pData;
    done = true;
  }

  if (!done)
  {
    if (rows == 2)
    {
      result = (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
      done = true;
    }
  }

  if (!done)
  {
    //
    // Find the row with the most zero elements.
    //
    for (row = 0; row < rows; ++row)
    {
      zeroCnt = 0.0;
      for (col = 0; col < cols; ++col)
      {
        if (WithinTolerance((*this)[row][col],  0.0))
        {
          ++zeroCnt;
        }
      }
      if (zeroCnt == cols)
      {
        result = 0.0;
        done = true;
        break;  // exit loop for (row = 0; row < rows; ++row)
      }
      else if (zeroCnt > prevZeroCnt)
      {
        maxZeroRow = row;
        prevZeroCnt = zeroCnt;
      }
    }
    
    row = maxZeroRow;

  } // end if (!done)

  //
  // Sum the products of the selected row elements and their cofactors.
  //
  if (!done)
  {  
    for (col = 0; col < cols; ++col)
    {
      if (!WithinTolerance(pData[row*cols+col], 0.0))
      { 
        //
        // Odd powers of -1 are negative 1, even powers are positive 1.
        //
        sign = (row + col) % 2 == 0 ? 1.0 : -1.0;

        minor = Minor(row, col);
    
        cofactor = sign * pData[row*cols+col] * minor;
    
        result += cofactor;
      }
    }
  }  // end if (!done)

  return result;
}

//
// Calculate the minor of the matrix at row i, and col j.  The minor of a matrix
// is the determinate of the submatrix matrix formed by deleting the ith row
// and jth column of the parent matrix.
//
double Matrix::Minor(double row_, double col_)
{
  int    minCol(0);
  Matrix minMat(rows-1, cols-1);
  double result(0);
  int    minRow(0);

  for (int row = 0; row < rows; ++row)
  {
    if (row != row_)
    {
      for (int col = 0; col < cols; ++col)
      {
        if  (col != col_)
        {
          minMat[minRow][minCol] = (*this)[row][col];
          ++minCol;
        }
      }
      ++minRow;
      minCol = 0;
    }
  }

  result = minMat.Det();

  return result;
}

//
// Cholesky square root factorization.  The upper triangular matrix U is
// calculated, such that A = U'U.
//
// Diagonal terms:
//
// u00 = sqrt(a00)
// u11 = sqrt(a11 - u01^2)
// u22 = sqrt(a22 - (u02^2 + u12^2))
//
//                   k
// ukk = sqrt(akk - sum( uik^2 )  
//                   j=0
//
// Terms above the diagonal:
//
// u01 = a10 / u00
// u02 = a02 / u00
// u12 = (a12 - u01*u02)/u11
//
//              k
// uik = (aik - sum( uij*uik ) / uii 
//              j=0
//
Matrix Matrix::Chol_UtU(bool &posDef)
{
  if (rows != cols)
  {
    throw(DIMERR());
  }

  Matrix Amat = *this; 
  int    col;
  int    idx;
  Matrix Lmat(Amat.NumRows(), Amat.NumCols());
  int    row;
  double sum;
  double test;

  posDef = Amat.IsSymmetric();

  if (posDef)
  {
    for (row = 0; row < rows; ++row)
    {
      for (col = row; col < cols; ++col)
      {
        sum = 0.0;
        for (idx = 0; idx < cols-1; ++idx)
        {
          sum += Lmat[idx][row] * Lmat[idx][col];
        }

        if (row == col)
        {
          test = Amat[row][col] - sum;
          if (test > 0.0)
          {
            Lmat[row][col] = sqrt(test);
          }
          else
          {
            posDef = false;
            Lmat = 0.0;
            return Lmat;
          }
        }
        else
        {
          Lmat[row][col] = (Amat[row][col] - sum) / Lmat[row][row];
        }
      }  // end for (col = 0; col < row+1; ++col)
    }  // end for (row = 0; row < rows; ++row)
  }  // end if (posDef)

  return Lmat;
}

//
// Cholesky square root factorization.  The lower triangular matrix C is
// calculated, such that A = CC'.
//
// Diagonal terms:
//
// c00 = sqrt(a00)
// c11 = sqrt(a11 - c10^2)
// c22 = sqrt(a22 - (c20^2 + c21^2))
//
//                   k
// ckk = sqrt(akk - sum( ckj^2 )  
//                   j=0
//
// Terms below the diagonal:
//
// c10 = a10 / c00
// c20 = a20 / c00
// c21 = (a21 - c20*c10)/c11
//
//              k
// cik = (aik - sum( cij*ckj ) 
//              j=0
//
Matrix Matrix::Chol_LLt(bool &posDef)
{
  if (rows != cols)
  {
    throw(DIMERR());
  }

  Matrix Amat = *this; 
  int    col;
  int    idx;
  Matrix Lmat(Amat.NumRows(), Amat.NumCols());
  int    row;
  double sum;
  double test;

  posDef = Amat.IsSymmetric();

  if (posDef)
  {
    for (row = 0; row < rows; ++row)
    {
      for (col = 0; col < row+1; ++col)
      {
        sum = 0.0;
        for (idx = 0; idx < col; ++idx)
        {
          if (row != 0)
          {
            sum += Lmat[row][idx] * Lmat[col][idx];
          }
        }

        if (row == col)
        {
          test = sqrt(Amat[row][col] - sum);
          if (test >= 0.0)
          {
            Lmat[row][col] = test;
          }
          else
          {
            posDef = false;
            Lmat = 0.0;
            return Lmat;
          }
        }
        else
        {
          Lmat[row][col] = (Amat[row][col] - sum) / Lmat[col][col];
        }
      }  // end for (col = 0; col < row+1; ++col)
    }  // end for (row = 0; row < rows; ++row)
  }  // end if (posDef)

  return Lmat;
}

void Matrix::Eye(int size)
{
  int idx(0);

  Resize(size, size);

  for (idx = 0; idx < rows; ++idx)
  {
    pData[idx * cols + idx] = 1.0;
  }
}

void Matrix::Eye()
{
  int idx;

  for (idx = 0; idx < rows; ++idx)
  {
    pData[idx * rows + idx] = 1.0;
  }
}

//
// Determine whether two numbers are equal within a tolerance.
//
bool Matrix::WithinTolerance(double arg1,
                             double arg2,
                             double tol)
{
  bool test;

  if (tol == 0.0)
  {
    //
    // The default value of tol is 0.0, so no tolerance was provided.  Use
    // machine epsilon.
    //
    std::numeric_limits<double> lim;

    double eps = lim.min();

    tol = eps;
  }

  test = fabs(arg1 - arg2) <= tol;

  return test;
}

//
// Private maximum function
//
double Matrix::Max(double lhArg, double rhArg)
{
  double ans;

  if (lhArg < rhArg)
  {
    ans = rhArg;
  }
  else
  {
    ans = lhArg;
  }

  return (ans);
}

////////////////////////////////////////////////////////////////////////////////
//
// Auxillary functions
//
////////////////////////////////////////////////////////////////////////////////
namespace MATRIX
{

//
// Binary matrix addition by a lhs scalar
//
Matrix operator+ (const double lhs, Matrix rhs)
{
  Matrix result(rhs.NumRows(), rhs.NumCols());

  for (int row = 0; row < rhs.NumRows(); ++row)
  {
    for (int col = 0; col < rhs.NumCols(); ++col)
    {
      result[row][col] = lhs + rhs[row][col];
    }
  }

  return result;
}

//
// Binary matrix subtraction by a lhs scalar
//
Matrix operator- (const double lhs, Matrix rhs)
{
  Matrix result(rhs.NumRows(), rhs.NumCols());

  for (int row = 0; row < rhs.NumRows(); ++row)
  {
    for (int col = 0; col < rhs.NumCols(); ++col)
    {
      result[row][col] = lhs - rhs[row][col];
    }
  }

  return result;
}

//
// Binary matrix multiplication by a lhs scalar
//
Matrix operator* (const double lhs, Matrix rhs)
{
  Matrix result(rhs.NumRows(), rhs.NumCols());

  for (int row = 0; row < rhs.NumRows(); ++row)
  {
    for (int col = 0; col < rhs.NumCols(); ++col)
    {
      result[row][col] = lhs * rhs[row][col];
    }
  }

  return result;
}

//
// Binary matrix division by a lhs scalar
//
Matrix operator/ (const double lhs, Matrix rhs)
{
  Matrix result(rhs.NumRows(), rhs.NumCols());

  for (int row = 0; row < rhs.NumRows(); ++row)
  {
    for (int col = 0; col < rhs.NumCols(); ++col)
    {
      if (rhs[row][col] == 0.0)
      {
        throw(Matrix::DIVBYZERO());
      }
      result[row][col] = lhs / rhs[row][col];
    }
  }

  return result;
}

//
// Return the inverse of a matrix
//
Matrix Invert(Matrix &MM)
{
  Matrix result;

  result = MM.Inv();

  return result;
}

//
// Dot product function
//
double DotProduct(Matrix arg1, Matrix arg2)
{
  double result(0.0);

  result = arg1.DotProduct(arg2);

  return result;
}

//
// Transpose function
//
Matrix Transpose(Matrix arg)
{
  Matrix result;

  result = arg.Transpose();

  return result;
}

//
// Symmetry test
//
bool IsSymmetric(Matrix arg)
{
  bool result(false);

  result = arg.IsSymmetric();

  return result;
}

} // end namespace MATRIX

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
