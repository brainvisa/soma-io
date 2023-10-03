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

//--- plugin -----------------------------------------------------------------
#include <soma-io/checker/mifformatchecker.h> // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/nifticlib/znzlib/znzlib.h>
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfoloader.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/datasource/datasourcelist.h>
#include <soma-io/datasource/filedatasource.h>  // because we use file sources
#include <soma-io/transformation/affinetransformation3d_base.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                                 // header
#include <cartobase/object/property.h>                               // header
#include <cartobase/stream/fileutil.h>             // to manipulate file names
#include <cartobase/type/byte_order.h>
//--- system -----------------------------------------------------------------
#include <cstdio>
#include <limits>
#include <locale>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii -> used for byteswap
//--- Boost! -----------------------------------------------------------------
#include <boost/algorithm/string.hpp>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "MIFFORMATCHECKER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

#ifdef CARTO_DEBUG
constexpr bool mif_debug_enabled = true;
#else
constexpr bool mif_debug_enabled = false;
#endif

//============================================================================
//   P R I V A T E   M E T H O D S
//============================================================================
/*** BUILDING DATASOURCELIST *************************************************
 * This method builds a DataSourceList from the initial datasource.
 * It tries to find .mif, .mif.gz, .mih, and .minf files
 * If one or several of those files doesn't exist, it writes the initial
 * DataSource instead. Thus, the list returned contains at least one ds for
 * each of the following keywords : "header", "data", "minf", "default", but
 * in the worst case they can all be the initial ds.
 ****************************************************************************/
void MifFormatChecker::_buildDSList( DataSourceList & dsl ) const
{
  rc_ptr<DataSource> pds = dsl.dataSource();
  string headername, minfname, basename;
  // headername is the name of the file containing the MIF header (.mif, .mif.gz, or .mih)

  headername = FileUtil::uriFilename( pds->url() );
  if( headername.empty() )
  {
    // we suppose ds is an uncompressed mif file
    dsl.addDataSource( "header", pds );
  }
  else
  {
    string ext = FileUtil::extension( headername );
    basename = FileUtil::removeExtension( headername );

    if( ext == "mif" )
    {
    }
    else if( ext == "mih" )
    {
    }
    else if( ext == "gz" )
    {
      string ext2 = FileUtil::extension( basename );
      if( ext2 == "mif" )
      {
        ext = "mif.gz";
        basename = FileUtil::removeExtension( basename );
      }
      else
      {
        // probably a problem... but we'll try like this
        ext.clear();
      }
    }
    else
    {
      // probably a problem... but we'll try like this
      ext.clear();
      basename = headername;
    }

    if( ext.empty() )
    {
      if( FileUtil::fileStat( headername ).find( '+' ) != string::npos )
      {
      }
      else if( FileUtil::fileStat( headername + ".mif" ).find( '+' )
        != string::npos )
      {
        headername = basename + ".mif";
      }
      else if( FileUtil::fileStat( headername + ".mif.gz" ).find( '+' )
        != string::npos )
      {
        headername = basename + ".mif.gz";
      }
      else if( FileUtil::fileStat( headername + ".mih" ).find( '+' )
        != string::npos )
      {
        headername = basename + ".mih";
      }
    }

    minfname = headername + ".minf";

    if( headername == pds->url() )
    {
      // if hdrname is original url
      dsl.addDataSource( "header", pds );
    }
    else
    {
      if( !headername.empty()
        && FileUtil::fileStat( headername ).find( '+' ) != string::npos )
      {
        // if header file exists
        dsl.addDataSource( "header", rc_ptr<DataSource>
            ( new FileDataSource( headername ) ) );
      }
      else
      {
        // we suppose original url contains header
        dsl.addDataSource( "header", pds );
      }
    }


  }
  localMsg( "header: " + dsl.dataSource( "header" )->url() );

  // Minf DataSource
  if( !minfname.empty()
      && FileUtil::fileStat( minfname ).find( '+' ) != string::npos )
  {
    // if minf file exist
    dsl.addDataSource( "minf", rc_ptr<DataSource>
        ( new FileDataSource( minfname ) ) );
  } else {
    // we suppose original url is minf
    dsl.addDataSource( "minf", pds );
  }

  localMsg( "minf: " + dsl.dataSource( "minf" )->url() );
}


