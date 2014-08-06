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

