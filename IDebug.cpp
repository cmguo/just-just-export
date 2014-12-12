// IDebug.cpp

#include "just/export/Common.h"
#include <just/common/Debuger.h>

#include <framework/process/MessageQueue.h>
#include <framework/process/Process.h>
using namespace framework::process;

#include <framework/logger/Logger.h>
#include <framework/logger/StreamRecord.h>

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.IDebug", framework::logger::Debug);

namespace just
{

    class IDebug
    {
    public:
        IDebug()
            : debuger_(util::daemon::use_module<just::common::Debuger>(global_daemon()))
        {

        }

        PP_err change_debug_mode(
            PP_bool mode)
        {
            debuger_.change_debug_mode(mode);
            return just_success;
        }

        PP_uint get_debug_msg(
            JUST_DebugMessage * vector, 
            PP_uint size, 
            PP_str module, 
            PP_uint level)
        {
            static std::vector<Message> msgs;
            msgs.clear();
            debuger_.get_debug_msg(msgs, size, module, level);

            for (size_t i = 0; i < msgs.size(); ++i) {
                vector[i].time = msgs[i].time;
                vector[i].module = msgs[i].sender.c_str();
                vector[i].level = msgs[i].level;
                vector[i].size = msgs[i].data.length();
                vector[i].msg = msgs[i].data.c_str();
            }

            return msgs.size();
        }

        PP_err set_log_hook(
            JUST_LogHook callback,
            boost::uint32_t level)
        {
            debuger_.set_log_hook(
                ( just::common::Debuger::on_logdump_type )(callback), level);
            return just_success;
        }

    private:
        just::common::Debuger & debuger_;
    };

}

static just::IDebug & debug()
{
    static just::IDebug the_debug;
    return the_debug;
}

using namespace just;

#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err JUST_ChangeDebugMode(
        bool mode)
    {
        return debug().change_debug_mode(mode);
    }

    JUST_DECL PP_uint JUST_GetDebugMessage(
        JUST_DebugMessage * vector, 
        PP_uint size, 
        PP_str module, 
        PP_uint level)
    {
        return debug().get_debug_msg(vector, size, module, level);
    }

    JUST_DECL PP_err JUST_SetLogHook(
        JUST_LogHook hook,
        PP_uint level)
    {
        return debug().set_log_hook(hook, level);
    }

#if __cplusplus
}
#endif // __cplusplus
