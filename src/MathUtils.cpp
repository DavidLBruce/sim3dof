//##############################################################################
//                              UNCLASSIFIED
//##############################################################################
/*
 mathUtils.cpp

 Overview:

  A collection of math utilities.
*/
//##############################################################################


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "MathUtils.hpp"

/*
// *****************************************************************************
//
//                         Miscellaneous math functions
//
// *****************************************************************************
*/

/*!
 * Return sign of x
 */
double sgn(double x)
{
  if (x < 0.0)
    return -1.0;
  else
    return 1.0;
}


/*!
 * Return minimum of x and y
 */
double mini(double x,
            double y)
{
  if (x < y) return x;
  else       return y;
}


/*!
 * Return maximum of x and y
 */
double maxi(double x,
            double y)
{
  if (x > y)
    return x;
  else
    return y;
}


/*!
 * Integrator-friendly delta function approximation
 */
double delta(double t,
             double td,
             double dt)
{
  if ((t>=td)&&(t<td+dt))      return 1.0/dt;
  else                         return 0.0;
}


/*!
 * Apply upper and lower limits to input
 */
double limit(double val,
             double lower_lim,
             double upper_lim)
{
  if (val < lower_lim)
    return lower_lim;
  if (val > upper_lim)
    return upper_lim;

  return val;
}

/*!
 * Quantize input signal to nearest LSB, rounding up
 */
double quant(double val, double lsb)
{
  double x;
  int    intx;
  double quantized_val;

  if (lsb > 1e-10) {      /// allow lsb = 0 to turn off, also help prevent overflow
      x = val/lsb;
      intx = (int)x;
      if ( fabs(x - (double)intx) >= 0.5 )   intx = intx + (int)sgn(x);
      quantized_val = lsb*intx;
  } else {
      quantized_val = val;
  }
  return quantized_val;
}

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
              double x )
{
  int xu;
  int xl;
  int xm;
  if (x < xlist[0])
    x = xlist[0];
  if (x > xlist[x_dim-1])
    x = xlist[x_dim-1];

  xu = x_dim - 1;
  xl = 0;
  xm = (xu + xl) / 2;

  if (x == xlist[xu])
    xl = xu;
  else if (x == xlist[xl])
    xu = xl;
  else {
    while (xu - xl > 1) {
      xm = (xu + xl) / 2;
      if (xlist[xm] == x)
        xl = xu = xm;
      else if (xlist[xm] > x)
        xu = xm;
      else xl = xm;
    }
  }

  if (xl == xu) {
    return lut[xu];
  } else {
    return lut[xl] + (x - xlist[xl]) *
                         ((lut[xu] - lut[xl]) / (xlist[xu] - xlist[xl]));
  }
}


double lut2d( double *xlist,
              double *ylist,
              double *lut,
              int    x_dim,
              int    y_dim,
              double x,
              double y )
{
  int xu;
  int xl;
  int xm;

  int yu;
  int yl;
  int ym;

  int    i1;
  int    i2;
  int    i3;
  int    i4;
  double fxy1;
  double fxy2;

  if (x < xlist[0])
    x = xlist[0];
  if (x > xlist[x_dim-1])
    x = xlist[x_dim-1];
  if (y < ylist[0])
    y = ylist[0];
  if (y > ylist[y_dim-1])
    y = ylist[y_dim-1];

  xu = x_dim - 1;
  xl = 0;
  xm = (xu + xl) / 2;

  if (x == xlist[xu])
    xl = xu;
  else if (x == xlist[xl])
    xu = xl;
  else {
    while (xu - xl > 1) {
      xm = (xu + xl) / 2;
      if (xlist[xm] == x)
        xl = xu = xm;
      else if (xlist[xm] > x)
        xu = xm;
      else xl = xm;
    }
  }

  yu = y_dim - 1;
  yl = 0;
  ym = (yu + yl) / 2;

  if (y == ylist[yu])
    yl = yu;
  else if (y == ylist[yl])
    yu = yl;
  else {
    while (yu - yl>1) {
      ym = (yu + yl) / 2;
      if (ylist[ym] == y)
        yl = yu = ym;
      else if (ylist[ym] > y)
        yu = ym;
      else
        yl = ym;
    }
  }

  i1 = xl * y_dim + yl;
  i2 = xl * y_dim + yu;
  i3 = xu * y_dim + yl;
  i4 = xu * y_dim + yu;

  if (xl == xu) {
    fxy1 = lut[i1];
    fxy2 = lut[i2];
  } else {
    fxy1 = lut[i1] + (lut[i3] - lut[i1]) * (x -xlist[xl]) / (xlist[xu] - xlist[xl]);
    fxy2 = lut[i2] + (lut[i4] - lut[i2]) * (x - xlist[xl])/(xlist[xu]-xlist[xl]);
  }

  if (yl==yu)
    return fxy1;
  else
    return fxy1 + (fxy2 - fxy1)*(y - ylist[yl])/(ylist[yu] - ylist[yl]);
}