namespace {  // for file-local helper functions

string get_unique_mif_header_field(const unordered_map<string, list<string>> &raw_header,
                                   const string& field_name, const string& filename)
{
  auto header_it = raw_header.find(field_name);
  if(header_it == end(raw_header)) {
    throw invalid_format_error( "Invalid MIF image: missing " + field_name + " header field",
                                filename );
  }
  if(header_it->second.size() != 1) {
    throw invalid_format_error( "Invalid MIF image: multiple " + field_name + " header fields",
                                filename );
  }
  return header_it->second.front();
}


template<typename Element>
vector<Element> parse_comma_separated_string(const string& str,
                                             const string& field_name,
                                             const string& filename)
{
  vector<Element> result;

  vector<string> split_str;
  boost::split(split_str, str, [](char c) { return c == ','; });
  Element element;
  for(auto it = begin(split_str); it != end(split_str); ++it) {
    boost::trim(*it);
    std::istringstream ss(*it);
    ss.imbue(locale::classic());
    ss >> element;
    if(ss.fail()) {
      boost::to_lower(*it, locale::classic());
      if(*it == "nan") {
        result.push_back(std::numeric_limits<Element>::quiet_NaN());
      } else {
        throw invalid_format_error("Invalid MIF header: cannot parse field "
                                   + field_name, filename);
      }
    }
    result.push_back(element);
  }
  return result;
}


template<typename Element>
vector<Element> parse_comma_separated_field(const unordered_map<string, list<string>> &raw_header,
                                            const string& field_name, const string& filename)
{
  string raw_value = get_unique_mif_header_field(raw_header, field_name, filename);
  return parse_comma_separated_string<Element>(raw_value, field_name, filename);
}

class ZnzWrapper
{
public:
  ZnzWrapper(znzFile fp) : _fp(fp), _eof(false) {};
  ~ZnzWrapper()
  {
    if(!znz_isnull(_fp)) {
      znzclose(_fp);
      _fp = nullptr;
    }
  }

  operator znzFile () { return _fp; };
  string getline()
  {
    string ret;
    int c = znzgetc(_fp);
    while( c != EOF && c != '\n' && c != '\r' )
    {
      ret += static_cast<char>(c);
      c = znzgetc(_fp);
    }
    if(c == EOF) {
      _eof = true;
    }
    return ret;
  }
  bool eof() const { return _eof; };

private:
  znzFile _fp;
  bool _eof;
};


} // end of anonymous namespace for file-local helper functions


/*** BUILDING HEADER *********************************************************
 * This method builds a header from a .mif / .mih DataSource.
 * The argument is given by check(...) and is supposed to be a MIF/MIH file.
 ****************************************************************************/
