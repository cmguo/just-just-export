// Plugin.h

#ifndef _PPBOX_PPBOX_PLUGIN_H_
#define _PPBOX_PPBOX_PLUGIN_H_

#include "ppbox/ppbox/IPlugin.h"

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/comma_if.hpp>

namespace ppbox
{

    template <typename T>
    class Plugin
    {
    protected:
        Plugin(
            PPBOX_PluginCreate creator, 
            PPBOX_PluginDestroy destroyer)
            : impl_(create_va(creator))
            , destroyer_(destroyer)
        {
        }

        ~Plugin()
        {
            if (impl_ && destroyer_) {
                destroyer_(impl_);
            }
        }

    protected:
        T const & impl() const
        {
            return *(T const *)impl_;
        }

        PPBOX_Plugin * pimpl() const
        {
            return impl_;
        }

    private:
        static PPBOX_Plugin * create_va(
            PPBOX_PluginCreate creator, 
            ...)
        {
            va_list args;
            va_start(args, creator); \
            PPBOX_Plugin * plugin = creator(args); \
            va_end(args);
            return plugin;
        }

    private:
        PPBOX_Plugin * impl_;
        PPBOX_PluginDestroy destroyer_;
    };

} // namespace ppbox

#endif // _PPBOX_PPBOX_PLUGIN_H_
