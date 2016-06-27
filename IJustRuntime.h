// just_runtime.h

#ifndef _PLUGINS_JUST_EXPORT_RUNTIME_H_
#define _PLUGINS_JUST_EXPORT_RUNTIME_H_

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

#ifndef JUST_LIB_NAME
#  ifdef __cplusplus
#    define JUST_LIB_NAME just::name_string()
#  else
#    define JUST_LIB_NAME name_string()
#  endif
#endif

#ifdef __cplusplus
extern HMODULE JUST_Load(
    char const * name = NULL);
#else
extern HMODULE JUST_Load(
    char const * name);
#endif
extern void JUST_Unload(void); 


#ifndef JUST_LIBRARY_NOT_EXIST
#  define JUST_LIBRARY_NOT_EXIST(x) fprintf(stderr, "library %s not exists\r\n", x); abort();
#endif

#ifndef JUST_FUNCTION_NOT_EXIST
#  define JUST_FUNCTION_NOT_EXIST(x) fprintf(stderr, "function %s not exists", #x); abort();
#endif

#define JUST_DECL

#ifndef JUST_FUNC
#ifdef JUST_DECLARE_ONLY
#define JUST_FUNC(type, name, np, params) \
    type name(FUNCTION_PARAMS_TYPE_NAME(np, params));
#else  // JUST_DECLARE_ONLY
#define JUST_FUNC(type, name, np, params) \
    type name(FUNCTION_PARAMS_TYPE_NAME(np, params)); \
    inline type name(FUNCTION_PARAMS_TYPE_NAME(np, params)) \
    { \
        typedef type (* FT_ ## name)(FUNCTION_PARAMS_TYPE(np, params)); \
        static FT_ ## name fp = NULL; \
        if (fp == NULL) { \
            fp = (FT_ ## name)dlsym(JUST_Load(NULL), #name); \
            if (fp == NULL) { \
                JUST_FUNCTION_NOT_EXIST(name); \
                return type##_defalut(); \
            } \
        } \
        return fp(FUNCTION_PARAMS_NAME(np, params)); \
    }
#endif // JUST_DECLARE_ONLY
#endif // JUST_FUNC

#include "IJustTypes.h"

static inline void void_defalut(void) { }
static inline PP_err PP_err_defalut(void) { return just_other_error; }
static inline PP_str PP_str_defalut(void) { return ""; }
static inline PP_handle PP_handle_defalut(void) { return NULL; }
static inline PP_ushort PP_ushort_defalut(void) { return 0; }
static inline PP_uint PP_uint_defalut(void) { return 0; }
static inline PP_ulong PP_ulong_defalut(void) { return 0; }

#include "IJust.h"

#include "Name.h"

#ifndef JUST_DECLARE_ONLY

inline void JUST_Unload(void)
{
    if (JUST_Load(NULL)) {
#ifndef JUST_DISABLE_AUTO_START
        JUST_StopEngine();
#endif
        dlclose(JUST_Load(NULL));
    }
}

inline HMODULE JUST_Load(
    char const * name)
{
    static HMODULE hMod = NULL;
    if (hMod == NULL) {
        if (name == NULL)
            name = JUST_LIB_NAME;
        hMod = dlopen(name, RTLD_LAZY);
        if (hMod == NULL) {
            JUST_LIBRARY_NOT_EXIST(name);
        } else {
#ifndef JUST_DISABLE_AUTO_START
            PP_err ret = JUST_StartEngine("12", "161", "08ae1acd062ea3ab65924e07717d5994");
            if (ret != just_success && ret != just_already_start) {
                JUST_Unload();
            }
#endif
        }
    }
    return hMod;
}

#endif // JUST_DECLARE_ONLY

#endif // _PLUGINS_JUST_EXPORT_RUNTIME_H_
