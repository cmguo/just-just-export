// IBlob.cpp

#include "ppbox/ppbox/Common.h"
#include "ppbox/ppbox/IBlob.h"
using namespace ppbox::error;

#include <ppbox/common/BlobManager.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.IBlob", framework::logger::Debug);

namespace ppbox
{

    class IBlob
    {
    public:
        IBlob()
            : manager_(ppbox::common::blob_manager())
        {
        }

        PP_err insert(
            PP_str key, 
            PPBOX_ConstBuffer const & data, 
            PP_bool remove_on_get)
        {
            boost::asio::const_buffer data2(data.data, data.len);
            error_code ec = manager_.insert(key, data2, remove_on_get);
            return last_error(__FUNCTION__, ec);
        }

        PP_err get(
            PP_str key, 
            PPBOX_ConstBuffer & data)
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
            ppbox::error::last_error(ec);
            return ppbox::error::last_error_enum(ec);
        }

    private:
        ppbox::common::BlobManager & manager_;
    };

} // namespace ppbox

static ppbox::IBlob & blob()
{
    static ppbox::IBlob the_blob;
    return the_blob;
}


#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBOX_InertBlob(
        PP_str key, 
        PPBOX_ConstBuffer const * data, 
        PP_bool remove_on_get)
    {
        return blob().insert(key, *data, remove_on_get);
    }

    PPBOX_DECL PP_err PPBOX_GetBlob(
        PP_str key, 
        PPBOX_ConstBuffer * data)
    {
        return blob().get(key, *data);
    }

    PPBOX_DECL PP_err PPBOX_RemoveBlob(
        PP_str key)
    {
        return blob().remove(key);
    }

#if __cplusplus
}
#endif // __cplusplus
