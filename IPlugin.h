// IPlugin.h

#ifndef _JUST_JUST_I_PLUGIN_H_
#define _JUST_JUST_I_PLUGIN_H_

#include "IPpboxTypes.h"
#include "IFormat.h"

#include <stdarg.h>

JUST_STRUCT_0(JUST_Plugin, ());

JUST_CALLBACK_1(JUST_Plugin *, JUST_PluginCreate, (va_list));

JUST_CALLBACK_1(void, JUST_PluginDestroy, (JUST_Plugin *));

JUST_FUNC_4(PP_err, PPBBOX_PluginRegister, (
    (PP_str, factory), 
    (PP_str, name), 
    (JUST_PluginCreate, creator), 
    (JUST_PluginDestroy, destroyer)));

JUST_FUNC_2(PP_err, PPBBOX_PluginUnregister, (
    (PP_str, factory), 
    (PP_str, name)));

#endif // _JUST_JUST_I_PLUGIN_H_
