// CxxValueError.h

#ifndef _PPBOX_PPBOX_DETAIL_CXX_VALUE_ERROR_H_
#define _PPBOX_PPBOX_DETAIL_CXX_VALUE_ERROR_H_

#include "ppbox/ppbox/detail/CxxTypeBind.h"

namespace ppbox
{

    template <>
    struct TypeBind<PPBOX_Error>
    {
        typedef PPBOX_Error ctype_t;
        typedef boost::system::error_code xtype_t;

        static xtype_t c2x(ctype_t c)
        {
            return xtype_t(c.ec, boost::system::get_system_category());
        }

        static ctype_t x2c(xtype_t x)
        {
            ctype_t c;
            c.ec = x.value();
            return c;
        }
    };
    
} // namespace ppbox

#endif // _PPBOX_PPBOX_DETAIL_CXX_VALUE_ERROR_H_
