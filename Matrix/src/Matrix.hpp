////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Matrix.hpp
//
//
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <exception>
#include <sstream>
#include <string>
#include <limits>


class Matrix
{

public:

  //
  // Default constructor
  //
  Matrix(int    _rows = 1, 
         int    _cols = 1,
         double initVal = 0.0);

  //
  // Copy constructor
  //
  Matrix(const Matrix &mat);

  //
  // Destructor
  //
  ~Matrix();

  void Print(const char *pFormat);

  void Print(char *pFormat);

  void Print(std::string format);


void Resize(int rows, int cols, double initVal = 0.0);

void Set(double val);

bool CheckDim(int row, int col);

bool CheckDim(int idx);

double *GetPtr(int row, int col);

double *GetPtr(int idx);

int NumRows() 
{
  return rows;
}

int NumCols()
{
  return cols;
}

int NumEmems()
{
  return nElem;
}

Matrix GetRow(int row);

Matrix GetCol(int col);

double & operator()(int row, int col);

double & operator()(int idx);

double* operator[](int row);

//Matrix& operator= (const Matrix& mat);
//Matrix& operator= (Matrix& mat);
Matrix operator= (Matrix mat);

Matrix operator= (double scalar);

//Matrix operator= (const double scalar);        // Didn't work

Matrix operator+ (const Matrix rhs) const;

Matrix operator- (const Matrix rhs) const;

Matrix operator/ (const Matrix rhs) const;

Matrix operator* (const Matrix rhs) const;

Matrix operator+ (const double rhs) const;

Matrix operator- (const double rhs) const;

Matrix operator* (const double rhs) const;

Matrix operator/ (const double rhs) const;

void operator+= (const Matrix rhs);

void operator-= (const Matrix rhs);

void operator*= (const Matrix rhs);

void operator/= (const Matrix rhs);

void operator+= (const double rhs);

void operator-= (const double rhs);

void operator*= (const double rhs);

void operator/= (const double rhs);

Matrix Inv();

double DotProduct(Matrix arg);

Matrix Transpose();

bool IsSymmetric(double tol = 0.0);

double Det();

double Minor(double row, double col);

bool IsPosDef();

Matrix Chol_UtU(bool &posDef);

Matrix Chol_LLt(bool &posDef);

void Eye();

void Eye(int size);

class DIMERR : public std::exception
{
public:

  const char* what() const throw()
  {
    return "DIMERR:  Dimension error";
  }
};

class MATERR : public std::exception
{
public:

  const char* what() const throw()
  {
    return "MATERR:  Matrix internal error";
  }
};

class SINGERR : public std::exception
{
public:

  const char* what() const throw()
  {
    return "SINGERR:  Matrix singularity";
  }
};

  class MEMERR : public std::exception
  {
  public:
  
    const char* what() const throw()
    {
      return "MEMERR:  Matrix memory allocation error";
    }
  };

  class DIVBYZERO : public std::exception
  {
  public:
  
    const char* what() const throw()
    {
      return "DIVBYZERO:  Divide by zero error";
    }
  };

private:

  void Construct(int    rows,
                 int    cols,
                 double initVal);

  void Construct(int    rows,
                 int    cols);

  bool WithinTolerance(double arg1,
                       double arg2,
                       double tol = 0.0);

  double Max(double arg1, double arg2);

  int    cols;
  int    nElem;
  int    rows;
  double *pData;

};  // end class Matrix

namespace MATRIX
{

Matrix operator+ (const double lhs, Matrix rhs);

Matrix operator- (const double lhs, Matrix rhs);

Matrix operator* (const double lhs, Matrix rhs);

Matrix operator/ (const double lhs, Matrix rhs);

Matrix Invert(Matrix &MM);

double DotProduct(Matrix arg1, Matrix arg2);

Matrix Transpose(Matrix arg);

bool IsSymmetric(Matrix arg);

} // end namespace MATRIX

#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
