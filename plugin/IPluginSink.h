// IPluginSink.h

#ifndef _PPBOX_PPBOX_PLUGIN_I_PLUGIN_SINK_H_
#define _PPBOX_PPBOX_PLUGIN_I_PLUGIN_SINK_H_

#include "../IPlugin.h"
#include "../IFormat.h"

PPBOX_INTERFACE(PPBOX_PluginSink, PPBOX_Plugin, 1, (
    PPBOX_METHOD(PP_uint, private_write_some, 2, (
        (PPBOX_ConstBuffers const *, buffers), 
        (PPBOX_Error *, ec)))
))

PPBOX_INTERFACE(PPBOX_PluginSinkBase, PPBOX_PluginSink, 3, (
    PPBOX_METHOD(PPBOX_Error, cancel, 1, (
        (PPBOX_Error *, ec))),
    PPBOX_METHOD(PPBOX_Error, set_non_block, 2, (
        (PP_bool, non_block), 
        (PPBOX_Error *, ec))),
    PPBOX_METHOD(PPBOX_Error, set_time_out, 2, (
        (PP_uint, time_out), 
        (PPBOX_Error *, ec)))
))

PPBOX_INTERFACE(PPBOX_PluginUrlSink, PPBOX_PluginSinkBase, 4, (
    PPBOX_METHOD(PPBOX_Error, open, 4, (
        (PPBOX_Url const *, url), 
        (PP_ulong, beg), 
        (PP_ulong, end), 
        (PPBOX_Error *, ec))),
    PPBOX_METHOD(PP_bool, is_open, 1, (
        (PPBOX_Error *, ec))),
    PPBOX_METHOD(PPBOX_Error, close, 1, (
        (PPBOX_Error *, ec))),
    PPBOX_METHOD(PP_bool, recoverable, 1, (
        (PPBOX_Error const *, ec)))
))

#endif // _PPBOX_PPBOX_PLUGIN_I_PLUGIN_SINK_H_
