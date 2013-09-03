// IPlugin.h

#ifndef _PPBOX_PPBOX_I_PLUGIN_H_
#define _PPBOX_PPBOX_I_PLUGIN_H_

#include "IPpboxTypes.h"
#include "IFormat.h"

#include <stdarg.h>

PPBOX_STRUCT_0(PPBOX_Plugin, ());

PPBOX_CALLBACK_1(PPBOX_Plugin *, PPBOX_PluginCreate, (va_list));

PPBOX_CALLBACK_1(void, PPBOX_PluginDestroy, (PPBOX_Plugin *));

PPBOX_FUNC_4(PP_err, PPBBOX_PluginRegister, (
    (PP_str, factory), 
    (PP_str, name), 
    (PPBOX_PluginCreate, creator), 
    (PPBOX_PluginDestroy, destroyer)));

PPBOX_FUNC_2(PP_err, PPBBOX_PluginUnregister, (
    (PP_str, factory), 
    (PP_str, name)));

#endif // _PPBOX_PPBOX_I_PLUGIN_H_
