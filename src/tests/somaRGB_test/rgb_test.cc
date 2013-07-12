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

#include <cartobase/type/voxelrgb.h>
#include <cartobase/type/voxelrgba.h>
#include <cartobase/type/voxelhsv.h>
#include <cartobase/type/voxelvalue.h>
#include <soma-io/datasource/streamdatasource.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
#include <cartobase/exception/assert.h>
#include <iostream>

using namespace soma;
using namespace carto;
using namespace std;

int main()
{
  cout << "=====================================================================" << endl;
  cout << "                 V O X E L V A L U E   T E S T                       " << endl;
  cout << "=====================================================================" << endl;
  
  cout << "---|  TESTING sizes  |-----------------------------------------------" << endl;
  cout << "sizeof( VoxelValue<uint8_t,3> ) = " << sizeof( VoxelValue< uint8_t,3 > ) << endl;
  ASSERT( sizeof( VoxelValue< uint8_t,3 > ) == 3 );
  cout << "sizeof( VoxelValue<uint8_t,4> ) = " << sizeof( VoxelValue< uint8_t,4 > ) << endl;
  ASSERT( sizeof( VoxelValue< uint8_t,4 > ) == 4 );
  
  cout << "---|  TESTING creator()  |-------------------------------------------" << endl;
  cout << "> VoxelValue<uint8_t,3> vvdefault;" << endl;
  VoxelValue<uint8_t,3> vvdefault;
  cout << "vvdefault = ( " 
       << (int) vvdefault[0] << ", "
       << (int) vvdefault[1] << ", "
       << (int) vvdefault[2] << " )"
       << endl;
  cout << "> vvdefault[0] = 0;" << endl;
  vvdefault[0] = 0;
  cout << "> vvdefault[1] = 0;" << endl;
  vvdefault[1] = 0;
  cout << "> vvdefault[2] = 0;" << endl;
  vvdefault[2] = 0;
  cout << "vvdefault = ( " 
       << (int) vvdefault[0] << ", "
       << (int) vvdefault[1] << ", "
       << (int) vvdefault[2] << " )"
       << endl;
  ASSERT( vvdefault[0] == 0 && vvdefault[1] == 0 && vvdefault[2] == 0 );
  
  cout << "---|  TESTING creator( other )  |------------------------------------" << endl;
  cout << "> vvdefault[0] = 1;" << endl;
  vvdefault[0] = 1;
  cout << "> VoxelValue<uint8_t,3> vvother( vvdefault );" << endl;
  VoxelValue<uint8_t,3> vvother( vvdefault );;
  cout << "vvother = ( " 
       << (int) vvother[0] << ", "
       << (int) vvother[1] << ", "
       << (int) vvother[2] << " )"
       << endl;
  ASSERT( vvother[0] == 1 && vvother[1] == 0 && vvother[2] == 0 );
  
  cout << "---|  TESTING VV == VV  |--------------------------------------------" << endl;
  cout << "vvother == vvdefault : " << ( vvother == vvdefault ) << endl;
  ASSERT( vvother == vvdefault );
  cout << ">> vvother[1] = 2;" << endl;
  vvother[1] = 2;
  cout << "vvother == vvdefault : " << ( vvother == vvdefault ) << endl;
  ASSERT( vvother != vvdefault );
  
  cout << "---|  TESTING VV == const  |-----------------------------------------" << endl;
  cout << "> vvdefault[0] = 0;" << endl;
  vvdefault[0] = 0;
  cout << "vvdefault == 0 : " << ( vvdefault == 0 ) << endl;
  ASSERT( vvdefault == 0 );
  cout << "> vvdefault[1] = 2;" << endl;
  vvdefault[1] = 2;
  cout << "vvdefault == 0 : " << ( vvdefault == 0 ) << endl;
  ASSERT( !(vvdefault == 0) );
  
  cout << "---|  TESTING operator <<  |-----------------------------------------" << endl;
  cout << "> cout << vvdefault << endl;" << endl;
  cout << vvdefault << endl;
  
  cout << "---|  TESTING operator >>  |-----------------------------------------" << endl;
  cout << "> cin >> vvdefault;" << endl;
  cin >> vvdefault;
  cout << "vvdefault = ( " 
       << (int) vvdefault[0] << ", "
       << (int) vvdefault[1] << ", "
       << (int) vvdefault[2] << " )"
       << endl;
  
  cout << "---|  TESTING AsciiDataSourceTraits::write  |------------------------" << endl;
  cout << "> OStreamDataSource dsout( cout );" << endl;
  OStreamDataSource dsout( cout );
  cout << "AsciiDataSourceTraits< VoxelValue< uint8_t,3 > >::write( dsout, vvdefault );" << endl;
  AsciiDataSourceTraits< VoxelValue< uint8_t,3 > >::write( dsout, vvdefault );
  cout << endl;
  
  cout << "---|  TESTING AsciiDataSourceTraits::read  |-------------------------" << endl;
  cout << "> IStreamDataSource dsin( cin );" << endl;
  IStreamDataSource dsin( cin );
  cout << "AsciiDataSourceTraits< VoxelValue< uint8_t,3 > >::read( dsin, vvdefault );" << endl;
  AsciiDataSourceTraits< VoxelValue< uint8_t,3 > >::read( dsin, vvdefault );
  cout << "vvdefault = ( " 
       << (int) vvdefault[0] << ", "
       << (int) vvdefault[1] << ", "
       << (int) vvdefault[2] << " )"
       << endl;
  
  cout << "=====================================================================" << endl;
  cout << "                   V O X E L   R G B   T E S T                       " << endl;
  cout << "=====================================================================" << endl;
  
  cout << "---|  TESTING creator()  |-------------------------------------------" << endl;
  cout << ">> VoxelRGB voxel;" << endl;
  VoxelRGB voxel;
  cout << "voxel = ( " 
       << (int) voxel.red() << ", "
       << (int) voxel.green() << ", "
       << (int) voxel.blue() << " )"
       << endl;
  ASSERT( voxel.red() == 0 && voxel.green() == 0 && voxel.blue() == 0 );
  
  cout << "---|  TESTING creator( r,g,b )  |------------------------------------" << endl;
  cout << ">> VoxelRGB voxelrgb( 5, 10, 15 );" << endl;
  VoxelRGB voxelrgb( 5, 10, 15 );
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING creator( other )  |------------------------------------" << endl;
  cout << ">> VoxelRGB voxelcopy( voxelrgb );" << endl;
  VoxelRGB voxelcopy( voxelrgb );
  cout << "voxelcopy = ( " 
       << (int) voxelcopy.red() << ", "
       << (int) voxelcopy.green() << ", "
       << (int) voxelcopy.blue() << " )"
       << endl;
  ASSERT( voxelcopy.red() == 5 && voxelcopy.green() == 10 && voxelcopy.blue() == 15 );
  
  cout << "---|  TESTING operator =  |------------------------------------------" << endl;
  cout << ">> voxelcopy = voxelrgb;" << endl;
  voxelcopy = voxel;
  cout << "voxelcopy = ( " 
       << (int) voxelcopy.red() << ", "
       << (int) voxelcopy.green() << ", "
       << (int) voxelcopy.blue() << " )"
       << endl;
  ASSERT( voxelcopy.red() == 0 && voxelcopy.green() == 0 && voxelcopy.blue() == 0 );
  
  cout << "---|  TESTING operator +=  |-----------------------------------------" << endl;
  cout << ">> voxelrgb += VoxelRGB( 1, 2, 3 );" << endl;
  voxelrgb += VoxelRGB( 1, 2, 3 );
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 6 && voxelrgb.green() == 12 && voxelrgb.blue() == 18 );
  
  cout << "---|  TESTING operator -=  |-----------------------------------------" << endl;
  cout << ">> voxelrgb -= VoxelRGB( 1, 2, 3 );" << endl;
  voxelrgb -= VoxelRGB( 1, 2, 3 );
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator = const  |------------------------------------" << endl;
  cout << ">> voxelcopy = 3;" << endl;
  voxelcopy = 3;
  cout << "voxelcopy = ( " 
       << (int) voxelcopy.red() << ", "
       << (int) voxelcopy.green() << ", "
       << (int) voxelcopy.blue() << " )"
       << endl;
  ASSERT( voxelcopy.red() == 3 && voxelcopy.green() == 3 && voxelcopy.blue() == 3 );
  
  cout << "---|  TESTING operator += const  |-----------------------------------" << endl;
  cout << ">> voxelrgb += 1;" << endl;
  voxelrgb += 1;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 6 && voxelrgb.green() == 11 && voxelrgb.blue() == 16 );
  
  cout << "---|  TESTING operator -= const  |-----------------------------------" << endl;
  cout << ">> voxelrgb -= 1;" << endl;
  voxelrgb -= 1;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= uint8_t  |---------------------------------" << endl;
  cout << ">> voxelrgb *= (uint8_t)2;" << endl;
  voxelrgb *= (uint8_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= uint8_t  |---------------------------------" << endl;
  cout << ">> voxelrgb /= (uint8_t)2;" << endl;
  voxelrgb /= (uint8_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= uint16_t  |--------------------------------" << endl;
  cout << ">> voxelrgb *= (uint16_t)2;" << endl;
  voxelrgb *= (uint16_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= uint16_t  |--------------------------------" << endl;
  cout << ">> voxelrgb /= (uint16_t)2;" << endl;
  voxelrgb /= (uint16_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= uint32_t  |--------------------------------" << endl;
  cout << ">> voxelrgb *= (uint32_t)2;" << endl;
  voxelrgb *= (uint32_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= uint32_t  |--------------------------------" << endl;
  cout << ">> voxelrgb /= (uint32_t)2;" << endl;
  voxelrgb /= (uint32_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= uint64_t  |--------------------------------" << endl;
  cout << ">> voxelrgb *= (uint64_t)2;" << endl;
  voxelrgb *= (uint64_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= uint64_t  |--------------------------------" << endl;
  cout << ">> voxelrgb /= (uint64_t)2;" << endl;
  voxelrgb /= (uint64_t)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= float  |-----------------------------------" << endl;
  cout << ">> voxelrgb *= (float)2;" << endl;
  voxelrgb *= (float)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= float  |-----------------------------------" << endl;
  cout << ">> voxelrgb /= (float)2;" << endl;
  voxelrgb /= (float)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator *= double  |----------------------------------" << endl;
  cout << ">> voxelrgb *= (double)2;" << endl;
  voxelrgb *= (double)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 10 && voxelrgb.green() == 20 && voxelrgb.blue() == 30 );
  
  cout << "---|  TESTING operator /= double  |----------------------------------" << endl;
  cout << ">> voxelrgb /= (double)2;" << endl;
  voxelrgb /= (double)2;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 5 && voxelrgb.green() == 10 && voxelrgb.blue() == 15 );
  
  cout << "---|  TESTING operator (VoxelRGB + VoxelRGB) |-----------------------" << endl;
  cout << ">> voxelrgb = VoxelRGB(1,2,3)+VoxelRGB(3,2,1);" << endl;
  voxelrgb = VoxelRGB(1,2,3)+VoxelRGB(3,2,1);
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 4 && voxelrgb.green() == 4 && voxelrgb.blue() == 4 );
  
  cout << "---|  TESTING operator (VoxelRGB - VoxelRGB) |-----------------------" << endl;
  cout << ">> voxelrgb = VoxelRGB(1,2,3)-VoxelRGB(1,2,3);" << endl;
  voxelrgb = VoxelRGB(1,2,3)-VoxelRGB(1,2,3);
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 0 && voxelrgb.green() == 0 && voxelrgb.blue() == 0 );
  
  cout << "---|  TESTING operator (VoxelRGB + const) |--------------------------" << endl;
  cout << ">> voxelrgb = VoxelRGB(1,2,3)+1;" << endl;
  voxelrgb = VoxelRGB(1,2,3)+1;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 2 && voxelrgb.green() == 3 && voxelrgb.blue() == 4 );
  
  cout << "---|  TESTING operator (VoxelRGB - const) |--------------------------" << endl;
  cout << ">> voxelrgb = VoxelRGB(1,2,3)-1;" << endl;
  voxelrgb = VoxelRGB(1,2,3)-1;
  cout << "voxelrgb = ( " 
       << (int) voxelrgb.red() << ", "
       << (int) voxelrgb.green() << ", "
       << (int) voxelrgb.blue() << " )"
       << endl;
  ASSERT( voxelrgb.red() == 0 && voxelrgb.green() == 1 && voxelrgb.blue() == 2 );
  
  cout << "---------------------------------------------------------------------" << endl;
}
