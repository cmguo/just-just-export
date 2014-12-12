// CxxValueError.h

#ifndef _JUST_EXPORT_DETAIL_CXX_VALUE_ERROR_H_
#define _JUST_EXPORT_DETAIL_CXX_VALUE_ERROR_H_

#include "just/export/detail/CxxTypeBind.h"

namespace just
{

    template <>
    struct TypeBind<JUST_Error>
    {
        typedef JUST_Error ctype_t;
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
    
} // namespace just

#endif // _JUST_EXPORT_DETAIL_CXX_VALUE_ERROR_H_
