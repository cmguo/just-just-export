// PluginSink.cpp

#include "just/just/Common.h"
#include "just/just/detail/IInterface.h"
#include "just/just/plugin/PluginSink.h"
#include "just/just/plugin/IPluginSink.h"

#include <boost/bind.hpp>

#include "just/just/detail/CxxInterface.h"
#include "just/just/detail/CxxValueError.h"
#include "just/just/detail/CxxValueUrl.h"
#include "just/just/detail/CxxValueBuffer.h"

namespace just
{

    BIND_TYPE(size_t, PP_uint)

    class PluginUrlSink
        : public util::stream::UrlSink
        , Plugin<JUST_PluginUrlSink>
    {
    public:
        PluginUrlSink(
            boost::asio::io_service & io_svc, 
            JUST_PluginCreate creator, 
            JUST_PluginDestroy destroyer)
            : UrlSink(io_svc)
            , Plugin<JUST_PluginUrlSink>(creator, destroyer)
        {
        }

    public:
#undef _JUST_JUST_PLUGIN_I_PLUGIN_SINK_H_
#include "just/just/plugin/IPluginSink.h"
    };

    util::stream::UrlSink * create_url_sink(
        boost::asio::io_service & io_svc, 
        JUST_PluginCreate creator, 
        JUST_PluginDestroy destroyer)
    {
        return new PluginUrlSink(io_svc, creator, destroyer);
    }

}
