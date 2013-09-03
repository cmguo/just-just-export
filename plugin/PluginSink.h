// PluginSink.h

#ifndef _PPBOX_PPBOX_PLUGIN_PLUGIN_SINK_H_
#define _PPBOX_PPBOX_PLUGIN_PLUGIN_SINK_H_

#include "ppbox/ppbox/Plugin.h"

#include <ppbox/data/base/UrlSink.h>

namespace ppbox
{

    ppbox::data::UrlSink * create_url_sink(
        boost::asio::io_service & io_svc, 
        PPBOX_PluginCreate creator, 
        PPBOX_PluginDestroy destroyer);

}

#endif // _PPBOX_PPBOX_PLUGIN_PLUGIN_SINK_H_
