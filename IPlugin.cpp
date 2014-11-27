// IPpbox.cpp

#include "just/just/Common.h"
#define JUST_SOURCE
#include "just/just/IPlugin.h"
#include "just/just/plugin/PluginSink.h"
using namespace just::error;

#include <boost/bind.hpp>

namespace just
{
    class PluginManager
    {
    public:
        PP_err register_plugin(
            PP_str factory, 
            PP_str name, 
            JUST_PluginCreate creator, 
            JUST_PluginDestroy destroyer)
        {
            util::stream::UrlSinkFactory::register_creator(
                name, 
                boost::bind(create_url_sink, _1, creator, destroyer));
            return just_success;
        }

        PP_err unregister_plugin(
            PP_str factory, 
            PP_str name)
        {
            return just_success;
        }

    };
}

static just::PluginManager & plugin_manager()
{
    static just::PluginManager the_plugin_manager;
    return the_plugin_manager;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err PPBBOX_PluginRegister(
        PP_str factory, 
        PP_str name, 
        JUST_PluginCreate creator, 
        JUST_PluginDestroy destroyer)
    {
        return plugin_manager().register_plugin(factory, name, creator, destroyer);
    }

    JUST_DECL PP_err PPBBOX_PluginUnregister(
        PP_str factory, 
        PP_str name)
    {
        return plugin_manager().unregister_plugin(factory, name);
    }

#if __cplusplus
}
#endif // __cplusplus
