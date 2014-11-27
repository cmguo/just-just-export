// CxxTypeBind.h

#ifndef _JUST_JUST_DETAIL_CXX_TYPE_BIND_H_
#define _JUST_JUST_DETAIL_CXX_TYPE_BIND_H_

namespace just
{

    template <
        typename cT
    >
    struct TypeBind
    {
        typedef cT ctype_t;
        typedef cT xtype_t; // same

        static ctype_t const & x2c(xtype_t const & t)
        {
            return t;
        }

        static xtype_t const & c2x(ctype_t const & t)
        {
            return t;
        }
    };

#define BIND_TYPE(xt, ct) \
    template <> struct TypeBind<ct> { \
        typedef ct ctype_t; \
        typedef xt xtype_t; \
        static ctype_t const x2c(xtype_t const & x) { return ctype_t(x); } \
        static xtype_t const c2x(ctype_t const & c) { return xtype_t(c); } \
    };

    template <
        typename cT
    >
    struct TypeBind<cT *>
    {
        typedef cT * ctype_t;
        typedef typename TypeBind<cT>::xtype_t & xtype_t;
    };

    template <
        typename cT
    >
    struct TypeBind<cT const *>
    {
        typedef cT const * ctype_t;
        typedef typename TypeBind<cT>::xtype_t const & xtype_t;
    };

} // namespace just

#endif // _JUST_JUST_DETAIL_CXX_TYPE_BIND_H_