Object MifFormatChecker::_buildHeader( DataSource& hds ) const
{
  const string fname = FileUtil::uriFilename(hds.url());
  bool gzip_compressed = false;
  if(fname.length() >= 3 && fname.substr(fname.length() - 3) == ".gz") {
    gzip_compressed = true;
  }

  ZnzWrapper fp(znzopen(fname.c_str(), "rb", gzip_compressed));
  if(znz_isnull(fp)) {
    io_error::launchErrnoExcept( fname );
  }

  Object hdr = Object::value( PropertySet() );  // AIMS header

  // Raw MIF header key-value pairs (string-valued, with their original name).
  // A key can be repeated and take multiple values, and order of these
  // same-key values is important.
  unordered_map<string, list<string>> raw_header;

  string line = fp.getline();

  if(line.substr(0, 12) != "mrtrix image")
    throw wrong_format_error( "Not an mrtrix MIF image", hds.url() );

  /* Read all header fields and store them ***********************************/
  while(line != "END" && !fp.eof()) {
    line = fp.getline();
    const size_t colon_pos = line.find(':');
    if(colon_pos != string::npos) {
      const string key = line.substr(0, colon_pos);
      string raw_value = line.substr(colon_pos + 1);
      boost::trim(raw_value, locale::classic());
      auto element = raw_header.find(key);
      if(element == end(raw_header)) {
        raw_header.insert({move(key), {move(raw_value)}});
      } else {
        // Append to the existing list
        element->second.push_back(move(raw_value));
      }
    } else {
      // No colon on that line, must be END
      boost::trim(line, locale::classic());
      if(line != "END" && !fp.eof()) {
        throw invalid_format_error("invalid line in MIF header");
      }
    }
  }

  /***************************/
  /* MANDATORY HEADER FIELDS */
  /***************************/

  vector<int> dims = parse_comma_separated_field<int>(raw_header, "dim", hds.url());
  vector<float> vs = parse_comma_separated_field<float>(raw_header, "vox", hds.url());
  vector<string> layout = parse_comma_separated_field<string>(raw_header, "layout", hds.url());
  string datatype = get_unique_mif_header_field(raw_header, "datatype", hds.url());
  string file_and_offset = get_unique_mif_header_field(raw_header, "file", hds.url());

  /* File and offset */
  size_t data_offset;
  string mif_data_file;
  auto last_space_it = find_if(rbegin(file_and_offset), rend(file_and_offset),
                               boost::is_space(locale::classic()));
  if(last_space_it != rend(file_and_offset)) {
    // if there is a space
    size_t last_space_pos = rend(file_and_offset) - last_space_it - 1;
    mif_data_file = file_and_offset.substr(0, last_space_pos);
    boost::trim(mif_data_file, locale::classic());
    std::istringstream ss(file_and_offset.substr(last_space_pos + 1));
    ss.imbue(locale::classic());
    ss >> data_offset;
    if(ss.fail()) {
      mif_data_file = file_and_offset;
      data_offset = 0;
    }
  }
  if(mif_data_file.empty()) {
    // if there is no space
    mif_data_file = file_and_offset;
    boost::trim(mif_data_file, locale::classic());
    if(mif_data_file == ".") {
      // Data offset is mandatory for single-file data
      throw invalid_format_error("Invalid MIF header: invalid 'file' field", hds.url());
    } else if(mif_data_file.empty()) {
      // Empty filename is not allowed
      throw invalid_format_error("Invalid MIF header: invalid 'file' field", hds.url());
    } else {
      data_offset = 0;
    }
  }
  hdr->setProperty("_mif_data_file", mif_data_file);
  hdr->setProperty("_mif_data_offset", data_offset);

  /* Dimensions of the image */
  // same in MIF header (RAS+ convention) as in AIMS header (LPI+ convention)
  size_t ndims = dims.size();
  while( dims.size() < 4 ) // complete to 4D
    dims.push_back( 1 );
  hdr->setProperty( "sizeX", dims[0] );
  hdr->setProperty( "sizeY", dims[1] );
  hdr->setProperty( "sizeZ", dims[2] );
  hdr->setProperty( "sizeT", dims[3] );
  hdr->setProperty( "volume_dimension", dims );

  /* Voxel size */
  for(int i=vs.size(); i<3; ++i)
    vs.push_back(1.0f);
  if(vs.size() >= 4) {
    // 4th dimension of vox is assumed to be TR
    hdr->setProperty( "tr", vs[3] );
  }
  if(vs.size() > 4) {
    clog << "MIF image reader: warning: discarding values of vox beyond the 4th" << endl;
  }
  vs.resize(3);
  hdr->setProperty( "voxel_size", vs );  // same in MIF header (RAS+ convention) as in AIMS header (LPI+ convention)

  /* Data type */
  boost::to_lower(datatype, locale::classic());
  string type;
  int sz;
  string endian;
  if(datatype == "bit") {
    type = "U8";
    sz = 1;
  } else if(datatype == "int8") {
    type = "S8";
    sz = 8;
  } else if(datatype == "uint8") {
    type = "U8";
    sz = 8;
  } else if(boost::starts_with(datatype, "int16")) {
    type = "S16";
    sz = 16;
    endian = datatype.substr(5);
  } else if(boost::starts_with(datatype, "uint16")) {
    type = "U16";
    sz = 16;
    endian = datatype.substr(6);
  } else if(boost::starts_with(datatype, "int32")) {
    type = "S32";
    sz = 32;
    endian = datatype.substr(5);
  } else if(boost::starts_with(datatype, "uint32")) {
    type = "U32";
    sz = 32;
    endian = datatype.substr(6);
  } else if(boost::starts_with(datatype, "float32")) {
    type = "FLOAT";
    sz = 32;
    endian = datatype.substr(7);
  } else if(boost::starts_with(datatype, "float64")) {
    type = "DOUBLE";
    sz = 64;
    endian = datatype.substr(7);
  } else if(boost::starts_with(datatype, "cfloat32")) {
    type = "CFLOAT";
    sz = 64;
    endian = datatype.substr(8);
  } else if(boost::starts_with(datatype, "cfloat64")) {
    type = "CDOUBLE";
    sz = 128;
    endian = datatype.substr(8);
  } else {
    throw datatype_format_error("Unrecognized MIF data type "
                                + datatype, hds.url());
  }
  bool bswap;
  if(endian.empty()) {
    bswap = false; // native endianness
  } else if(endian == "le") {
    bswap = (std::string(byteOrderString()) != "DCBA" /* little endian */);
  } else if(endian == "be") {
    bswap = (std::string(byteOrderString()) != "ABCD" /* big endian */);
  } else {
    throw datatype_format_error("Unrecognized MIF data type endianness " + datatype,
                                hds.url());
  }
  localMsg("type: " + type + ", sz: " + carto::toString(sz));

  hdr->setProperty("disk_data_type", type);
  hdr->setProperty("bits_allocated", sz);
  hdr->setProperty("byte_swapping", bswap);

  /* Layout */
  // The 'layout' MIF header field determines the relationship from the on-disk
  // data order to anatomical axes. This can be seen as a way of defining strides.
  //
  // MIF axes are RAS+, AIMS axes have the same direction but are flipped
  // (LPI+). As a result, we can simply flip the inversion flag (+/- prefix in
  // the layout field) but consider the axis indices as column indices into the
  // storage_to_memory matrix.
  const size_t storage_to_memory_order = ndims;  // FIXME: or 3????
  AffineTransformationBase storage_to_lpi(ndims);
  if(layout.size() != ndims) {
    throw invalid_format_error("Invalid MIF file: layout vs dim inconsistency", hds.url());
  }
  vector<int> readable_data_layout(ndims); // As displayed by mrinfo (1-based)
  for(size_t i = 0; i < ndims; ++i) {
    string element = layout[i];
    bool invert_to_ras, invert_to_lpi;
    if(element[0] == '-') {
      invert_to_ras = true;
      element = element.substr(1);
    } else if(element[1] == '+') {
      invert_to_ras = false;
      element = element.substr(1);
    } else {
      invert_to_ras = false;  // assume +
    }
    int axis_index;
    {
      std::istringstream ss(element);
      ss.imbue(locale::classic());
      ss >> axis_index;
      if(ss.fail()) {
        throw invalid_format_error("Invalid MIF header: invalid 'layout' field", hds.url());
      }
    }
    if(axis_index < 0 || static_cast<std::size_t>(axis_index) >= ndims) {
      throw invalid_format_error("Invalid MIF file: invalid 'layout' field", hds.url());
    }
    readable_data_layout[i] = invert_to_ras ? -(axis_index + 1) : (axis_index + 1);
    if(i < 3) {
      invert_to_lpi = !invert_to_ras;
    } else {
      invert_to_lpi = invert_to_ras;
    }
    if(i < storage_to_memory_order) {
      storage_to_lpi.matrix()(i, i) = 0;
      if(static_cast<std::size_t>(axis_index) < storage_to_memory_order) {
        storage_to_lpi.matrix()(i, axis_index) = invert_to_lpi ? -1.0 : 1.0;
      }
      if(invert_to_lpi) {
        storage_to_lpi.matrix()(i, storage_to_memory_order) = dims[i] - 1;
      }
    }
  }
  hdr->setProperty( "mif_data_layout", readable_data_layout );
  hdr->setProperty( "storage_to_memory", storage_to_lpi.toVector() );

  /**************************/
  /* OPTIONAL HEADER FIELDS */
  /**************************/

  string dt = type;
  /* Scaling parameter: slope, intercept */
  auto header_it = raw_header.find("scaling");
  if(header_it != end(raw_header)) {
    vector<float> scaling_params = parse_comma_separated_field<float>(raw_header, "scaling", hds.url());
    if(scaling_params.size() != 2) {
      throw invalid_format_error("Invalid MIF header: invalid scaling", hds.url());
    }
    float offset = scaling_params[0];
    float slope = scaling_params[1];
    if ( slope != 0.0 )
    {
      hdr->setProperty( "scale_factor", slope );
      hdr->setProperty( "scale_offset", offset );
      if( type != "DOUBLE" )
        dt = "FLOAT";
    }
    hdr->setProperty( "scale_factor_applied", false );
  }

  // Data type
  hdr->setProperty( "data_type", dt );
  vector<string> vt;
  vt.push_back( dt );
  if( dt != type )
    vt.push_back( type );
  if( dt != "FLOAT" )
    vt.push_back( "FLOAT" );
  if( dt != "DOUBLE" )
    vt.push_back( "DOUBLE" );
  hdr->setProperty( "possible_data_types", vt );

  header_it = raw_header.find("transform");
  if(header_it != end(raw_header)) {
    vector<float> mif_transform_vec;
    mif_transform_vec.reserve(12);
    // Usually there are 3 instances of the transform header, one for each row
    // of the matrix, in row-major order.
    for(auto list_it = begin(header_it->second);
        list_it != end(header_it->second);
        ++list_it) {
      string raw_row = *list_it;
      vector<float> line_coefs = parse_comma_separated_string<float>(raw_row, "transform", hds.url());
      copy(begin(line_coefs), end(line_coefs), back_insert_iterator<vector<float>>(mif_transform_vec));
    }
    if(mif_transform_vec.size() != 12) {
      throw invalid_format_error("Invalid MIF header: transform should have exactly 12 coefficients", hds.url());
    }
    mif_transform_vec.push_back(0);
    mif_transform_vec.push_back(0);
    mif_transform_vec.push_back(0);
    mif_transform_vec.push_back(1);

    // The MIF transform goes from millimetres in RAS+ convention to the target
    // physical coordinates. The AIMS transform goes from millimetres in LPI+
    // convention to the target physical coordinates. Therefore, we have to
    // invert every axis.
    AffineTransformation3dBase aims_transform = AffineTransformation3dBase(mif_transform_vec)
      * AffineTransformation3dBase(vector<float>{
          -1, 0, 0, (dims[0] - 1) * vs[0],
          0, -1, 0, (dims[1] - 1) * vs[1],
          0, 0, -1, (dims[2] - 1) * vs[2],
          0, 0, 0, 1
    });
    vector<vector<float>> transformations;
    // MIF does not specify the referential, scanner-based is probably the best
    // guess (i.e. correct in many cases, not too wrong otherwise).
    transformations.push_back(aims_transform.toVector());
    hdr->setProperty( "transformations", transformations );
    vector<string> referentials;
    referentials.push_back("Scanner-based anatomical coordinates");
    hdr->setProperty( "referentials", referentials );
  }

  hdr->setProperty( "object_type", string( "Volume" ) );
  hdr->setProperty( "format", formatName() );

  return hdr;
}


