// IPluginSink.h

#ifndef _JUST_JUST_PLUGIN_I_PLUGIN_SINK_H_
#define _JUST_JUST_PLUGIN_I_PLUGIN_SINK_H_

#include "../IPlugin.h"
#include "../IFormat.h"

JUST_INTERFACE(JUST_PluginSink, JUST_Plugin, 4, (
    JUST_METHOD(PP_bool, cancel, 1, (
        (JUST_Error, ec))),
    JUST_METHOD(PP_bool, set_non_block, 2, (
        (PP_bool, non_block), 
        (JUST_Error *, ec))),
    JUST_METHOD(PP_bool, set_time_out, 2, (
        (PP_uint, time_out), 
        (JUST_Error *, ec))),
    JUST_METHOD(PP_uint, private_write_some, 2, (
        (JUST_ConstBuffers const *, buffers), 
        (JUST_Error *, ec)))
))

JUST_INTERFACE(JUST_PluginUrlSink, JUST_PluginSink, 4, (
    JUST_METHOD(PP_bool, open, 4, (
        (JUST_Url const *, url), 
        (PP_ulong, beg), 
        (PP_ulong, end), 
        (JUST_Error *, ec))),
    JUST_METHOD(PP_bool, is_open, 1, (
        (JUST_Error *, ec))),
    JUST_METHOD(PP_bool, close, 1, (
        (JUST_Error *, ec))),
    JUST_METHOD(PP_bool, recoverable, 1, (
        (JUST_Error const *, ec)))
))

#endif // _JUST_JUST_PLUGIN_I_PLUGIN_SINK_H_
