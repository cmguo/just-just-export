// CxxValueUrl.h

#ifndef _JUST_EXPORT_DETAIL_CXX_VALUE_URL_H_
#define _JUST_EXPORT_DETAIL_CXX_VALUE_URL_H_

#include "just/export/detail/CxxTypeBind.h"

#include <framework/string/Url.h>

namespace just
{

    struct url_holder
        : JUST_Url
    {
    public:
        url_holder(
            framework::string::Url const & url)
            : str_(url.to_string())
        {
            JUST_Url::str = str_.c_str();
        }

    protected:
        std::string str_;
    };

    template <>
    struct TypeBind<JUST_Url>
    {
        typedef JUST_Url ctype_t;
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
        JUST_Url c_;
    };
    
} // namespace just

#endif // _JUST_EXPORT_DETAIL_CXX_VALUE_URL_H_
