//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
/*

 mathUtils.hpp

 Overview:

  A collection of math utilities.

*/
///////////////////////////////////////////////////////////////////////////////


#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

/*
// *****************************************************************************
//
//                         Miscellaneous math functions
//
// *****************************************************************************
*/

//=======================================================================
//
// \brief  Square a value
//
// \note   This function is compatibile with real-time coding standards
//
// \param[in]  x     Vector left hand side of the addition.
// \return           Input \a x squared. x*x
//
//=======================================================================
template <typename T>
T SQ(T x)
{
  return x*x;
}


/*
 * Return sign of x
 */

/*!
 * Output cumulative probability
 */

void abscdf(double *data,
            int    samples,
            int    bins);

/*!
 * Output histogram data
 */
void histogram(double *data,
               int    samples,
               int    bins);

//
// Argument of signum function
//
double sgn(double x);

//
// Return minimum of x and y
//
double mini(double x, double y);

//
// Return maximum of x and y
//
double maxi(double x, double y);

//
// Integrator-friendly delta function approximation
//
double delta(double t, double td, double dt);

//
// Apply upper and lower limits to input
//
double limit(double val, double lower, double upper);

//
// Quantize input signal to nearest LSB, rounding up
//
double quant(double val, double lsb);

/*
// *****************************************************************************
//
//                          Simple lookup table functions
//
// *****************************************************************************
*/

double lut1d( double *xlist,
              double *lut,
              int    x_dim,
              double x );

double lut2d( double *xlist,
              double *ylist,
              double *lut,
              int    x_dim,
              int    y_dim,
              double x,
              double y );


/*
// *****************************************************************************
//
//                            Linear algebra functions
//
// *****************************************************************************
*/

//
// Matrix definition
//
// typedef struct _matrix_
// {
//   int    rows;
//   int    cols;
//   double data[10][10];
// } Matrix;

//
// Vector definition
//
// typedef struct _vector_
// {
//   int    rows;
//   double data[10];
// } VecType;


// void vec_equal( VecType *v, VecType *out );

// void vec_set( VecType *v, int dim, ... );

// void vec_print( VecType *v );

// //
// // Vector-Vector addition
// //
// void vv_add( VecType *v1, VecType *v2, VecType *out );

// //
// // Vector-Vector subtraction
// //
// void vv_sub( VecType *v1, VecType *v2, VecType *out );

// //
// // Scalar-Vector multiplication
// //
// void sv_mul( double s, VecType *v, VecType *out );

// void mat_equal( Matrix *A, Matrix *out );

// void mat_set( Matrix *A, int rows, int cols, ... );

// void mat_set_element( Matrix *A, int row, int col, double val );

// void mat_print( Matrix *A );

// //
// // Matrix-Matrix addition
// //
// void mm_add( Matrix *A, Matrix *B, Matrix *out );

// //
// // Matrix-Matrix subtraction
// //
// void mm_sub( Matrix *A, Matrix *B, Matrix *out );

// //
// // Matrix-Matrix multiplication
// //
// void mm_mul( Matrix *A, Matrix *B, Matrix *out );

// //
// // Matrix-Vector multiplication
// //
// void mv_mul( Matrix *A, VecType *v, VecType *out );

// //
// // Scalar-Matrix multiplication
// //
// void sm_mul( double s, Matrix *A, Matrix *out );

// //
// // Matrix transpose
// //
// void transpose( Matrix *A, Matrix *out );

// //
// // Numerical Recipes lower-upper matrix decomposition
// //
// void ludcmp(Matrix *M, int *indx, double *d);

// //
// // Numerical Recipes lower-upper matrix back substitution
// //
// void lubksb(Matrix *M, int *indx, double *b);

// //
// // Matrix inversion
// //
// void inverse( Matrix *A, Matrix *out );

// /*!
//  * Initializes a zero vector
//  */
// void zero_vector( VecType *v,
//                   int rows );


// /*!
//  * Initializes a zero matrix
//  */
// void zero_matrix( Matrix *a,
//                   int    rows,
//                   int    cols );

/*
// *****************************************************************************
//
//                        Random number generator code
//
// *****************************************************************************
*/

//
// Numerical Recipes uniform random number generator
//
double ran1(long *idum);

//
// Numerical Recipes Gaussian random number generator
//
double gasdev(long *idum);

//
// Uniform random number generator
//
double uniform(double min, double max, long *seed);

//
// Gaussian random number generator
//
double gaussian(double mean, double sigma, long *seed);



/*
// *****************************************************************************
//
//                       Statistics functions
//
// *****************************************************************************
*/

//
// Compute mean of data values
//
double mean(int samples, double *data);

//
// Compute root-mean-square of data values
//
double rms(int samples, double *data);

//
// Compute standard deviation of data values
//
double stdd(int samples, double *data);

//
// Find maximum value in data values
//
double theMaximum(int samples, double *data);

//
// Find minimum values in data values
//
double minimum(int samples, double *data);

//
// Find median values in data values
//
double median(int samples, double *data);

//
// Find p90 values in data array.  The data array will be sorted.
//
double p90(int samples, double *data);

//
// Find p98 values in data array.  The data array will be sorted.
//
double p98(int samples, double *data);

//
// Compare two doubles
//  arg1 < arg2 ==> -1
//  arg1 == arg2 ==> 0
//  arg1 > arg2 ==>  1
//
int compDbl(const void *arg1, const void *arg2);



#endif

//##############################################################################
//
//                              UNCLASSIFIED
//
//##############################################################################
