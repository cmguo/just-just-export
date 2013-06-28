// ppbox_load.h

#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/comma_if.hpp>

#define FOMART(r, f, i, e) f(i, e)
#define LIST_FORMAT(f, n, items) BOOST_PP_SEQ_FOR_EACH_I(FOMART, f, BOOST_PP_TUPLE_TO_SEQ(n, items))

#define PAIR_FOMART(r, f, i, e) f(i, BOOST_PP_TUPLE_ELEM(2, 0, e), BOOST_PP_TUPLE_ELEM(2, 1, e))
#define LIST_PAIR_FORMAT(f, n, items) BOOST_PP_SEQ_FOR_EACH_I(PAIR_FOMART, f, BOOST_PP_TUPLE_TO_SEQ(n, items))

#define PAIR_DATA_FOMART(r, f_d, i, e) \
    BOOST_PP_TUPLE_ELEM(2, 0, f_d)(i, BOOST_PP_TUPLE_ELEM(2, 1, f_d), BOOST_PP_TUPLE_ELEM(2, 0, e), BOOST_PP_TUPLE_ELEM(2, 1, e))
#define LIST_DATA_PAIR_FORMAT(f, d, n, items) BOOST_PP_SEQ_FOR_EACH_I(PAIR_DATA_FOMART, (f, d), BOOST_PP_TUPLE_TO_SEQ(n, items))

#define ENUM_NAME_VALUE(i, n, v) n = v,
#define ENUMS_NAME_VALUE(ne, enums) LIST_PAIR_FORMAT(ENUM_NAME_VALUE, ne, enums)

#define ENUM_PREFIX_NAME_VALUE(i, p, n, v) BOOST_PP_CAT(p ## _, n) = v,
#define ENUMS_PREFIX_NAME_VALUE(p, ne, enums) LIST_DATA_PAIR_FORMAT(ENUM_PREFIX_NAME_VALUE, p, ne, enums)

#define FIELD_TYPE_NAME(i, t, n) t n;

#define FIELDS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(FIELD_TYPE_NAME, np, params)

#define PARAM_TYPE_NAME(i, t, n) BOOST_PP_COMMA_IF(i) t n
#define PARAM_TYPE(i, t, n) BOOST_PP_COMMA_IF(i) t
#define PARAM_NAME(i, t, n) BOOST_PP_COMMA_IF(i) n

#define PARAMS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(PARAM_TYPE_NAME, np, params)
#define PARAMS_TYPE(np, params) LIST_PAIR_FORMAT(PARAM_TYPE, np, params)
#define PARAMS_NAME(np, params) LIST_PAIR_FORMAT(PARAM_NAME, np, params)

#define PPBOX_ENUM_0(name, enums) PPBOX_ENUM(name, 0, enums)
#define PPBOX_ENUM_1(name, enums) PPBOX_ENUM(name, 1, enums)
#define PPBOX_ENUM_2(name, enums) PPBOX_ENUM(name, 2, enums)
#define PPBOX_ENUM_3(name, enums) PPBOX_ENUM(name, 3, enums)
#define PPBOX_ENUM_4(name, enums) PPBOX_ENUM(name, 4, enums)
#define PPBOX_ENUM_5(name, enums) PPBOX_ENUM(name, 5, enums)
#define PPBOX_ENUM_6(name, enums) PPBOX_ENUM(name, 6, enums)
#define PPBOX_ENUM_7(name, enums) PPBOX_ENUM(name, 7, enums)
#define PPBOX_ENUM_8(name, enums) PPBOX_ENUM(name, 8, enums)
#define PPBOX_ENUM_9(name, enums) PPBOX_ENUM(name, 9, enums)
#define PPBOX_ENUM_10(name, enums) PPBOX_ENUM(name, 10, enums)
#define PPBOX_ENUM_11(name, enums) PPBOX_ENUM(name, 11, enums)

