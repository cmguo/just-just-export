// IBlob.cpp

#include "just/just/Common.h"
#include "just/just/IBlob.h"
using namespace just::error;

#include <just/common/BlobManager.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.IBlob", framework::logger::Debug);

namespace just
{

    class IBlob
    {
    public:
        IBlob()
            : manager_(just::common::blob_manager())
        {
        }

        PP_err insert(
            PP_str key, 
            JUST_ConstBuffer const & data, 
            PP_bool remove_on_get)
        {
            boost::asio::const_buffer data2(data.data, data.len);
            error_code ec = manager_.insert(key, data2, remove_on_get);
            return last_error(__FUNCTION__, ec);
        }

        PP_err get(
            PP_str key, 
            JUST_ConstBuffer & data)
        {
            boost::asio::const_buffer data2;
            error_code ec = manager_.get(key, data2);
            if (!ec) {
                data.data = boost::asio::buffer_cast<PP_ubyte const *>(data2);
                data.len = boost::asio::buffer_size(data2);
            }
            return last_error(__FUNCTION__, ec);
        }

        PP_err remove(
            PP_str key)
        {
            error_code ec = manager_.remove(key);
            return last_error(__FUNCTION__, ec);
        }

        error::errors last_error(
            char const * log_title, 
            error_code const & ec) const
        {
            if (ec && ec != boost::asio::error::would_block) {
                LOG_WARN(log_title << ": " << ec.message());
            }
            just::error::last_error(ec);
            return just::error::last_error_enum(ec);
        }

    private:
        just::common::BlobManager & manager_;
    };

} // namespace just

static just::IBlob & blob()
{
    static just::IBlob the_blob;
    return the_blob;
}


#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err JUST_InsertBlob(
        PP_str key, 
        JUST_ConstBuffer const * data, 
        PP_bool remove_on_get)
    {
        return blob().insert(key, *data, remove_on_get);
    }

    JUST_DECL PP_err JUST_GetBlob(
        PP_str key, 
        JUST_ConstBuffer * data)
    {
        return blob().get(key, *data);
    }

    JUST_DECL PP_err JUST_RemoveBlob(
        PP_str key)
    {
        return blob().remove(key);
    }

#if __cplusplus
}
#endif // __cplusplus
