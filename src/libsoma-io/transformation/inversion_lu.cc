/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#include <soma-io/transformation/affinetransformation3d_base.h>

using namespace soma;
using namespace carto;
using namespace std;

namespace soma
{

float
decompositionLU( AffineTransformation3dBase::Table<float> &a,
                 AffineTransformation3dBase::Table<int32_t> &indx)
{
  int   i,imax=0,j,k,n;
  float big,dum,sum,temp,TINY=1.0e-20,d;

  n = a.dimX();

  AffineTransformation3dBase::Table<float> vv(1,n);

  d = 1.0;

  for (i=0;i<n;i++)
  { big = 0.0;
    for (j=0;j<n;j++)
      if ((temp = fabs(a(i,j))) > big)  big = temp;
    vv(i) = 1.0 / big;
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
      if ((dum = vv(i) * fabs(sum)) >= big)
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
      vv(imax) = vv(j);
    }
    indx(j) = imax;
    if (a(j,j) == 0.0)  a(j,j) = TINY;
    if (j != n)
    { dum = 1.0 / a(j,j);
      for (i=j+1;i<n;i++)  a(i,j) *= dum;
    }
  }
  return d;
}


void backSubstitutionLU( AffineTransformation3dBase::Table<float> &a,
                         AffineTransformation3dBase::Table<int32_t> &indx,
                         AffineTransformation3dBase::Table<float> &b)
{
  int   i,ii=-1,ip,j,n;
  float sum;

  n = a.dimX();

  for (i=0;i<n;i++)
  { ip = indx(i);
    sum = b(ip);
    b(ip) = b(i);
    if (ii != -1)
      for (j=ii;j<=i-1;j++)  sum -= a(i,j) * b(j);
    else if (sum!=0)
      ii = i;
    b(i) = sum;
  }
  for (i=n;i--;)
  { sum = b(i);
    for (j=i+1;j<n;j++)  sum -= a(i,j) * b(j);
    b(i) = sum / a(i,i);
  }
}


AffineTransformation3dBase::Table<float>
inversionLU( const AffineTransformation3dBase::Table<float> &matrix )
{
  int                 n,i,j;
  //float d;

  n = matrix.dimX();

  AffineTransformation3dBase::Table<float> inverse(n,n);
  AffineTransformation3dBase::Table<float> stockage(n,n);

  for (j=n;j--;)
    for (i=n;i--;)  stockage(i,j) = matrix(i,j);

  AffineTransformation3dBase::Table<float> column(1,n);

  AffineTransformation3dBase::Table<int32_t>  indx(1,n);

  /*d =*/ decompositionLU(stockage,indx);

  for (j=0;j<n;j++)
  { for (i=0;i<n;i++)  column(i) = 0.0;
    column(j) = 1.0;
    backSubstitutionLU(stockage,indx,column);
    for (i=0;i<n;i++)  inverse(i,j) = column(i);
  }
  return inverse;
}

}

