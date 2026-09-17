#ifndef SOMAIO_CHECKER_NIFTIFORMATCHECKER_H
#define SOMAIO_CHECKER_NIFTIFORMATCHECKER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/checker/formatchecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;
  class DataSourceList;
  class DataSourceInfoLoader;
  class DataSourceInfo;
  class NiftiApiHelpers;

  /// FormatChecker for NIFTI-1 files.
  class NiftiFormatChecker : public FormatChecker
  {
  public:
    NiftiFormatChecker();
    virtual DataSourceInfo check( DataSourceInfo dsi,
                                  DataSourceInfoLoader & f,
                                  carto::Object options = carto::none() )
                                  const;
    virtual ~NiftiFormatChecker() = 0;

  protected:
    void _buildDSList( DataSourceList & dsl ) const;
    carto::Object _buildHeader( DataSource * hds ) const;
    void _readDiffusionVectors( DataSource* bvecfile, DataSource* bvalfile,
                                carto::Object header ) const;
    void setApi( NiftiApiHelpers *api );
    virtual std::string formatName() const = 0;
    NiftiApiHelpers *api;
  };


  class Nifti1FormatChecker : public NiftiFormatChecker
  {
  public:
    Nifti1FormatChecker();
    virtual ~Nifti1FormatChecker();

  protected:
    virtual std::string formatName() const { return "NIFTI-1"; }
  };


  class Nifti2FormatChecker : public NiftiFormatChecker
  {
  public:
    Nifti2FormatChecker();
    virtual ~Nifti2FormatChecker();

  protected:
    virtual std::string formatName() const { return "NIFTI-2"; }
  };


  class StandardReferentials
  {
  public:
    static std::string mniTemplateReferential();
    static std::string acPcReferential();
    static std::string talairachReferential();
    static std::string commonScannerBasedReferential();
    static std::string mniTemplateReferentialID();
    static std::string acPcReferentialID();
    static std::string commonScannerBasedReferentialID();
  };

  std::string NiftiReferential( int xform_code );
  int NiftiReferential( const std::string & ref );

}

#endif

