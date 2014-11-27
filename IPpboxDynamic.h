// just_dynamic.h

#ifndef _PLUGINS_JUST_JUST_DYNAMIC_H_
#define _PLUGINS_JUST_JUST_DYNAMIC_H_

#ifdef WIN32
# define JUST_DECL __declspec(dllimport)
#else
# define JUST_DECL
#endif

#include "IPpbox.h"

#endif // _PLUGINS_JUST_JUST_DYNAMIC_H_
