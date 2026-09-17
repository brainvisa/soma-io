#include <cartobase/config/paths.h>
#include <string>
#include <iostream>

using namespace carto;
using namespace std;

int main()
{
  string pfile = Paths::findResourceFile( "plugins/cartodata.plugins", "aims" );
  cout << "Plugin File: " << pfile << endl;
}
