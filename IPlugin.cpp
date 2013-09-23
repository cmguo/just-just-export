// IPpbox.cpp

#include "ppbox/ppbox/Common.h"
#define PPBOX_SOURCE
#include "ppbox/ppbox/IPlugin.h"
#include "ppbox/ppbox/plugin/PluginSink.h"
using namespace ppbox::error;

#include <boost/bind.hpp>

namespace ppbox
{
    class PluginManager
    {
    public:
        PP_err register_plugin(
            PP_str factory, 
            PP_str name, 
            PPBOX_PluginCreate creator, 
            PPBOX_PluginDestroy destroyer)
        {
            util::stream::UrlSink::register_creator(
                name, 
                boost::bind(create_url_sink, _1, creator, destroyer));
            return ppbox_success;
        }

        PP_err unregister_plugin(
            PP_str factory, 
            PP_str name)
        {
            return ppbox_success;
        }

    };
}

static ppbox::PluginManager & plugin_manager()
{
    static ppbox::PluginManager the_plugin_manager;
    return the_plugin_manager;
}

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBBOX_PluginRegister(
        PP_str factory, 
        PP_str name, 
        PPBOX_PluginCreate creator, 
        PPBOX_PluginDestroy destroyer)
    {
        return plugin_manager().register_plugin(factory, name, creator, destroyer);
    }

    PPBOX_DECL PP_err PPBBOX_PluginUnregister(
        PP_str factory, 
        PP_str name)
    {
        return plugin_manager().unregister_plugin(factory, name);
    }

#if __cplusplus
}
#endif // __cplusplus
