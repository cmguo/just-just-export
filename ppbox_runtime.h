// ppbox_runtime.h

#ifndef _PLUGINS_PPBOX_PPBOX_RUNTIME_H_
#define _PLUGINS_PPBOX_PPBOX_RUNTIME_H_

#include <assert.h>
#include <stdlib.h>

#if (defined _WIN32) || (defined WIN32)
#include <windows.h>
#  define dlopen(n, f) LoadLibraryA(n)
#  define dlsym(m, s) GetProcAddress(m, s)
#  define dlclose(m) FreeLibrary(m)
#  define RTLD_LAZY 0
#else
#  include <dlfcn.h>
typedef void * HMODULE;
#endif

#if (defined WINRT) || (defined WIN_PHONE)
static inline HMODULE LoadLibraryA(
	_In_  LPCSTR lpFileName)
{
	static WCHAR pwFileName[MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, pwFileName, MAX_PATH);
	return LoadPackagedLibrary(pwFileName, 0);
}
#endif

#define PPBOX_LIB_NAME ppbox::name_string()

extern HMODULE PPBOX_Load(
    char const * name = NULL);
extern void PPBOX_Unload();


#ifndef PPBOX_LIBRARY_NOT_EXIST
#  define PPBOX_LIBRARY_NOT_EXIST(x) assert("library" #x "not exists" && false);
#endif

#ifndef PPBOX_FUNCTION_NOT_EXIST
#  define PPBOX_FUNCTION_NOT_EXIST(x) assert("function" #x "not exists" && false);
#endif

#define PPBOX_DECL

#ifndef PPBOX_FUNC
#define PPBOX_FUNC(type, name, np, params) \
    typedef type (* FT_ ## name)(PARAMS_TYPE(np, params)); \
    inline type name(PARAMS_TYPE_NAME(np, params)) \
    { \
        static FT_ ## name fp = NULL; \
        if (fp == NULL) { \
            fp = (FT_ ## name)dlsym(PPBOX_Load(), #name); \
            if (fp == NULL) { \
                PPBOX_FUNCTION_NOT_EXIST(name); \
                return type##_defalut(); \
            } \
        } \
        return fp(PARAMS_NAME(np, params)); \
    }
#endif

#include "ppbox/ppbox/import_func.h"

#include "ppbox/ppbox/Name.h"

inline void PPBOX_Unload()
{
    if (PPBOX_Load()) {
#ifndef PPBOX_DISABLE_AUTO_START
        PPBOX_StopEngine();
#endif
        dlclose(PPBOX_Load());
    }
}

inline HMODULE PPBOX_Load(
    char const * name)
{
    static HMODULE hMod = NULL;
    if (hMod == NULL) {
        if (name == NULL)
            name = PPBOX_LIB_NAME;
        hMod = dlopen(name, RTLD_LAZY);
        if (hMod == NULL) {
            PPBOX_LIBRARY_NOT_EXIST(name);
        } else {
#ifndef PPBOX_DISABLE_AUTO_START
            PP_err ret = PPBOX_StartEngine("12", "161", "08ae1acd062ea3ab65924e07717d5994");
            if (ret != ppbox_success && ret != ppbox_already_start) {
                PPBOX_Unload();
            }
#endif
        }
    }
    return hMod;
}

#endif // _PLUGINS_PPBOX_PPBOX_RUNTIME_H_
