// ICallback.cpp

#include "just/just/Common.h"
#include "just/just/ICallback.h"
#include "just/just/Callback.h"
using namespace just::error;

#include <just/common/ScheduleManager.h>

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

#include <boost/bind.hpp>
using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.ISchedule", framework::logger::Debug);

namespace just
{

    class ISchedule
    {
    public:
        ISchedule()
        : scheduler_(util::daemon::use_module<just::common::ScheduleManager>(global_daemon()))
        {
        }

#ifdef JUST_ENABLE_REDIRECT_CALLBACK
        static void redirect_schedule_callback(
            JUST_Callback callback, 
            PP_context user_data, 
            PP_err ec)
        {
            redirect_call().call(callback, user_data, ec);
        }
#endif

        PP_handle schedule_callback(
            PP_uint delay, 
            PP_context user_data, 
            JUST_Callback callback)
        {
            return scheduler_.schedule_callback(delay, user_data, 
#ifndef JUST_ENABLE_REDIRECT_CALLBACK
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
            just::error::last_error(ec);
            return just::error::last_error_enum(ec);
        }

    private:
        just::common::ScheduleManager & scheduler_;
    };

} // namespace just

static just::ISchedule & schedule()
{
    static just::ISchedule the_schedule;
    return the_schedule;
}


#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err JUST_RedirectCallback(
        JUST_CallbackRedirect redirect, 
        JUST_CallbackFree free, 
        PP_context user_data)
    {
        just::redirect_call().set(redirect, free, user_data);
        return just_success;
    }

    JUST_DECL PP_handle JUST_ScheduleCallback(
        PP_uint delay, 
        PP_context user_data, 
        JUST_Callback callback)
    {
        return schedule().schedule_callback(delay, user_data, callback);
    }

    JUST_DECL PP_err JUST_CancelCallback(
        PP_handle handle)
    {
        return schedule().cancel_callback(handle);
    }

#if __cplusplus
}
#endif // __cplusplus
