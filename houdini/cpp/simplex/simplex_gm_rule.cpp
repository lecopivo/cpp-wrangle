#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

#include "simplex_gm_rule.h"

namespace simplex {

//****************************************************************************80

void comp_next(int n, int k, int a[], bool &more, int &h, int &t)

//****************************************************************************80
//
//  Purpose:
//
//    COMP_NEXT computes the compositions of the integer N into K parts.
//
//  Discussion:
//
//    A composition of the integer N into K parts is an ordered sequence
//    of K nonnegative integers which sum to N.  The compositions (1,2,1)
//    and (1,1,2) are considered to be distinct.
//
//    The routine computes one composition on each call until there are no more.
//    For instance, one composition of 6 into 3 parts is
//    3+2+1, another would be 6+0+0.
//
//    On the first call to this routine, set MORE = FALSE.  The routine
//    will compute the first element in the sequence of compositions, and
//    return it, as well as setting MORE = TRUE.  If more compositions
//    are desired, call again, and again.  Each time, the routine will
//    return with a new composition.
//
//    However, when the LAST composition in the sequence is computed
//    and returned, the routine will reset MORE to FALSE, signaling that
//    the end of the sequence has been reached.
//
//    This routine originally used a SAVE statement to maintain the
//    variables H and T.  I have decided that it is safer
//    to pass these variables as arguments, even though the user should
//    never alter them.  This allows this routine to safely shuffle
//    between several ongoing calculations.
//
//
//    There are 28 compositions of 6 into three parts.  This routine will
//    produce those compositions in the following order:
//
//     I         A
//     -     ---------
//     1     6   0   0
//     2     5   1   0
//     3     4   2   0
//     4     3   3   0
//     5     2   4   0
//     6     1   5   0
//     7     0   6   0
//     8     5   0   1
//     9     4   1   1
//    10     3   2   1
//    11     2   3   1
//    12     1   4   1
//    13     0   5   1
//    14     4   0   2
//    15     3   1   2
//    16     2   2   2
//    17     1   3   2
//    18     0   4   2
//    19     3   0   3
//    20     2   1   3
//    21     1   2   3
//    22     0   3   3
//    23     2   0   4
//    24     1   1   4
//    25     0   2   4
//    26     1   0   5
//    27     0   1   5
//    28     0   0   6
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 July 2008
//
//  Author:
//
//    Original FORTRAN77 version by Albert Nijenhuis, Herbert Wilf.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Albert Nijenhuis, Herbert Wilf,
//    Combinatorial Algorithms for Computers and Calculators,
//    Second Edition,
//    Academic Press, 1978,
//    ISBN: 0-12-519260-6,
//    LC: QA164.N54.
//
//  Parameters:
//
//    Input, int N, the integer whose compositions are desired.
//
//    Input, int K, the number of parts in the composition.
//
//    Input/output, int A[K], the parts of the composition.
//
//    Input/output, bool &MORE.
//    Set MORE = FALSE on first call.  It will be reset to TRUE on return
//    with a new composition.  Each new call returns another composition until
//    MORE is set to FALSE when the last composition has been computed
//    and returned.
//
//    Input/output, int &H, &T, two internal parameters needed for the
//    computation.  The user should allocate space for these in the calling
//    program, include them in the calling sequence, but never alter them!
//
{
  int i;

  if (!(more)) {
    t    = n;
    h    = 0;
    a[0] = n;
    for (i = 1; i < k; i++) {
      a[i] = 0;
    }
  } else {
    if (1 < t) {
      h = 0;
    }
    h        = h + 1;
    t        = a[h - 1];
    a[h - 1] = 0;
    a[0]     = t - 1;
    a[h]     = a[h] + 1;
  }

  more = (a[k - 1] != n);

  return;
}
//*****************************************************************************/

void gm_general_rule_set(int rule, int m, int n, double t[], double w[],
                         double x[])

//*****************************************************************************/
//
//  Purpose:
//
//    GM_GENERAL_RULE_SET sets a Grundmann-Moeller rule for a general simplex.
//
//  Discussion:
//
//    The vertices of the simplex are given by the array T.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 March 2017
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Axel Grundmann, Michael Moeller,
//    Invariant Integration Formulas for the N-Simplex
//    by Combinatorial Methods,
//    SIAM Journal on Numerical Analysis,
//    Volume 15, Number 2, April 1978, pages 282-290.
//
//  Parameters:
//
//    Input, int RULE, the index of the rule.
//    0 <= RULE.
//
//    Input, int M, the spatial dimension.
//    1 <= M.
//
//    Input, int N, the number of points in the rule.
//
//    Input, double T[M*(M+1)], the vertices of the simplex.
//
//    Output, double W[N], the weights.
//
//    Output, double X[M*N], the abscissas.
//
{
  int     j;
  double  volume;
  double  volume1;
  double *w1;
  double *x1;
  //
  //  Get the unit rule.
  //
  w1 = new double[n];
  x1 = new double[m * n];

  gm_unit_rule_set(rule, m, n, w1, x1);
  //
  //  Compute the volume of the unit simplex.
  //
  volume1 = simplex_unit_volume(m);
  //
  //  Compute the volume of the general simplex.
  //
  volume = simplex_general_volume(m, t);
  //
  //  Convert the points.
  //
  simplex_unit_to_general(m, n, t, x1, x);
  //
  //  Convert the weights.
  //
  for (j = 0; j < n; j++) {
    w[j] = w1[j] * volume / volume1;
  }

  delete[] w1;
  delete[] x1;

  return;
}
//****************************************************************************80

int gm_rule_size(int rule, int m)

//****************************************************************************80
//
//  Purpose:
//
//    GM_RULE_SIZE determines the size of a Grundmann-Moeller rule.
//
//  Discussion:
//
//    This rule returns the value of N, the number of points associated
//    with a GM rule of given index.
//
//    After calling this rule, the user can use the value of N to
//    allocate space for the weight vector as W(N) and the abscissa
//    vector as X(M,N), and then call GM_RULE_SET.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 July 2007
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Axel Grundmann, Michael Moeller,
//    Invariant Integration Formulas for the N-Simplex
//    by Combinatorial Methods,
//    SIAM Journal on Numerical Analysis,
//    Volume 15, Number 2, April 1978, pages 282-290.
//
//  Parameters:
//
//    Input, int RULE, the index of the rule.
//    0 <= RULE.
//
//    Input, int M, the spatial dimension.
//    1 <= M.
//
//    Output, int GM_RULE_SIZE, the number of points in the rule.
//
{
  int arg1;
  int n;

  arg1 = m + rule + 1;

  n = i4_choose(arg1, rule);

  return n;
}
//****************************************************************************80

void gm_unit_rule_set(int rule, int m, int n, double w[], double x[])

//****************************************************************************80
//
//  Purpose:
//
//    GM_UNIT_RULE_SET sets a Grundmann-Moeller rule.
//
//  Discussion:
//
//    This is a revised version of the calculation which seeks to compute
//    the value of the weight in a cautious way that avoids intermediate
//    overflow.  Thanks to John Peterson for pointing out the problem on
//    26 June 2008.
//
//    This rule returns weights and abscissas of a Grundmann-Moeller
//    quadrature rule for the M-dimensional unit simplex.
//
//    The dimension N can be determined by calling GM_RULE_SIZE.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 June 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Axel Grundmann, Michael Moeller,
//    Invariant Integration Formulas for the N-Simplex
//    by Combinatorial Methods,
//    SIAM Journal on Numerical Analysis,
//    Volume 15, Number 2, April 1978, pages 282-290.
//
//  Parameters:
//
//    Input, int RULE, the index of the rule.
//    0 <= RULE.
//
//    Input, int M, the spatial dimension.
//    1 <= M.
//
//    Input, int N, the number of points in the rule.
//
//    Output, double W[N], the weights.
//
//    Output, double X[M*N], the abscissas.
//
{
  int *  beta;
  int    beta_sum;
  int    d;
  int    dim;
  int    h;
  int    i;
  int    j;
  int    j_hi;
  int    k;
  bool   more;
  double one_pm;
  int    s;
  int    t;
  double volume1;
  double weight;

  s      = rule;
  d      = 2 * s + 1;
  k      = 0;
  one_pm = 1.0;

  beta = new int[m + 1];

  for (i = 0; i <= s; i++) {
    weight = (double)one_pm;

    j_hi = i4_max(m, i4_max(d, d + m - i));

    for (j = 1; j <= j_hi; j++) {
      if (j <= m) {
        weight = weight * (double)(j);
      }
      if (j <= d) {
        weight = weight * (double)(d + m - 2 * i);
      }
      if (j <= 2 * s) {
        weight = weight / 2.0;
      }
      if (j <= i) {
        weight = weight / (double)(j);
      }
      if (j <= d + m - i) {
        weight = weight / (double)(j);
      }
    }

    one_pm = -one_pm;

    beta_sum = s - i;
    more     = false;
    h        = 0;
    t        = 0;

    for (;;) {
      comp_next(beta_sum, m + 1, beta, more, h, t);

      w[k] = weight;
      for (dim = 0; dim < m; dim++) {
        x[dim + k * m] =
            (double)(2 * beta[dim + 1] + 1) / (double)(d + m - 2 * i);
      }
      k = k + 1;

      if (!more) {
        break;
      }
    }
  }
  //
  //  Normalize.
  //
  volume1 = simplex_unit_volume(m);
  for (i = 0; i < n; i++) {
    w[i] = w[i] * volume1;
  }
  //
  //  Free memory.
  //
  delete[] beta;

  return;
}
//****************************************************************************80

int i4_choose(int n, int k)

//****************************************************************************80
//
//  Purpose:
//
//    I4_CHOOSE computes the binomial coefficient C(N,K).
//
//  Discussion:
//
//    The value is calculated in such a way as to avoid overflow and
//    roundoff.  The calculation is done in integer arithmetic.
//
//    The formula used is:
//
//      C(N,K) = N! / ( K! * (N-K)! )
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 June 2007
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    ML Wolfson, HV Wright,
//    Algorithm 160:
//    Combinatorial of M Things Taken N at a Time,
//    Communications of the ACM,
//    Volume 6, Number 4, April 1963, page 161.
//
//  Parameters:
//
//    Input, int N, K, are the values of N and K.
//
//    Output, int I4_CHOOSE, the number of combinations of N
//    things taken K at a time.
//
{
  int i;
  int mn;
  int mx;
  int value;

  mn = i4_min(k, n - k);

  if (mn < 0) {
    value = 0;
  } else if (mn == 0) {
    value = 1;
  } else {
    mx    = i4_max(k, n - k);
    value = mx + 1;

    for (i = 2; i <= mn; i++) {
      value = (value * (mx + i)) / i;
    }
  }

  return value;
}
//****************************************************************************80

int i4_huge()

//****************************************************************************80
//
//  Purpose:
//
//    I4_HUGE returns a "huge" I4.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, int I4_HUGE, a "huge" integer.
//
{
  return 2147483647;
}
//****************************************************************************80

int i4_max(int i1, int i2)

//****************************************************************************80
//
//  Purpose:
//
//    I4_MAX returns the maximum of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    05 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, two integers to be compared.
//
//    Output, int I4_MAX, the larger of i1 and i2.
//
{
  int value;

  if (i2 < i1) {
    value = i1;
  } else {
    value = i2;
  }
  return value;
}
//****************************************************************************80

int i4_min(int i1, int i2)

//****************************************************************************80
//
//  Purpose:
//
//    I4_MIN returns the smaller of two I4's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    05 May 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I1, I2, two integers to be compared.
//
//    Output, int I4_MIN, the smaller of i1 and i2.
//
{
  int value;

  if (i1 < i2) {
    value = i1;
  } else {
    value = i2;
  }
  return value;
}
//****************************************************************************80

int i4_power(int i, int j)

//****************************************************************************80
//
//  Purpose:
//
//    I4_POWER returns the value of I^J.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 April 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int I, J, the base and the power.  J should be nonnegative.
//
//    Output, int I4_POWER, the value of I^J.
//
{
  int k;
  int value;

  if (j < 0) {
    if (i == 1) {
      value = 1;
    } else if (i == 0) {
      cout << "\n";
      cout << "I4_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J negative.\n";
      exit(1);
    } else {
      value = 0;
    }
  } else if (j == 0) {
    if (i == 0) {
      cout << "\n";
      cout << "I4_POWER - Fatal error!\n";
      cout << "  I^J requested, with I = 0 and J = 0.\n";
      exit(1);
    } else {
      value = 1;
    }
  } else if (j == 1) {
    value = i;
  } else {
    value = 1;
    for (k = 1; k <= j; k++) {
      value = value * i;
    }
  }
  return value;
}
//****************************************************************************80

double *monomial_value(int m, int n, int e[], double x[])

//****************************************************************************80
//
//  Purpose:
//
//    MONOMIAL_VALUE evaluates a monomial.
//
//  Discussion:
//
//    This routine evaluates a monomial of the form
//
//      product ( 1 <= i <= m ) x(i)^e(i)
//
//    The combination 0.0^0 is encountered is treated as 1.0.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    17 August 2014
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the spatial dimension.
//
//    Input, int N, the number of evaluation points.
//
//    Input, int E[M], the exponents.
//
//    Input, double X[M*N], the point coordinates.
//
//    Output, double MONOMIAL_VALUE[N], the monomial values.
//
{
  int     i;
  int     j;
  double *v;

  v = new double[n];
  for (j = 0; j < n; j++) {
    v[j] = 1.0;
  }
  // v = r8vec_ones_new ( n );

  for (i = 0; i < m; i++) {
    if (0 != e[i]) {
      for (j = 0; j < n; j++) {
        v[j] = v[j] * pow(x[i + j * m], e[i]);
      }
    }
  }

  return v;
}
//****************************************************************************80

double r8_factorial(int n)

//****************************************************************************80
//
//  Purpose:
//
//    R8_FACTORIAL computes the factorial of N.
//
//  Discussion:
//
//    factorial ( N ) = product ( 1 <= I <= N ) I
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    16 January 1999
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the argument of the factorial function.
//    If N is less than 1, the function value is returned as 1.
//
//    Output, double R8_FACTORIAL, the factorial of N.
//
{
  int    i;
  double value;

  value = 1.0;

  for (i = 1; i <= n; i++) {
    value = value * (double)(i);
  }

  return value;
}
//****************************************************************************80

double r8ge_det(int n, double a_lu[], int pivot[])

//****************************************************************************80
//
//  Purpose:
//
//    R8GE_DET computes the determinant of a matrix factored by R8GE_FA or
//    R8GE_TRF.
//
//  Discussion:
//
//    The R8GE storage format is used for a "general" M by N matrix.
//    A physical storage space is made for each logical entry.  The two
//    dimensional logical array is mapped to a vector, in which storage is
//    by columns.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 March 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Jack Dongarra, Jim Bunch, Cleve Moler, Pete Stewart,
//    LINPACK User's Guide,
//    SIAM, 1979,
//    ISBN13: 978-0-898711-72-1,
//    LC: QA214.L56.
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input, double A_LU[N*N], the LU factors from R8GE_FA or R8GE_TRF.
//
//    Input, int PIVOT[N], as computed by R8GE_FA or R8GE_TRF.
//
//    Output, double R8GE_DET, the determinant of the matrix.
//
{
  double det;
  int    i;

  det = 1.0;

  for (i = 1; i <= n; i++) {
    det = det * a_lu[i - 1 + (i - 1) * n];
    if (pivot[i - 1] != i) {
      det = -det;
    }
  }

  return det;
}
//****************************************************************************80

int r8ge_fa(int n, double a[], int pivot[])

//****************************************************************************80
//
//  Purpose:
//
//    R8GE_FA performs a LINPACK-style PLU factorization of an R8GE matrix.
//
//  Discussion:
//
//    The R8GE storage format is used for a "general" M by N matrix.
//    A physical storage space is made for each logical entry.  The two
//    dimensional logical array is mapped to a vector, in which storage is
//    by columns.
//
//    R8GE_FA is a simplified version of the LINPACK routine SGEFA.
//
//    The two dimensional array is stored by columns in a one dimensional
//    array.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Jack Dongarra, Jim Bunch, Cleve Moler, Pete Stewart,
//    LINPACK User's Guide,
//    SIAM, 1979,
//    ISBN13: 978-0-898711-72-1,
//    LC: QA214.L56.
//
//  Parameters:
//
//    Input, int N, the order of the matrix.
//    N must be positive.
//
//    Input/output, double A[N*N], the matrix to be factored.
//    On output, A contains an upper triangular matrix and the multipliers
//    which were used to obtain it.  The factorization can be written
//    A = L * U, where L is a product of permutation and unit lower
//    triangular matrices and U is upper triangular.
//
//    Output, int PIVOT[N], a vector of pivot indices.
//
//    Output, int R8GE_FA, singularity flag.
//    0, no singularity detected.
//    nonzero, the factorization failed on the INFO-th step.
//
{
  int    i;
  int    j;
  int    k;
  int    l;
  double t;
  //
  for (k = 1; k <= n - 1; k++) {
    //
    //  Find L, the index of the pivot row.
    //
    l = k;

    for (i = k + 1; i <= n; i++) {
      if (fabs(a[l - 1 + (k - 1) * n]) < fabs(a[i - 1 + (k - 1) * n])) {
        l = i;
      }
    }

    pivot[k - 1] = l;
    //
    //  If the pivot index is zero, the algorithm has failed.
    //
    if (a[l - 1 + (k - 1) * n] == 0.0) {
      cerr << "\n";
      cerr << "R8GE_FA - Fatal error!\n";
      cerr << "  Zero pivot on step " << k << "\n";
      exit(1);
    }
    //
    //  Interchange rows L and K if necessary.
    //
    if (l != k) {
      t                      = a[l - 1 + (k - 1) * n];
      a[l - 1 + (k - 1) * n] = a[k - 1 + (k - 1) * n];
      a[k - 1 + (k - 1) * n] = t;
    }
    //
    //  Normalize the values that lie below the pivot entry A(K,K).
    //
    for (i = k + 1; i <= n; i++) {
      a[i - 1 + (k - 1) * n] = -a[i - 1 + (k - 1) * n] / a[k - 1 + (k - 1) * n];
    }
    //
    //  Row elimination with column indexing.
    //
    for (j = k + 1; j <= n; j++) {
      if (l != k) {
        t                      = a[l - 1 + (j - 1) * n];
        a[l - 1 + (j - 1) * n] = a[k - 1 + (j - 1) * n];
        a[k - 1 + (j - 1) * n] = t;
      }

      for (i = k + 1; i <= n; i++) {
        a[i - 1 + (j - 1) * n] =
            a[i - 1 + (j - 1) * n] +
            a[i - 1 + (k - 1) * n] * a[k - 1 + (j - 1) * n];
      }
    }
  }

  pivot[n - 1] = n;

  if (a[n - 1 + (n - 1) * n] == 0.0) {
    cerr << "\n";
    cerr << "R8GE_FA - Fatal error!\n";
    cerr << "  Zero pivot on step " << n << "\n";
    exit(1);
  }

  return 0;
}
//****************************************************************************80

double r8vec_dot_product(int n, double a1[], double a2[])

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_DOT_PRODUCT computes the dot product of a pair of R8VEC's.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    03 July 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int N, the number of entries in the vectors.
//
//    Input, double A1[N], A2[N], the two vectors to be considered.
//
//    Output, double R8VEC_DOT_PRODUCT, the dot product of the vectors.
//
{
  int    i;
  double value;

  value = 0.0;
  for (i = 0; i < n; i++) {
    value = value + a1[i] * a2[i];
  }
  return value;
}
//****************************************************************************80

double *r8vec_uniform_01_new(int n, int &seed)

//****************************************************************************80
//
//  Purpose:
//
//    R8VEC_UNIFORM_01_NEW returns a new unit pseudorandom R8VEC.
//
//  Discussion:
//
//    This routine implements the recursion
//
//      seed = ( 16807 * seed ) mod ( 2^31 - 1 )
//      u = seed / ( 2^31 - 1 )
//
//    The integer arithmetic never requires more than 32 bits,
//    including a sign bit.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    19 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Paul Bratley, Bennett Fox, Linus Schrage,
//    A Guide to Simulation,
//    Second Edition,
//    Springer, 1987,
//    ISBN: 0387964673,
//    LC: QA76.9.C65.B73.
//
//    Bennett Fox,
//    Algorithm 647:
//    Implementation and Relative Efficiency of Quasirandom
//    Sequence Generators,
//    ACM Transactions on Mathematical Software,
//    Volume 12, Number 4, December 1986, pages 362-376.
//
//    Pierre L'Ecuyer,
//    Random Number Generation,
//    in Handbook of Simulation,
//    edited by Jerry Banks,
//    Wiley, 1998,
//    ISBN: 0471134031,
//    LC: T57.62.H37.
//
//    Peter Lewis, Allen Goodman, James Miller,
//    A Pseudo-Random Number Generator for the System/360,
//    IBM Systems Journal,
//    Volume 8, Number 2, 1969, pages 136-143.
//
//  Parameters:
//
//    Input, int N, the number of entries in the vector.
//
//    Input/output, int &SEED, a seed for the random number generator.
//
//    Output, double R8VEC_UNIFORM_01_NEW[N], the vector of pseudorandom values.
//
{
  int       i;
  const int i4_huge = 2147483647;
  int       k;
  double *  r;

  if (seed == 0) {
    cerr << "\n";
    cerr << "R8VEC_UNIFORM_01_NEW - Fatal error!\n";
    cerr << "  Input value of SEED = 0.\n";
    exit(1);
  }

  r = new double[n];

  for (i = 0; i < n; i++) {
    k = seed / 127773;

    seed = 16807 * (seed - k * 127773) - k * 2836;

    if (seed < 0) {
      seed = seed + i4_huge;
    }

    r[i] = (double)(seed)*4.656612875E-10;
  }

  return r;
}
//****************************************************************************80

double simplex_general_volume(int m, double t[])

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_GENERAL_VOLUME computes the volume of a simplex in N dimensions.
//
//  Discussion:
//
//    The formula is:
//
//      volume = 1/M! * det ( B )
//
//    where B is the M by M matrix obtained by subtracting one
//    vector from all the others.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    29 March 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the dimension of the space.
//
//    Input, double T[M*(M+1)], the vertices.
//
//    Output, double SIMPLEX_GENERAL_VOLUME, the volume of the simplex.
//
{
  double *b;
  double  det;
  int     i;
  int     info;
  int     j;
  int *   pivot;
  double  volume;

  pivot = new int[m];
  b     = new double[m * m];

  for (j = 0; j < m; j++) {
    for (i = 0; i < m; i++) {
      b[i + j * m] = t[i + j * m] - t[i + m * m];
    }
  }

  r8ge_fa(m, b, pivot);

  det = r8ge_det(m, b, pivot);

  volume = fabs(det);
  for (i = 1; i <= m; i++) {
    volume = volume / (double)(i);
  }

  delete[] b;
  delete[] pivot;

  return volume;
}
//****************************************************************************80

double simplex_unit_monomial_integral(int m, int expon[])

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_UNIT_MONOMIAL_INTEGRAL integrates a monomial over a simplex.
//
//  Discussion:
//
//    This routine evaluates a monomial of the form
//
//      product ( 1 <= dim <= m ) x(dim)^expon(dim)
//
//    where the exponents are nonnegative integers.  Note that
//    if the combination 0^0 is encountered, it should be treated
//    as 1.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 July 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the spatial dimension.
//
//    Input, int EXPON[M], the exponents.
//
//    Output, double SIMPLEX_UNIT_MONOMIAL_INTEGRAL, the value of the integral
//    of the monomial.
//
{
  int    dim;
  int    i;
  int    k;
  double value;

  value = 1.0;

  k = 0;

  for (dim = 0; dim < m; dim++) {
    for (i = 1; i <= expon[dim]; i++) {
      k     = k + 1;
      value = value * (double)(i) / (double)(k);
    }
  }

  for (dim = 0; dim < m; dim++) {
    k     = k + 1;
    value = value / (double)(k);
  }

  return value;
}
//****************************************************************************80

double simplex_unit_monomial_quadrature(int m, int expon[], int n, double x[],
                                        double w[])

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_UNIT_MONOMIAL_QUADRATURE: quadrature of monomials in a unit
//    simplex.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 July 2007
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the spatial dimension.
//
//    Input, int EXPON[M], the exponents.
//
//    Input, int N, the number of points in the rule.
//
//    Input, double X[M*N], the quadrature points.
//
//    Input, double W[N], the quadrature weights.
//
//    Output, double SIMPLEX_UNIT_MONOMIAL_QUADRATURE, the quadrature error.
//
{
  double  exact = 1.0;
  double  quad;
  double  quad_error;
  double  scale;
  double *value;
  //
  //  Get the exact value of the integral of the unscaled monomial.
  //
  scale = simplex_unit_monomial_integral(m, expon);
  //
  //  Evaluate the monomial at the quadrature points.
  //
  value = monomial_value(m, n, expon, x);
  //
  //  Compute the weighted sum and divide by the exact value.
  //
  quad = r8vec_dot_product(n, w, value) / scale;

  delete[] value;
  //
  //  Error:
  //
  quad_error = fabs(quad - exact);

  return quad_error;
}
//****************************************************************************80

double *simplex_unit_sample(int m, int n, int &seed)

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_UNIT_SAMPLE returns uniformly random points from a general
//    simplex.
//
//  Discussion:
//
//    The interior of the unit M dimensional simplex is the set of
//    points X(1:M) such that each X(I) is nonnegative, and
//    sum(X(1:M)) <= 1.
//
//    This routine is valid for any spatial dimension M.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Reuven Rubinstein,
//    Monte Carlo Optimization, Simulation, and Sensitivity
//    of Queueing Networks,
//    Krieger, 1992,
//    ISBN: 0894647644,
//    LC: QA298.R79.
//
//  Parameters:
//
//    Input, int M, the dimension of the space.
//
//    Input, int N, the number of points.
//
//    Input/output, int &SEED, a seed for the random number generator.
//
//    Output, double UNIFORM_IN_SIMPLEX01_MAP[M*N], the points.
//
{
  double *e;
  int     i;
  int     j;
  double  total;
  double *x;
  //
  //  The construction begins by sampling M+1 points from the
  //  exponential distribution with parameter 1.
  //
  x = new double[m * n];

  for (j = 0; j < n; j++) {
    e = r8vec_uniform_01_new(m + 1, seed);

    for (i = 0; i <= m; i++) {
      e[i] = -log(e[i]);
    }

    total = 0.0;
    for (i = 0; i <= m; i++) {
      total = total + e[i];
    }

    for (i = 0; i < m; i++) {
      x[i + m * j] = e[i] / total;
    }
    delete[] e;
  }

  return x;
}
//****************************************************************************80

void simplex_unit_to_general(int m, int n, double t[], double ref[],
                             double phy[])

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_UNIT_TO_GENERAL maps the unit simplex to a general simplex.
//
//  Discussion:
//
//    Given that the unit simplex has been mapped to a general simplex
//    with vertices T, compute the images in T, under the same linear
//    mapping, of points whose coordinates in the unit simplex are REF.
//
//    The vertices of the unit simplex are listed as suggested in the
//    following:
//
//      (0,0,0,...,0)
//      (1,0,0,...,0)
//      (0,1,0,...,0)
//      (0,0,1,...,0)
//      (...........)
//      (0,0,0,...,1)
//
//    Thanks to Andrei ("spiritualworlds") for pointing out a mistake in the
//    previous implementation of this routine, 02 March 2008.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    02 March 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the spatial dimension.
//
//    Input, int N, the number of points to transform.
//
//    Input, double T[M*(M+1)], the vertices of the
//    general simplex.
//
//    Input, double REF[M*N], points in the
//    reference triangle.
//
//    Output, double PHY[M*N], corresponding points in the physical triangle.
//
{
  int i;
  int j;
  int k;
  //
  //  The image of each point is initially the image of the origin.
  //
  //  Insofar as the pre-image differs from the origin in a given vertex
  //  direction, add that proportion of the difference between the images
  //  of the origin and the vertex.
  //
  for (j = 0; j < n; j++) {
    for (i = 0; i < m; i++) {
      phy[i + j * m] = t[i + 0 * m];

      for (k = 1; k < m + 1; k++) {
        phy[i + j * m] =
            phy[i + j * m] + (t[i + k * m] - t[i + 0 * m]) * ref[k - 1 + j * m];
      }
    }
  }

  return;
}
//****************************************************************************80

double simplex_unit_volume(int m)

//****************************************************************************80
//
//  Purpose:
//
//    SIMPLEX_UNIT_VOLUME computes the volume of the unit simplex.
//
//  Discussion:
//
//    The formula is simple: volume = 1/M!.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    04 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, int M, the dimension of the space.
//
//    Output, double SIMPLEX_UNIT_VOLUME, the volume of the cone.
//
{
  int    i;
  double volume;

  volume = 1.0;
  for (i = 1; i <= m; i++) {
    volume = volume / ((double)i);
  }

  return volume;
}
//****************************************************************************80

void timestamp()

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    May 31 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
#define TIME_SIZE 40

  static char      time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t           len;
  time_t           now;

  now = time(NULL);
  tm  = localtime(&now);

  len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

  cout << time_buffer << "\n";

  return;
#undef TIME_SIZE
}

} // namespace simplex
