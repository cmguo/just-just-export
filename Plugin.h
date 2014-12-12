// Plugin.h

#ifndef _JUST_EXPORT_PLUGIN_H_
#define _JUST_EXPORT_PLUGIN_H_

#include "just/export/IPlugin.h"

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/comma_if.hpp>

namespace just
{

    template <typename T>
    class Plugin
    {
    protected:
        Plugin(
            JUST_PluginCreate creator, 
            JUST_PluginDestroy destroyer)
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

        JUST_Plugin * pimpl() const
        {
            return impl_;
        }

    private:
        static JUST_Plugin * create_va(
            JUST_PluginCreate creator, 
            ...)
        {
            va_list args;
            va_start(args, creator); \
            JUST_Plugin * plugin = creator(args); \
            va_end(args);
            return plugin;
        }

    private:
        JUST_Plugin * impl_;
        JUST_PluginDestroy destroyer_;
    };

} // namespace just

#endif // _JUST_EXPORT_PLUGIN_H_
