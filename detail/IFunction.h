// IFunction.h

#ifndef _PPBOX_PPBOX_DETAIL_I_FUNCTION_H_
#define _PPBOX_PPBOX_DETAIL_I_FUNCTION_H_

#include "ppbox/ppbox/detail/IPreprocessor.h"

#define FUNCTION_PARAM_TYPE_NAME(i, t, n) BOOST_PP_COMMA_IF(i) t n
#define FUNCTION_PARAM_TYPE(i, t, n) BOOST_PP_COMMA_IF(i) t
#define FUNCTION_PARAM_NAME(i, t, n) BOOST_PP_COMMA_IF(i) n

#define FUNCTION_PARAMS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_TYPE_NAME, np, params)
#define FUNCTION_PARAMS_TYPE(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_TYPE, np, params)
#define FUNCTION_PARAMS_NAME(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_NAME, np, params)

#ifndef PPBOX_DECL
#  define PPBOX_DECL
#endif

#define PPBOX_FUNC_0(type, name, params) PPBOX_FUNC(type, name, 0, params)
#define PPBOX_FUNC_1(type, name, params) PPBOX_FUNC(type, name, 1, params)
#define PPBOX_FUNC_2(type, name, params) PPBOX_FUNC(type, name, 2, params)
#define PPBOX_FUNC_3(type, name, params) PPBOX_FUNC(type, name, 3, params)
#define PPBOX_FUNC_4(type, name, params) PPBOX_FUNC(type, name, 4, params)
#define PPBOX_FUNC_5(type, name, params) PPBOX_FUNC(type, name, 5, params)
#define PPBOX_FUNC_6(type, name, params) PPBOX_FUNC(type, name, 6, params)
#define PPBOX_FUNC_7(type, name, params) PPBOX_FUNC(type, name, 7, params)

#ifndef PPBOX_FUNC
#  ifdef __cplusplus
#    define PPBOX_FUNC(type, name, np, params) extern "C" { PPBOX_DECL type name(FUNCTION_PARAMS_TYPE(np, params)); }
#  else
#    define PPBOX_FUNC(type, name, np, params) PPBOX_DECL type name(FUNCTION_PARAMS_TYPE(np, params))
#  endif
#endif

#endif // _PPBOX_PPBOX_DETAIL_I_FUNCTION_H_