/*
// *****************************************************************************
//
//                          Linear algebra functions
//
// *****************************************************************************
*/


// /*!
//  * Vector equality.  Copies v into out.
//  */
// void vec_equal( VecType *v,
//                 VecType *out )
// {
//   int i;

//   for (i = 0; i < v->rows; i++)
//     out->data[i] = v->data[i];

//   out->rows = v->rows;
// }


// /*!
//  * Vector initialization.
//  */
// void vec_set( VecType *v,
//               int    dim,
//               ... )
// {
//   int i;
//   va_list argptr;

//   v->rows = dim;

//   va_start(argptr, dim);

//   for (i = 0; i < dim; i++)
//     v->data[i] = va_arg(argptr, double);

//   va_end(argptr);
// }


// /*!
//  * Vector text output.
//  */
// void vec_print( VecType *v )
// {
//   int i;

//   for (i = 0; i < v->rows; i++)
//     printf("%f ", v->data[i]);
// }


// /*!
//  * Vector-vector addition.
//  */
// void vv_add( VecType *v1,
//              VecType *v2,
//              VecType *out )
// {
//   int i;

//   if (v1->rows != v2->rows) {
//     fprintf(stderr, "Vector dimensions not equal in vv_add\n");
//     exit(0);
//   }

//   for (i=0; i<v1->rows; i++)
//     out->data[i] = v1->data[i] + v2->data[i];

//   out->rows = v1->rows;
// }


// /*!
//  * Vector-vector subtraction.
//  */
// void vv_sub( VecType *v1,
//              VecType *v2,
//              VecType *out )
// {
//   int i;

//   if (v1->rows != v2->rows) {
//     fprintf(stderr, "Vector dimensions not equal in vv_sub\n");
//     exit(0);
//   }

//   for (i=0; i<v1->rows; i++)
//     out->data[i] = v1->data[i] - v2->data[i];

//   out->rows = v1->rows;
// }


// /*!
//  * Scalar-vector multiplication
//  */
// void sv_mul( double s,
//              VecType *v,
//              VecType *out )
// {
//   int i;

//   for (i=0; i<v->rows; i++)
//     out->data[i]            = s*v->data[i];

//   out->rows                    = v->rows;
// }


// /*!
//  * Matrix equality.
//  */
// void mat_equal( Matrix *A,
//                 Matrix *out )
// {
//   int i, j;

//   for (i = 0; i < A->rows; i++)
//     for (j = 0; j < A->cols; j++)
//       out->data[i][j] = A->data[i][j];

//   out->rows = A->rows;
//   out->cols = A->cols;
// }


// /*!
//  * Matrix initialization.
//  */
// void mat_set( Matrix *A,
//               int    rows,
//               int cols,
//               ... )
// {
//   int i, j;
//   va_list argptr;

//   A->rows = rows;
//   A->cols = cols;

//   va_start(argptr, cols);

//   for (i = 0; i < rows; i++)
//     for (j = 0; j < cols; j++)
//       A->data[i][j] = va_arg(argptr, double);

//   va_end(argptr);
// }


// /*!
//  * Matrix element initialization.
//  */

// void mat_set_element( Matrix *A,
//                       int    row,
//                       int    col,
//                       double val )
// {
//   A->data[row][col] = val;
// }


// /*!
//  *  Matrix text output.
//  */
// void mat_print( Matrix *A )
// {
//   int i, j;

//   for (i = 0; i < A->rows; i++) {

//     for (j = 0; j < A->cols; j++)
//       printf("%.8f ", A->data[i][j]);

//     printf("\n");
//   }
// }


// /*!
//  * Matrix-matrix addition.
//  */
// void mm_add( Matrix *A,
//              Matrix *B,
//              Matrix *out )
// {
//   int i, j;

