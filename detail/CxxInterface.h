// Interface.h

#include "ppbox/ppbox/detail/IPreprocessor.h"
#include "ppbox/ppbox/detail/CxxValue.h"

#define PARAM_CXX_TYPE_NAME(i, t, n) BOOST_PP_COMMA_IF(i) TypeBind<t>::xtype_t n
#define PARAM_CXX_TYPE(i, t, n) BOOST_PP_COMMA_IF(i) TypeBind<t>::xtype_t
#define PARAM_LOCAL_VARIABLE(i, t, n) XValue<t> BOOST_PP_CAT(n, _l)(n);
#define PARAM_LOCAL_VALUE(i, t, n) BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(n, _l).cvalue()

#define PARAMS_CXX_TYPE_NAME(np, params) LIST_PAIR_FORMAT(PARAM_CXX_TYPE_NAME, np, params)
#define PARAMS_CXX_TYPE(np, params) LIST_PAIR_FORMAT(PARAM_CXX_TYPE, np, params)
#define PARAMS_LOCAL_VARIABLE(np, params) LIST_PAIR_FORMAT(PARAM_LOCAL_VARIABLE, np, params)
#define PARAMS_LOCAL_VALUE(np, params) LIST_PAIR_FORMAT(PARAM_LOCAL_VALUE, np, params)

#undef PPBOX_METHOD
#define PPBOX_METHOD(type, name, np, params) \
    virtual TypeBind<type>::xtype_t name(PARAMS_CXX_TYPE_NAME(np, params)) { \
        PARAMS_LOCAL_VARIABLE(np, params); \
        return CValue<type>(impl().name(pimpl() BOOST_PP_COMMA_IF(np) PARAMS_LOCAL_VALUE(np, params))).xvalue(); \
    }

#undef PPBOX_INTERFACE
#define PPBOX_INTERFACE(name, base, nm, methods) \
        INTERFACE_METHODS(nm, methods)
