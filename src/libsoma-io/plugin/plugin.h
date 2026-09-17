#ifndef SOMAIO_PLUGIN_PLUGIN_H
#define SOMAIO_PLUGIN_PLUGIN_H

#include <string>
#include <set>
#include <list>

namespace carto
{

  class Plugin
  {
    friend class PluginManager;

  public:
    Plugin();
    virtual ~Plugin();
    virtual std::string name() const;

  private:
    virtual void initialize();
  };

  class PluginManager
  {
  public:
    PluginManager();
    ~PluginManager();
    void registerPlugin( Plugin* p );
    void unregisterPlugin( Plugin* p );
    std::set<Plugin*> plugins() const;
    static PluginManager & singleton();

  private:
    struct PluginManagerPrivate;
    PluginManagerPrivate	*d;
  };


  class PluginLoader
  {
  public:
    /// load the complete pluginFiles list
    static void load( int verboseLevel = -1, bool forcereload = false );
    ///	load a specific plugin file (in Python format)
    static void loadPluginFile( const std::string & file, 
                                const std::string & version = "", 
			        int verboseLevel = 0 );
    struct PluginFile
    {
      PluginFile( const std::string & file, const std::string & ver = "" );
      bool operator == ( const PluginFile & pf ) const
      { return filename == pf.filename && version == pf.version; }
      std::string filename;
      std::string version;
      bool loaded;
    };
    /** static list of plugin files to be loaded.
        A plugin file version is normally the version of the corresponding 
        library. It is only a hint: any version actually specified in 
        the plugin file itself overrides this version. If no version is 
        provided here, libs in the plugin file are loaded as is.
     */
    static std::list<PluginFile> & pluginFiles();
  };

}

#endif