//   if ((A->rows != B->rows) && (A->cols != B->cols)) {
//     fprintf(stderr, "Matrix dimensions not equal in mm_add\n");
//     exit(0);
//   }

//   for (i = 0; i < A->rows; i++)
//     for (j=0; j<A->cols; j++)
//       out->data[i][j] = A->data[i][j] + B->data[i][j];

//   out->rows = A->rows;
//   out->cols = A->cols;
// }


// /*!
//  * Matrix-matrix subtraction.
//  */
// void mm_sub( Matrix *A,
//              Matrix *B,
//              Matrix *out )
// {
//   int i, j;

//   if ((A->rows != B->rows) && (A->cols != B->cols)) {
//     fprintf(stderr, "Matrix dimensions not equal in mm_sub\n");
//     exit(0);
//   }

//   for (i = 0; i < A->rows; i++)
//     for (j=0; j<A->cols; j++)
//       out->data[i][j] = A->data[i][j] - B->data[i][j];

//   out->rows = A->rows;
//   out->cols = A->cols;
// }


// /*!
//  * Matrix-matrix multiplication.
//  */
// void mm_mul( Matrix *A,
//              Matrix *B,
//              Matrix *out )
// {
//   int i, j, k;

//   if (A->cols != B->rows) {
//     fprintf(stderr, "Matrix dimension incompatibility in mm_mul\n");
//     exit(0);
//   }

//   for (i = 0; i < A->rows; i++) {

//     for (j = 0; j < B->cols; j++) {

//       out->data[i][j] = 0.0;

//       for (k=0; k<A->cols; k++)
//         out->data[i][j] += A->data[i][k] * B->data[k][j];
//     }
//   }

//   out->rows = A->rows;
//   out->cols = B->cols;
// }


// /*!
//  * Matrix-vector multiplication.
//  */
// void mv_mul( Matrix *A,
//              VecType *v,
//              VecType *out )
// {
//   int i, j;

//   if (A->cols != v->rows) {
//     fprintf(stderr, "Dimension incompatibility in mv_mul\n");
//     exit(0);
//   }

//   for (i = 0; i < A->rows; i++) {

//     out->data[i] = 0.0;

//     for (j=0; j<A->cols; j++)
//       out->data[i] += A->data[i][j]*v->data[j];
//   }

//   out->rows = A->rows;
// }


// /*!
//  * Matrix-scalar multiplication.
//  */
// void sm_mul( double s,
//              Matrix *A,
//              Matrix *out )
// {
//   int i, j;

//   for (i=0; i<A->rows; i++) {

//     for (j=0; j<A->cols; j++)
//       out->data[i][j] = s * A->data[i][j];
//   }

//   out->rows = A->rows;
//   out->cols = A->cols;
// }


// /*!
//  * Matrix transpose.
//  */
// void transpose( Matrix *A,
//                 Matrix *out )
// {
//   int i, j;

//   for (i=0; i<A->rows; i++)
//     for (j=0; j<A->cols; j++)
//       out->data[j][i] = A->data[i][j];

//   out->rows = A->cols;
//   out->cols = A->rows;
// }


// /*!
//  * Numerical Recipes in C LU decomposition algorithm
//  */
// void ludcmp(Matrix *M,
//             int    *indx,
//             double *d)
// {
//   int    NMAX  = 500;
//   int    imax  = 0;
//   int    i, j, k;
//   double TINY  = 1.0e-20;
//   double aamax, dum, sum, temp;
//   double vv[500];  // Used to be NMAS but Visual Studio did not like...
//   int    n     = M->rows;
//   int    rows  = M->rows;
//   int    cols  = M->cols;

//   if (rows!=cols) {
//     printf("ludcmp: input matrix is not square\n");
//     exit(0);
//   }

//   *d = 1.0;

//   for (i = 0; i < n; i++) {

//     aamax = 0.0;

//     for (j = 0; j < n; j++)
//       if ((temp = fabs(M->data[i][j])) > aamax)
//         aamax = temp;

//     if (aamax == 0.0) {
//       printf("ludcmp: singular matrix\n");
//       mat_print(M); printf("\n");
//       exit(0);
//     }
//     vv[i] = 1.0 / aamax;
//   }

//   for (j = 0; j < n; j++) {

//     for (i = 0; i < j; i++) {

//       sum = M->data[i][j];

//       for (k=0; k<i; k++)
//         sum -= M->data[i][k]*M->data[k][j];

