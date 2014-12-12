// CxxValueBuffer.h

#ifndef _JUST_EXPORT_DETAIL_CXX_VALUE_BUFFER_H_
#define _JUST_EXPORT_DETAIL_CXX_VALUE_BUFFER_H_

#include "just/export/detail/CxxTypeBind.h"

#include <framework/container/Array.h>

#include <util/stream/StreamBuffers.h>

namespace just
{

    template <>
    struct TypeBind<JUST_ConstBuffer>
    {
        typedef JUST_ConstBuffer ctype_t;
        typedef boost::asio::const_buffer xtype_t;

        static xtype_t c2x(ctype_t c)
        {
            return xtype_t(c.data, c.len);
        }

        static ctype_t x2c(xtype_t x)
        {
            ctype_t c;
            c.data = boost::asio::buffer_cast<PP_ubyte const *>(x);
            c.len = boost::asio::buffer_size(x);
            return c;
        }
    };
    
    struct buffers_holder
        : JUST_ConstBuffers
    {
    public:
        buffers_holder(
            util::stream::StreamConstBuffers const & buffers)
        {
            util::stream::StreamConstBuffers::const_iterator iter = buffers.begin();
            for (; iter != buffers.end(); ++iter) {
                buffers_.push_back(TypeBind<JUST_ConstBuffer>::x2c(*iter));
            }
            JUST_ConstBuffers::buffers = &buffers_.front();
            JUST_ConstBuffers::count = buffers_.size();
        }

    protected:
        std::vector<JUST_ConstBuffer> buffers_;
    };

    template <>
    struct TypeBind<JUST_ConstBuffers>
    {
        typedef JUST_ConstBuffers ctype_t;
        typedef util::stream::StreamConstBuffers xtype_t;

        static xtype_t c2x(ctype_t c)
        {
            xtype_t x;
            for (PP_uint i = 0; i < c.count; ++i) {
                x.push_back(TypeBind<JUST_ConstBuffer>::c2x(c.buffers[i]));
            }
            return x;
        }

        static ctype_t x2c(xtype_t x)
        {
            return buffers_holder(x);
        }
    };
    
} // namespace just

#endif // _JUST_EXPORT_DETAIL_CXX_VALUE_BUFFER_H_
