// CxxValueUrl.h

#ifndef _PPBOX_PPBOX_DETAIL_CXX_VALUE_URL_H_
#define _PPBOX_PPBOX_DETAIL_CXX_VALUE_URL_H_

#include "ppbox/ppbox/detail/CxxTypeBind.h"

#include <framework/string/Url.h>

namespace ppbox
{

    struct url_holder
        : PPBOX_Url
    {
    public:
        url_holder(
            framework::string::Url const & url)
            : str_(url.to_string())
        {
            PPBOX_Url::str = str_.c_str();
        }

    protected:
        std::string str_;
    };

    template <>
    struct TypeBind<PPBOX_Url>
    {
        typedef PPBOX_Url ctype_t;
        typedef framework::string::Url xtype_t;

        static xtype_t c2x(ctype_t c)
        {
            return xtype_t(c.str);
        }

        static url_holder x2c(xtype_t x)
        {
            return url_holder(x);
        }

    protected:
        xtype_t x_;
        PPBOX_Url c_;
    };
    
} // namespace ppbox

#endif // _PPBOX_PPBOX_DETAIL_CXX_VALUE_URL_H_