//       M->data[i][j] = sum;
//     }

//     aamax = 0.0;

//     for (i=j; i<n; i++) {

//       sum = M->data[i][j];

//       for (k=0; k<j; k++)
//         sum -= M->data[i][k] * M->data[k][j];

//       M->data[i][j] = sum;

//       if ((dum=vv[i] * fabs(sum)) >= aamax) {
//         imax  = i;
//         aamax = dum;
//       }
//     }

//     if (j!=imax) {
//       for (k = 0; k < n; k++) {
//         dum = M->data[imax][k];
//         M->data[imax][k] = M->data[j][k];
//         M->data[j][k] = dum;
//       }
//       *d = -(*d);
//       vv[imax] = vv[j];
//     }

//     indx[j] = imax;
//     if (M->data[j][j] == 0.0)
//       M->data[j][j] = TINY;

//     if (j != n) {
//       dum = 1.0 / M->data[j][j];
//       for (i = j + 1; i < n; i++)
//         M->data[i][j] *= dum;
//     }
//   }
// }


// /*!
//  * Numerical Recipes LU back substitution algorithm
//  */
// void lubksb(Matrix *M,
//             int *indx,
//             double *b)
// {
//   int    i, j, ll;
//   int    ii   = -1;
//   double sum;
//   int    rows = M->rows;
//   int    cols = M->cols;
//   int    n    = M->rows;

//   if (rows!=cols) {
//     printf("lubksb: Matrix is not square\n");
//     exit(0);
//   }

//   ii = -1;

//   for (i=0; i<n; i++) {

//     ll    = indx[i];
//     sum   = b[ll];
//     b[ll] = b[i];

//     if (ii!=-1)
//       for (j=ii; j<i; j++)
//         sum -= M->data[i][j]*b[j];
//     else if (sum != 0)
//       ii = i;

//     b[i] = sum;
//   }

//   for (i = n-1; i >= 0; i--) {
//     sum = b[i];

//     for (j = i+1; j < n; j++)
//       sum -= M->data[i][j]*b[j];

//     b[i] = sum/M->data[i][i];
//   }
// }


// /*!
//  * Matrix inversion.
//  */
// void inverse( Matrix *A,
//               Matrix *out )
// {
//   int    indx[200];
//   double d;
//   double col[200];
//   int    rows = A->rows;
//   int    cols = A->cols;
//   int    n    = cols;
//   int    i, j;
//   Matrix AA;

//   if (rows!=cols) {
//     printf("inverse: Matrix is not square!\n");
//     exit(-1);
//   }

//   out->rows = cols;
//   out->cols = rows;

//   if (rows == 1)
//     out->data[0][0] = 1.0 / A->data[0][0];
//   else {

//     for (i=0; i<cols; i++) {
//       indx[i] = 0;
//       col[i]  = 0.0;
//     }

//     for (i=0; i<A->rows; i++)
//       for (j=0; j<A->cols; j++)
//         AA.data[i][j] = A->data[i][j];

//     AA.rows = A->rows;
//     AA.cols = A->cols;

//     ludcmp(&AA, indx, &d);

//     for (j = 0; j < n; j++) {

//       for (i = 0; i < n; i++)
//         col[i] = 0.0;

//       col[j] = 1.0;

//       lubksb(&AA, indx, col);

//       for (i = 0; i < n; i++)
//         out->data[i][j] = col[i];
//     }
//   }
// }


// /*!
//  * Initializes a zero vector
//  */
// void zero_vector( VecType *v,
//                   int rows )
// {
//   int i;
//   for (i=0; i<(v->rows=rows); i++)
//     v->data[i] = 0.0;
// }


// /*!
//  * Initializes a zero matrix
//  */
// void zero_matrix( Matrix *a,
//                   int    rows,
//                   int    cols )
// {
//   int i, j;

//   for (i = 0; i < (a->rows=rows); i++)
//     for (j = 0; j < (a->cols=cols); j++)
//       a->data[i][j] = 0.0;
// }


// /*!
//  * Initializes an identity matrix
//  */
// void identity_matrix( Matrix *a,
//                       int    rows,
//                       int    cols )
// {
//   int i, j;
//   if (rows!=cols) {
//     fprintf(stderr, "Matrix dimensions not square in identity_matrix\n");
//     exit(0);
//   }

