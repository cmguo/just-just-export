// IInterface.h

#ifndef _PPBOX_PPBOX_DETAIL_I_INTERFACE_H_
#define _PPBOX_PPBOX_DETAIL_I_INTERFACE_H_

#include "ppbox/ppbox/detail/IPreprocessor.h"

#define METHOD_PARAM_TYPE_NAME(i, t, n) BOOST_PP_COMMA_IF(i) t n
#define METHOD_PARAM_TYPE(i, t, n) BOOST_PP_COMMA_IF(i) t
#define METHOD_PARAM_NAME(i, t, n) BOOST_PP_COMMA_IF(i) n

#define METHOD_PARAMS_TYPE_NAME(np, params) LIST_PAIR_FORMAT(METHOD_PARAM_TYPE_NAME, np, params)
#define METHOD_PARAMS_TYPE(np, params) LIST_PAIR_FORMAT(METHOD_PARAM_TYPE, np, params)
#define METHOD_PARAMS_NAME(np, params) LIST_PAIR_FORMAT(METHOD_PARAM_NAME, np, params)

#define INTERFACE_METHOD(i, m) m
#define INTERFACE_METHODS(nm, methods) LIST_FORMAT(INTERFACE_METHOD, nm, methods)

#ifndef PPBOX_INTERFACE
#  ifdef __cplusplus
#    define PPBOX_METHOD(type, name, np, params) \
        type (*name)(PPBOX_Plugin * BOOST_PP_COMMA_IF(np) METHOD_PARAMS_TYPE(np, params));
#    define PPBOX_INTERFACE(name, base, nm, methods) \
        struct name : base { \
            INTERFACE_METHODS(nm, methods) \
        };
#  else
#  endif
#endif

#endif // _PPBOX_PPBOX_DETAIL_I_INTERFACE_H_
