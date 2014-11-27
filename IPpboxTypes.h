// IPpboxTypes.h

#ifndef _JUST_JUST_I_JUST_TYPES_H_
#define _JUST_JUST_I_JUST_TYPES_H_

#ifndef DEFINED_PP_BOOL
#define DEFINED_PP_BOOL
typedef bool PP_bool;
#endif // DEFINED_PP_BOOL

#ifndef DEFINED_PP_CHAR
#define DEFINED_PP_CHAR
typedef char PP_char;
#endif // DEFINED_PP_CHAR

#ifndef DEFINED_PP_BYTE
#define DEFINED_PP_BYTE
typedef signed char PP_byte;
#endif // DEFINED_PP_BYTE

#ifndef DEFINED_PP_SHORT
#define DEFINED_PP_SHORT
typedef short PP_short;
#endif // DEFINED_PP_SHORT

#ifndef DEFINED_PP_INI
#define DEFINED_PP_INI
typedef long PP_int;
#endif // DEFINED_PP_INI

#ifndef DEFINED_PP_LONG
#define DEFINED_PP_LONG
typedef long long PP_long;
#endif // DEFINED_PP_LONG

#ifndef DEFINED_PP_UBYTE
#define DEFINED_PP_UBYTE
typedef unsigned char PP_ubyte;
#endif // DEFINED_PP_UBYTE

#ifndef DEFINED_PP_USHORT
#define DEFINED_PP_USHORT
typedef unsigned short PP_ushort;
#endif // DEFINED_PP_USHORT

#ifndef DEFINED_PP_UINI
#define DEFINED_PP_UINI
typedef unsigned long PP_uint;
#endif // DEFINED_PP_UINI

#ifndef DEFINED_PP_ULONG
#define DEFINED_PP_ULONG
typedef unsigned long long PP_ulong;
#endif // DEFINED_PP_ULONG

typedef PP_int PP_err;

typedef PP_char const * PP_str;
typedef PP_ubyte const * PP_data;

typedef void const * PP_handle;
typedef void const * PP_context;

enum JUST_ErrorEnum
{
    just_success = 0, 
    just_not_start, 
    just_already_start, 
    just_not_open, 
    just_already_open, 
    just_operation_canceled, 
    just_would_block, 
    just_stream_end, 
    just_logic_error, 
    just_network_error, 
    just_demux_error, 
    just_certify_error, 
    just_other_error = 1024, 
};

#endif // _JUST_JUST_I_JUST_TYPES_H_
