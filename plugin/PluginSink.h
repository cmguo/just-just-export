// PluginSink.h

#ifndef _JUST_EXPORT_PLUGIN_PLUGIN_SINK_H_
#define _JUST_EXPORT_PLUGIN_PLUGIN_SINK_H_

#include "just/export/Plugin.h"

#include <util/stream/UrlSink.h>

namespace just
{

    util::stream::UrlSink * create_url_sink(
        boost::asio::io_service & io_svc, 
        JUST_PluginCreate creator, 
        JUST_PluginDestroy destroyer);

}

#endif // _JUST_EXPORT_PLUGIN_PLUGIN_SINK_H_