#define PPBOX_STRUCT_0(name, fields) PPBOX_STRUCT(name, 0, fields)
#define PPBOX_STRUCT_1(name, fields) PPBOX_STRUCT(name, 1, fields)
#define PPBOX_STRUCT_2(name, fields) PPBOX_STRUCT(name, 2, fields)
#define PPBOX_STRUCT_3(name, fields) PPBOX_STRUCT(name, 3, fields)
#define PPBOX_STRUCT_4(name, fields) PPBOX_STRUCT(name, 4, fields)
#define PPBOX_STRUCT_5(name, fields) PPBOX_STRUCT(name, 5, fields)
#define PPBOX_STRUCT_6(name, fields) PPBOX_STRUCT(name, 6, fields)
#define PPBOX_STRUCT_7(name, fields) PPBOX_STRUCT(name, 7, fields)
#define PPBOX_STRUCT_8(name, fields) PPBOX_STRUCT(name, 8, fields)
#define PPBOX_STRUCT_9(name, fields) PPBOX_STRUCT(name, 9, fields)
#define PPBOX_STRUCT_10(name, fields) PPBOX_STRUCT(name, 10, fields)
#define PPBOX_STRUCT_11(name, fields) PPBOX_STRUCT(name, 11, fields)

#define PPBOX_FUNC_0(type, name, params) PPBOX_FUNC(type, name, 0, params)
#define PPBOX_FUNC_1(type, name, params) PPBOX_FUNC(type, name, 1, params)
#define PPBOX_FUNC_2(type, name, params) PPBOX_FUNC(type, name, 2, params)
#define PPBOX_FUNC_3(type, name, params) PPBOX_FUNC(type, name, 3, params)
#define PPBOX_FUNC_4(type, name, params) PPBOX_FUNC(type, name, 4, params)
#define PPBOX_FUNC_5(type, name, params) PPBOX_FUNC(type, name, 5, params)
#define PPBOX_FUNC_6(type, name, params) PPBOX_FUNC(type, name, 6, params)
#define PPBOX_FUNC_7(type, name, params) PPBOX_FUNC(type, name, 7, params)

#define PPBOX_CALLBACK_0(type, name, fields) PPBOX_CALLBACK(type, name, 0, fields)
#define PPBOX_CALLBACK_1(type, name, fields) PPBOX_CALLBACK(type, name, 1, fields)
#define PPBOX_CALLBACK_2(type, name, fields) PPBOX_CALLBACK(type, name, 2, fields)
#define PPBOX_CALLBACK_3(type, name, fields) PPBOX_CALLBACK(type, name, 3, fields)
#define PPBOX_CALLBACK_4(type, name, fields) PPBOX_CALLBACK(type, name, 4, fields)
#define PPBOX_CALLBACK_5(type, name, fields) PPBOX_CALLBACK(type, name, 5, fields)
#define PPBOX_CALLBACK_6(type, name, fields) PPBOX_CALLBACK(type, name, 6, fields)
#define PPBOX_CALLBACK_7(type, name, fields) PPBOX_CALLBACK(type, name, 7, fields)

#ifndef PPBOX_DECL
#  define PPBOX_DECL
#endif

#ifndef PPBOX_ENUM
#  ifdef __cplusplus
#    define PPBOX_ENUM(name, ne, enums) typedef struct name { enum Enum { ENUMS_NAME_VALUE(ne, enums) }; } name
#  else
#    define PPBOX_ENUM(name, ne, enums) typedef enum name ## _ ## Enum { ENUMS_PREFIX_NAME_VALUE(name, ne, enums) } name ## _ ## Enum
#  endif
#endif

#ifndef PPBOX_STRUCT
#  define PPBOX_STRUCT(name, nf, fields) typedef struct name { FIELDS_TYPE_NAME(nf, fields) } name
#endif

#ifndef PPBOX_FUNC
#  ifdef __cplusplus
#    define PPBOX_FUNC(type, name, np, params) extern "C" { PPBOX_DECL type name(PARAMS_TYPE(np, params)); }
#  else
#    define PPBOX_FUNC(type, name, np, params) PPBOX_DECL type name(PARAMS_TYPE(np, params))
#  endif
#endif

#ifndef PPBOX_CALLBACK
#  define PPBOX_CALLBACK(type, name, np, params) typedef type (*name)params
#endif

#include "ppbox/ppbox/IPpboxTypes.h"

inline void void_defalut() { }
inline PP_err PP_err_defalut() { return ppbox_other_error; }
inline PP_str PP_str_defalut() { return ""; }
inline PP_handle PP_handle_defalut() { return NULL; }
inline PP_ushort PP_ushort_defalut() { return 0; }
inline PP_uint PP_uint_defalut() { return 0; }
inline PP_ulong PP_ulong_defalut() { return 0; }

#include "ppbox/ppbox/IGeneric.h"
#include "ppbox/ppbox/ICallback.h"
#include "ppbox/ppbox/IDemuxer.h"
#include "ppbox/ppbox/IDownloader.h"
#include "ppbox/ppbox/ICapture.h"