//   for (i = 0; i < (a->rows=rows); i++)
//     for (j=0; j < (a->cols=cols); j++)
//       if (i == j)
//         a->data[i][j] = 1.0;
//       else
//         a->data[i][j] = 0.0;
// }


/*
// *****************************************************************************
//
//                         Random number generator code
//
// *****************************************************************************
*/

#define IA 16807
#define IM 2147483647
#define AM (1.0 / IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1 + (IM - 1) / NTAB)
#define EPS 1.2e-7
#define RNMX (1.0 - EPS)


/*!
 * Numerical Recipes uniform random number generator
 */
double ran1(long *idum)
{
  int j;
  long k;
  static long iy = 0;
  static long iv[NTAB];
  double temp;

  if (*idum <= 0 || !iy) {
    if (-(*idum) < 1)
      *idum = 1;
    else *idum
      = -(*idum);
    for (j = NTAB + 7; j >= 0; j--) {
      k = (*idum) / IQ;
      *idum = IA * (*idum-k * IQ) - IR * k;
      if (*idum<0)
        *idum += IM;
      if (j < NTAB)
       iv[j] = *idum;
    }
    iy = iv[0];
  }

  k = (*idum) / IQ;
  *idum = IA * (*idum - k * IQ) - IR * k;
  if (*idum <= 0) *idum += IM;
  j = iy / NDIV;
  iy = iv[j];
  iv[j] = *idum;
  if ( (temp = AM * iy) > RNMX )
    return RNMX;
  else
    return temp;
}


/*!
 * Numerical Recipes Gaussian random number generator
 */
double gasdev(long *idum)
{
  static int iset              = 0;
  static double gset;
  double fac, rsq, v1, v2;
  int i;
  double sum;
  if (0) {
  if (*idum<0)            iset = 0;
  if (iset==0) {
    do {
      v1                       = 2.0*ran1(idum)-1.0;
      v2                       = 2.0*ran1(idum)-1.0;
      rsq                      = v1*v1 + v2*v2;
    } while ((rsq>=1.0) || (rsq==0.0));
    fac                        = sqrt(-2.0*log(rsq)/rsq);
    gset                       = v1*fac;
    iset                       = 1;
    return v2*fac;
  } else {
    iset                       = 0;
    return gset;
  }
  } else {
    sum = 0;
    for (i=0; i<12; ++i) {
      sum += ran1(idum)-0.5;
    }
    return sum;
  }


}


/*!
 * Uniform random number generator
 */
double uniform(double min,
               double max,
               long *seed)
{
  double ran = ran1(seed);

  return (min + ran * (max - min));
}


/*!
 * Gaussian random number generator
 */
double gaussian(double mean,
                double sigma,
                long   *seed)
{
  double rannum;

  while (fabs(rannum = gasdev(seed)) > 3) {}

  return mean + sigma*rannum;
}


/*!
 * Weibull random number generator
 */
double weibull( double alpha,
                double beta,
                long   *seed)
{
  double ran;

  do {
    ran = ran1(seed);
  } while (ran == 0.0);

  return beta * pow(-log(ran), 1.0 / alpha);
}


/*!
 * Exponential distribution random number generator
 */
double exponential( double av,
                    long   *seed )
{
  double ran;

  do {
    ran = ran1(seed);
  } while (ran == 0.0);

  return -av * log(ran);
}


/*
// *****************************************************************************
//
//                             Statistics functions
//
// *****************************************************************************
*/


/*!
 *  Compute mean of data values
 */
double mean(int    samples,
            double *data)
{
  double sum = 0.0;
  int    i;

  for (i = 0; i < samples; i++)
    sum = sum + data[i];

  return sum / (double)samples;
}


/*!
 * Compute root-mean-square of data values
 */
double rms(int    samples,
           double *data)
{
  double sumsq = 0.0;
  int    i;

  for (i = 0; i < samples; i++)
    sumsq += data[i] * data[i];

  return sqrt(sumsq / ((double)samples));
}


/*!
 * Compute standard deviation of data values
 */
double stdd(int    samples,
            double *data)
{
  double mean;
  double stdev;
  if (samples == 1)
  {
    return 0.0;
  }
  else
  {
    double sum = 0.0;
    int    i;

    for (i = 0; i < samples; i++)
      sum += data[i];

    mean  = sum / ((double)samples);
    stdev = 0.0;

    for (i=0; i < samples; i++)
      stdev = stdev + ((data[i] - mean) * (data[i] - mean));

    return sqrt(stdev / ((double)samples - 1));
  }
}