//============================================================================
//   P U B L I C   M E T H O D S
//============================================================================

MifFormatChecker::MifFormatChecker()
  : FormatChecker()
{
}


MifFormatChecker::~MifFormatChecker()
{
}

DataSourceInfo MifFormatChecker::check( DataSourceInfo dsi,
                                        DataSourceInfoLoader & /* f */,
                                        Object options ) const
{
  bool doread = dsi.header().isNone() ;
  bool dolist = dsi.list().typecount() == 1 ;
  bool docapa = !dsi.capabilities().isInit();

  //--- read uri -------------------------------------------------------------
  // we don't use any options right now, but we may later ?
  // then, uncomment the following lines to get options from the uri.
//   std::string uri = dsi.list().dataSource( "default", 0 )->url();
//   carto::Object urioptions = FileUtil::uriOptions( uri );
//   if( urioptions.get() ) {
//     options->copyProperties( urioptions );
//   }

  //--- test header format ---------------------------------------------------
  if( !doread )
    if( !dsi.header()->hasProperty( "format" )
        || dsi.header()->getProperty( "format" )->getString() != formatName() )
      throw wrong_format_error( string( "Not a " ) + formatName() + " header",
                                dsi.list().dataSource()->url() );

  //--- build datasourcelist -------------------------------------------------
  if( dolist )
  {
    localMsg( "Building list..." );
    _buildDSList( dsi.list() );
  }
  //--- build header ---------------------------------------------------------
  if( doread || dolist )
  {
    localMsg( "Reading header..." );
    DataSource* hds = 0;
    try
    {
      hds = dsi.list().dataSource( "header" ).get();
    }
    catch(...)
    {
    }
    if( !hds )
      return dsi;

    dsi.header() = _buildHeader( *hds );

    string dataname;
    dsi.header()->getProperty("_mif_data_file", dataname);
    if(dataname == ".") {
      dsi.list().addDataSource( "data", dsi.list().dataSource( "header" ) );
    } else {
      dataname = FileUtil::basename(dataname); // For security, only allow filenames in the same directory
      dataname = FileUtil::dirname(FileUtil::uriFilename(hds->url()))
        + FileUtil::separator() + dataname;
      dsi.list().addDataSource("data", rc_ptr<DataSource>(new FileDataSource(dataname)));
    }

    localMsg( "data: " + dsi.list().dataSource( "data" )->url() );

    dsi.privateIOData()->setProperty("_mif_data_layout",
      dsi.header()->getProperty("mif_data_layout"));
    dsi.privateIOData()->setProperty("_mif_data_offset",
      dsi.header()->getProperty("_mif_data_offset"));
    dsi.privateIOData()->setProperty("_mif_byteswap",
      dsi.header()->getProperty("byte_swapping"));
    if(!mif_debug_enabled) {
      dsi.header()->removeProperty("_mif_data_offset");
      dsi.header()->removeProperty("_mif_data_file");
    }

    localMsg( "Reading MINF..." );
    string obtype = dsi.header()->getProperty( "object_type" )->getString();
    string dtype;
    dsi.header()->getProperty( "data_type", dtype );
    DataSource* minfds = dsi.list().dataSource( "minf" ).get();
    DataSourceInfoLoader::readMinf( *minfds, dsi.header(), options );
    localMsg( "MINF read" );
    vector<int> dims( 4, 1 );
    if( dsi.header()->getProperty( "volume_dimension", dims ) )
      if( dims.size() < 4 )
      {
        dims.resize( 4, 1 );
        dsi.header()->setProperty( "volume_dimension", dims );
      }
    dsi.header()->setProperty( "object_type", obtype );
    if( !dtype.empty() )
      dsi.header()->setProperty( "data_type", dtype );
  }
  //--- write capabilities ---------------------------------------------------
  if( docapa )
  {
    localMsg ("Writing capabilities..." );
    dsi.capabilities().setMemoryMapping( false );
    //dsi.capabilities().setDataSource( dsi.list().dataSource( "data" ) );  // ?? maybe for memory mapping?
    dsi.capabilities().setThreadSafe( false ); /* TODO */
    dsi.capabilities().setOrdered( true ); // FIXME: what does that mean??
    dsi.capabilities().setSeekVoxel( false );
    dsi.capabilities().setSeekLine( false );
    dsi.capabilities().setSeekSlice( false );
    dsi.capabilities().setSeekVolume( false );
    dsi.capabilities().setRandomAccessEfficient( false );
    dsi.capabilities().setHandleStrides( true );
  }
  //--------------------------------------------------------------------------
  localMsg( "Checking done" );
  return dsi;
}

#undef localMsg
