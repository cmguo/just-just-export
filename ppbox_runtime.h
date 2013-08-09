// ppbox_runtime.h

#ifndef _PLUGINS_PPBOX_PPBOX_RUNTIME_H_
#define _PLUGINS_PPBOX_PPBOX_RUNTIME_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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

#ifndef __cplusplus
#  define inline
#endif

#if (defined WINRT) || (defined WIN_PHONE)
static inline HMODULE LoadLibraryA(
	_In_  LPCSTR lpFileName)
{
	WCHAR pwFileName[MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, pwFileName, MAX_PATH);
	return LoadPackagedLibrary(pwFileName, 0);
}
#endif

#ifdef __cplusplus
#  define PPBOX_LIB_NAME ppbox::name_string()
#else
#  define PPBOX_LIB_NAME name_string()
#endif

#ifdef __cplusplus
extern HMODULE PPBOX_Load(
    char const * name = NULL);
#else
extern HMODULE PPBOX_Load(
    char const * name);
#endif
extern void PPBOX_Unload(void); 


#ifndef PPBOX_LIBRARY_NOT_EXIST
#  define PPBOX_LIBRARY_NOT_EXIST(x) fprintf(stderr, "library %s not exists\r\n", x); abort();
#endif

#ifndef PPBOX_FUNCTION_NOT_EXIST
#  define PPBOX_FUNCTION_NOT_EXIST(x) fprintf(stderr, "function %s not exists", #x); abort();
#endif

#define PPBOX_DECL

#ifndef PPBOX_FUNC
#ifdef PPBOX_DECLARE_ONLY
#define PPBOX_FUNC(type, name, np, params) \
    type name(PARAMS_TYPE_NAME(np, params));
#else  // PPBOX_DECLARE_ONLY
#define PPBOX_FUNC(type, name, np, params) \
    type name(PARAMS_TYPE_NAME(np, params)); \
    inline type name(PARAMS_TYPE_NAME(np, params)) \
    { \
        typedef type (* FT_ ## name)(PARAMS_TYPE(np, params)); \
        static FT_ ## name fp = NULL; \
        if (fp == NULL) { \
            fp = (FT_ ## name)dlsym(PPBOX_Load(NULL), #name); \
            if (fp == NULL) { \
                PPBOX_FUNCTION_NOT_EXIST(name); \
                return type##_defalut(); \
            } \
        } \
        return fp(PARAMS_NAME(np, params)); \
    }
#endif // PPBOX_DECLARE_ONLY
#endif // PPBOX_FUNC

#include "ppbox/ppbox/import_func.h"

#include "ppbox/ppbox/Name.h"

#ifndef PPBOX_DECLARE_ONLY

inline void PPBOX_Unload(void)
{
    if (PPBOX_Load(NULL)) {
#ifndef PPBOX_DISABLE_AUTO_START
        PPBOX_StopEngine();
#endif
        dlclose(PPBOX_Load(NULL));
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

#endif // PPBOX_DECLARE_ONLY

#endif // _PLUGINS_PPBOX_PPBOX_RUNTIME_H_