/*!
 * Find maximum value in data values
 */
double theMaximum(int    samples,
               double *data)
{
  double max = data[0];
  int    i;

  for (i = 0; i < samples; i++) {
    if (data[i] > max)
      max = data[i];
  }

  return max;
}


/*!
 * Find minimum values in data values
 */
double minimum(int    samples,
               double *data)
{
  double min = data[0];
  double mm;
  int i;

  for (i = 0; i < samples; i++) {

    mm = data[i];

    if (mm < min)
      min = mm;
  }

  return min;
}

//
// Find median values in data array.  The data array will be sorted.
//
double median(int qtyElem, double *data)
{
  double ans;

  qsort(data, qtyElem, sizeof(double), compDbl);

  if ((qtyElem % 2) == 0) {
    ans = 0.5 * (data[qtyElem/2 - 1] + data[qtyElem/2]);
  } else {
    ans = data[(qtyElem - 1) / 2];
  }
  return ans;
}

//
// Find p90 values in data array.  The data array will be sorted.
//
double p90(int qtyElem, double *data)
{
  double ans;

  qsort(data, qtyElem, sizeof(double), compDbl);

  ans = data[(qtyElem*9/10)-1];

  return ans;
}

//
// Find p98 values in data array.  The data array will be sorted.
//
double p98(int qtyElem, double *data)
{
  double ans;

  qsort(data, qtyElem, sizeof(double), compDbl);

  ans = data[(qtyElem*98/100)-1];

  return ans;
}

//
// Compare two doubles
//  arg1 < arg2 ==> -1
//  arg1 == arg2 ==> 0
//  arg1 > arg2 ==>  1
//
int compDbl(const void *arg1, const void *arg2)
{
  double ansDbl;

  ansDbl = *(double*)arg1 - *(double*)arg2;

  if (ansDbl < 0.0){
    return -1;
  } else if ( ansDbl == 0 ) {
    return 0;
  } else {
    return 1;
  }
}

/*!
 * Output histogram data
 */
void histogram(double *data,
               int    samples,
               int    bins)
{
  int    i, j;
  double min_value     = minimum(samples, data);
  double max_value     = theMaximum(samples, data);
  double last_value    = min_value;
  double current_value = 0.0;
  int    count;

  printf("%d \t %7.3f \t %d\n", 0, min_value, 0);

  for (i=1; i<=bins; i++) {
    current_value = min_value + (max_value - min_value) * (double)i/(double)bins;
    count = 0;

    for (j = 0; j < samples; j++)
      if ( (data[j] < current_value) && (data[j] >= last_value))
        count++;

    printf("%d \t %7.3f \t %d\n", i, current_value, count);
    last_value = current_value;
  }
}



/*!
 * Output cumulative probability
 */

void cdf(double *data,
         int    samples,
         int    bins)
{
  int    i, j;
  double min_value     = minimum(samples, data);
  double max_value     = theMaximum(samples, data);
  double current_value = 0.0;
  int    count;

  printf("%d \t %7.3f \t %d \t %7f\n", 0, min_value, 0, 0.0);

  for (i = 1; i <= bins; i++) {
    current_value = min_value +
                      (max_value - min_value) * (double)i / (double)bins;
    count = 0;
    for (j = 0; j < samples; j++)
      if (data[j] <= current_value)
        count++;

    printf("%d \t %7f \t %d \t %7f\n",
         i, current_value, count, (double)count / (double)samples);
  }
}


/*!
 * Output cumulative probability
 */

void abscdf(double *data,
            int    samples,
            int    bins)
{
  int    i, j;
  double tmp_min = fabs(minimum(samples, data));
  double tmp_max = fabs(theMaximum(samples, data));
  double current_value = 0.0;
  int    count;

  double max_value = maxi(tmp_min, tmp_max);

  printf("%d \t %7f \t %d \t %7f\n", 0, 0.0, 0, 0.0);

  for (i=1; i<=bins; i++) {
    current_value = max_value*(double)i/(double)bins;
    count = 0;
    for (j = 0; j < samples; j++)
      if (fabs(data[j]) <= current_value)
        count++;
    printf("%d \t %7f \t %d \t %f\n",
        i, current_value, count, (double)count / (double)samples);
  }
}


//##############################################################################
//                              UNCLASSIFIED
//##############################################################################


