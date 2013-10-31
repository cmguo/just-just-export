// ICallback.cpp

#include "ppbox/ppbox/Common.h"
#include "ppbox/ppbox/ICallback.h"
#include "ppbox/ppbox/Callback.h"
using namespace ppbox::error;

#include <ppbox/common/ScheduleManager.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

#include <boost/bind.hpp>
using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.ISchedule", framework::logger::Debug);

namespace ppbox
{

    class ISchedule
    {
    public:
        ISchedule()
        : scheduler_(util::daemon::use_module<ppbox::common::ScheduleManager>(global_daemon()))
        {
        }

#ifdef PPBOX_ENABLE_REDIRECT_CALLBACK
        static void redirect_schedule_callback(
            PPBOX_Callback callback, 
            PP_context user_data, 
            PP_err ec)
        {
            redirect_call().call(callback, user_data, ec);
        }
#endif

        PP_handle schedule_callback(
            PP_uint delay, 
            PP_context user_data, 
            PPBOX_Callback callback)
        {
            return scheduler_.schedule_callback(delay, user_data, 
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, _1, boost::bind(last_error_enum, _2)));
#else
                boost::bind(redirect_schedule_callback, callback, _1, boost::bind(last_error_enum, _2)));
#endif
        }

        PP_err cancel_callback(
            PP_handle handle)
        {
            boost::system::error_code ec;
            scheduler_.cancel_callback(handle, ec);
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
        ppbox::common::ScheduleManager & scheduler_;
    };

} // namespace ppbox

static ppbox::ISchedule & schedule()
{
    static ppbox::ISchedule the_schedule;
    return the_schedule;
}


#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBOX_RedirectCallback(
        PPBOX_CallbackRedirect redirect, 
        PPBOX_CallbackFree free, 
        PP_context user_data)
    {
        ppbox::redirect_call().set(redirect, free, user_data);
        return ppbox_success;
    }

    PPBOX_DECL PP_handle PPBOX_ScheduleCallback(
        PP_uint delay, 
        PP_context user_data, 
        PPBOX_Callback callback)
    {
        return schedule().schedule_callback(delay, user_data, callback);
    }

    PPBOX_DECL PP_err PPBOX_CancelCallback(
        PP_handle handle)
    {
        return schedule().cancel_callback(handle);
    }

#if __cplusplus
}
#endif // __cplusplus
