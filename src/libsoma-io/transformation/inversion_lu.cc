#include <soma-io/transformation/affinetransformation3d_base.h>

using namespace soma;
using namespace carto;
using namespace std;

namespace soma
{

float
decompositionLU( AffineTransformationBase::Table<float> &a,
                 AffineTransformationBase::Table<int32_t> &indx)
{
  int   i,imax=0,j,k,n;
  float big,dum,sum,temp,TINY=1.0e-20,d;

  n = a.dimX();

  std::vector<float> vv(n);

  d = 1.0;

  for (i=0;i<n;i++)
  { big = 0.0;
    for (j=0;j<n;j++)
      if ((temp = fabs(a(i,j))) > big)  big = temp;
    vv[i] = 1.0 / big;
  }
  for (j=0;j<n;j++)
  { for (i=0;i<j;i++)
    { sum = a(i,j);
      for (k=0;k<i;k++)  sum -= a(i,k) * a(k,j);
      a(i,j) = sum;
    }

    big = 0.0;
    for (i=j;i<n;i++)
    { sum = a(i,j);
      for (k=0;k<j;k++)  sum -= a(i,k) * a(k,j);
      a(i,j) = sum;
      if ((dum = vv[i] * fabs(sum)) >= big)
      { big  = dum;
        imax = i;
      }
    }

    if (j != imax)
    { for (k=0;k<n;k++)
      { dum = a(imax,k);
        a(imax,k) = a(j,k);
        a(j,k)    = dum;
      }
      d = -d;
      vv[imax] = vv[j];
    }
    indx[j] = imax;
    if (a(j,j) == 0.0)  a(j,j) = TINY;
    if (j != n)
    { dum = 1.0 / a(j,j);
      for (i=j+1;i<n;i++)  a(i,j) *= dum;
    }
  }
  return d;
}


void backSubstitutionLU( AffineTransformationBase::Table<float> &a,
                         AffineTransformationBase::Table<int32_t> &indx,
                         AffineTransformationBase::Table<float> &b)
{
  int   i,ii=-1,ip,j,n;
  float sum;

  n = a.dimX();

  for (i=0;i<n;i++)
  { ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if (ii != -1)
      for (j=ii;j<=i-1;j++)  sum -= a(i,j) * b[j];
    else if (sum!=0)
      ii = i;
    b[i] = sum;
  }
  for (i=n;i--;)
  { sum = b[i];
    for (j=i+1;j<n;j++)  sum -= a(i,j) * b[j];
    b[i] = sum / a(i,i);
  }
}


AffineTransformationBase::Table<float>
inversionLU( const AffineTransformationBase::Table<float> &matrix )
{
  int                 n,i,j;
  //float d;

  n = matrix.dimX();

  AffineTransformationBase::Table<float> inverse(n,n);
  AffineTransformationBase::Table<float> stockage(n,n);

  for (j=n;j--;)
    for (i=n;i--;)  stockage(i,j) = matrix(i,j);

  AffineTransformationBase::Table<float> column(1,n);

  AffineTransformationBase::Table<int32_t>  indx(1,n);

  /*d =*/ decompositionLU(stockage,indx);

  for (j=0;j<n;j++)
  { for (i=0;i<n;i++)  column[i] = 0.0;
    column[j] = 1.0;
    backSubstitutionLU(stockage,indx,column);
    for (i=0;i<n;i++)  inverse(i,j) = column[i];
  }
  return inverse;
}

}

