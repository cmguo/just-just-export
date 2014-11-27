// IFunction.h

#ifndef _JUST_JUST_DETAIL_I_FUNCTION_H_
#define _JUST_JUST_DETAIL_I_FUNCTION_H_

#include "just/just/detail/IPreprocessor.h"

#define FUNCTION_PARAM_TYPE_NAME(i, t, n) BOOST_PP_COMMA_IF(i) t n
#define FUNCTION_PARAM_TYPE(i, t, n) BOOST_PP_COMMA_IF(i) t
#define FUNCTION_PARAM_NAME(i, t, n) BOOST_PP_COMMA_IF(i) n

#define FUNCTION_PARAMS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_TYPE_NAME, np, params)
#define FUNCTION_PARAMS_TYPE(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_TYPE, np, params)
#define FUNCTION_PARAMS_NAME(np, params) LIST_PAIR_FORMAT(FUNCTION_PARAM_NAME, np, params)

#ifndef JUST_DECL
#  define JUST_DECL
#endif

#define JUST_FUNC_0(type, name, params) JUST_FUNC(type, name, 0, params)
#define JUST_FUNC_1(type, name, params) JUST_FUNC(type, name, 1, params)
#define JUST_FUNC_2(type, name, params) JUST_FUNC(type, name, 2, params)
#define JUST_FUNC_3(type, name, params) JUST_FUNC(type, name, 3, params)
#define JUST_FUNC_4(type, name, params) JUST_FUNC(type, name, 4, params)
#define JUST_FUNC_5(type, name, params) JUST_FUNC(type, name, 5, params)
#define JUST_FUNC_6(type, name, params) JUST_FUNC(type, name, 6, params)
#define JUST_FUNC_7(type, name, params) JUST_FUNC(type, name, 7, params)

#ifndef JUST_FUNC
#  ifdef __cplusplus
#    define JUST_FUNC(type, name, np, params) extern "C" { JUST_DECL type name(FUNCTION_PARAMS_TYPE(np, params)); }
#  else
#    define JUST_FUNC(type, name, np, params) JUST_DECL type name(FUNCTION_PARAMS_TYPE(np, params))
#  endif
#endif

#endif // _JUST_JUST_DETAIL_I_FUNCTION_H_
