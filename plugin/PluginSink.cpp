// PluginSink.cpp

#include "ppbox/ppbox/Common.h"
#include "ppbox/ppbox/detail/IInterface.h"
#include "ppbox/ppbox/plugin/PluginSink.h"
#include "ppbox/ppbox/plugin/IPluginSink.h"

#include <boost/bind.hpp>

#include "ppbox/ppbox/detail/CxxInterface.h"
#include "ppbox/ppbox/detail/CxxValueError.h"
#include "ppbox/ppbox/detail/CxxValueUrl.h"
#include "ppbox/ppbox/detail/CxxValueBuffer.h"

namespace ppbox
{

    BIND_TYPE(size_t, PP_uint)

    class PluginUrlSink
        : public util::stream::UrlSink
        , Plugin<PPBOX_PluginUrlSink>
    {
    public:
        PluginUrlSink(
            boost::asio::io_service & io_svc, 
            PPBOX_PluginCreate creator, 
            PPBOX_PluginDestroy destroyer)
            : UrlSink(io_svc)
            , Plugin<PPBOX_PluginUrlSink>(creator, destroyer)
        {
        }

    public:
#undef _PPBOX_PPBOX_PLUGIN_I_PLUGIN_SINK_H_
#include "ppbox/ppbox/plugin/IPluginSink.h"
    };

    util::stream::UrlSink * create_url_sink(
        boost::asio::io_service & io_svc, 
        PPBOX_PluginCreate creator, 
        PPBOX_PluginDestroy destroyer)
    {
        return new PluginUrlSink(io_svc, creator, destroyer);
    }

}
