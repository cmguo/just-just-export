// ppbox_dynamic.h

#ifndef _PLUGINS_PPBOX_PPBOX_DYNAMIC_H_
#define _PLUGINS_PPBOX_PPBOX_DYNAMIC_H_

#ifdef WIN32
# define PPBOX_DECL __declspec(dllimport)
#else
# define PPBOX_DECL
#endif

#include "ppbox/ppbox/import_func.h"

#endif // _PLUGINS_PPBOX_PPBOX_DYNAMIC_H_
