// IPpbox.cpp

#include "ppbox/ppbox/Common.h"
#include "ppbox/ppbox/Version.h"
#include "ppbox/ppbox/IPpbox.h"
using namespace ppbox::error;

#include <ppbox/common/CommonModule.h>
#include <ppbox/common/Version.h>
//#include <ppbox/common/ConfigMgr.h>
#include <ppbox/common/ScheduleManager.h>
#include <ppbox/common/Debuger.h>
#include <ppbox/common/PortManager.h>
#ifndef PPBOX_DISABLE_DEBUGPROXY
#include <ppbox/common/DebugProxy.h>
#endif
using namespace ppbox::common;

#include "ppbox/ppbox/Core.h"
#include "ppbox/ppbox/Input.h"
#include "ppbox/ppbox/Output.h"
#include "ppbox/ppbox/Server.h"
#include "ppbox/ppbox/P2p.h"
#include "ppbox/ppbox/Callback.h"

#include <framework/logger/StreamRecord.h>
#include <framework/logger/Section.h>
#include <framework/process/MessageQueue.h>
#include <framework/process/Process.h>
#include <framework/filesystem/Path.h>
using namespace framework::logger;
using namespace framework::process;

#include <boost/asio/error.hpp>
#include <boost/thread/thread.hpp>
using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("ppbox.IPpbox", framework::logger::Debug);

namespace ppbox
{

    class IPpbox
        : public util::daemon::Daemon
    {
    public:
        IPpbox()
            : util::daemon::Daemon("ppbox.conf")
        {

            char const * default_argv[] = {
                "++framework.logger.Stream.0.file=$LOG/ppbox.log", 
                "++framework.logger.Stream.0.append=true", 
                "++framework.logger.Stream.0.roll=true", 
                "++framework.logger.Stream.0.level=5", 
                "++framework.logger.Stream.0.size=102400", 
            };
            parse_cmdline(sizeof(default_argv) / sizeof(default_argv[0]), default_argv);

            framework::logger::load_config(global_logger(), config());

            ppbox::common::log_versions();

            std::string ppbox_name = "Ppbox_" + framework::filesystem::bin_file().stem();
            ppbox::common::CommonModule & common = 
                util::daemon::use_module<ppbox::common::CommonModule>(*this, ppbox_name);
            common.set_version(ppbox::version());

            //util::daemon::use_module<ppbox::common::ConfigMgr>(*this);
            util::daemon::use_module<ppbox::common::Debuger>(*this);
            util::daemon::use_module<ppbox::common::PortManager>(*this);
#ifndef PPBOX_DISABLE_DEBUGPROXY
            util::daemon::use_module<ppbox::common::DebugProxy>(*this);
#endif
            core_init(*this);

            input_init(*this);

            output_init(*this);

            server_init(*this);

            p2p_init(*this);

            LOG_INFO("Ppbox ready.");
        }

        PP_err start_engine(
            PP_str gid, 
            PP_str pid, 
            PP_str auth)
        {
            LOG_SECTION();

            error_code ec;
            if (is_started()) {
                ec = already_start;
            } else {
#ifndef PPBOX_DISABLE_CERTIFY           
                ppbox::certify::Certifier & cert = 
                    util::daemon::use_module<ppbox::certify::Certifier>(*this);
                cert.set_auth_code(gid, pid, auth);
#endif				

#ifndef PPBOX_DISABLE_DAC
                ppbox::dac::DacModule & dac = 
                    util::daemon::use_module<ppbox::dac::DacModule>(*this);
                dac.set_auth_code(gid, pid, auth);
#endif
                ec = start(1);
            }

            return last_error(__FUNCTION__, ec);
        }

        PP_err stop_engine()
        {
            LOG_SECTION();

            error_code ec;
            if (!is_started()) {
                ec = not_start;
            } else {
                stop();
            }

            return last_error(__FUNCTION__, ec);
        }

        PP_ushort get_port(
            PP_str module)
        {
            boost::uint16_t port = 0;
            std::string strModule(module);
            if(strModule == "rtsp")
            {
                std::string netName("0.0.0.0:0");
                config().get("RtspManager","addr",netName);
                framework::network::NetName tmpNet(netName);
                port = tmpNet.port();
            }
            else if(strModule == "http")
            {
                std::string netName("0.0.0.0:0");
                config().get("HttpManager","addr",netName);
                framework::network::NetName tmpNet(netName);
                port = tmpNet.port();
            }
            else
            {
            }
            LOG_WARN("[get_port] Module:" << strModule<<" port:"<<port);
            return port;
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

        PP_err last_error() const
        {
            return ppbox::error::last_error_enum();
        }

        PP_str last_error_msg() const
        {
            static std::string err_msg;
            err_msg = ppbox::error::last_error().message();
            return err_msg.c_str();
        }

        PP_str version() const
        {
            return ppbox::version_string();
        }

        PP_err set_config(
            PP_str module, 
            PP_str section, 
            PP_str key, 
            PP_str value)
        {
            if(NULL == section ||
                NULL == key ||
                NULL == value)
            {
                return ppbox_logic_error;
            }
            config().set(section,key,value);
            return ppbox_success;
        }

        PP_err debug_mode(
            PP_bool mode)
        {
            ppbox::common::Debuger & debuger = 
                util::daemon::use_module<ppbox::common::Debuger>(*this);
            debuger.change_debug_mode(mode);
            return ppbox_success;
        }

        PP_uint get_debug_msg(
            DialogMessage * vector, 
            PP_uint size, 
            PP_str module, 
            PP_uint level)
        {
            ppbox::common::Debuger & debuger = 
                util::daemon::use_module<ppbox::common::Debuger>(*this);
            static std::vector<Message> msgs;
            msgs.clear();
            debuger.get_debug_msg(msgs, size, module, level);

            for (size_t i = 0; i < msgs.size(); ++i)
            {
                vector[i].time = msgs[i].time;
                vector[i].module = msgs[i].sender.c_str();
                vector[i].level = msgs[i].level;
                vector[i].size = msgs[i].data.length();
                vector[i].msg = msgs[i].data.c_str();
            }

            return msgs.size();
        }

        PP_err submit_msg(
            PP_str msg, 
            boost::int32_t size)
        {
#ifndef PPBOX_DISABLE_DAC
            ppbox::dac::DacModule & dac = 
                util::daemon::use_module<ppbox::dac::DacModule>(*this);
            dac.submit_log(msg, size);
#endif
            return ppbox_success;
        }

        PP_err log_dump(
            PPBOX_OnLogDump callback,
            boost::uint32_t level)
        {
            ppbox::common::Debuger & debuger = 
                util::daemon::use_module<ppbox::common::Debuger>(*this);
            debuger.set_log_hook(
                ( ppbox::common::Debuger::on_logdump_type )(callback), level);
            return ppbox_success;
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
            ppbox::common::ScheduleManager & scheduler = 
                util::daemon::use_module<ppbox::common::ScheduleManager>(*this);
            return scheduler.schedule_callback(delay, user_data, 
#ifndef PPBOX_ENABLE_REDIRECT_CALLBACK
                boost::bind(callback, _1, boost::bind(last_error_enum, _2)));
#else
                boost::bind(redirect_schedule_callback, callback, _1, boost::bind(last_error_enum, _2)));
#endif
        }

        PP_err cancel_callback(
            PP_handle handle)
        {
            ppbox::common::ScheduleManager & scheduler = 
                util::daemon::use_module<ppbox::common::ScheduleManager>(*this);
            boost::system::error_code ec;
            scheduler.cancel_callback(handle, ec);
            return last_error(__FUNCTION__, ec);
        }
    };

    util::daemon::Daemon & global_daemon()
    {
        static IPpbox the_ppbox;
        return the_ppbox;
    }

}

ppbox::IPpbox & the_ppbox()
{
    return static_cast<ppbox::IPpbox &>(ppbox::global_daemon());
}

using namespace ppbox;

#if __cplusplus
extern "C" {
#endif // __cplusplus

    PPBOX_DECL PP_err PPBOX_StartEngine(
        PP_str gid, 
        PP_str pid, 
        PP_str auth)
    {
        return the_ppbox().start_engine(gid, pid, auth);
    }

    PPBOX_DECL PP_ushort PPBOX_GetPort(
        PP_str moduleName)
    {
        return the_ppbox().get_port(moduleName);
    }

    PPBOX_DECL PP_err PPBOX_StopEngine()
    {
        return the_ppbox().stop_engine();
    }

    PPBOX_DECL PP_err PPBOX_GetLastError()
    {
        return the_ppbox().last_error();
    }

    PPBOX_DECL PP_str PPBOX_GetLastErrorMsg()
    {
        return the_ppbox().last_error_msg();
    }

    PPBOX_DECL PP_str PPBOX_GetVersion()
    {
        return the_ppbox().version();
    }

    PPBOX_DECL PP_err PPBOX_SetConfig(
        PP_str module, 
        PP_str section, 
        PP_str key, 
        PP_str value)
    {
        return the_ppbox().set_config(module, section, key, value);
    }

    PPBOX_DECL PP_err PPBOX_DebugMode(
        bool mode)
    {
        return the_ppbox().debug_mode(mode);
    }

    PPBOX_DECL PP_uint PPBOX_DialogMessage(
        DialogMessage * vector, 
        PP_uint size, 
        PP_str module, 
        PP_uint level)
    {
        return the_ppbox().get_debug_msg(vector, size, module, level);
    }

    PPBOX_DECL PP_err PPBOX_SubmitMessage(
        PP_str msg, 
        PP_uint size)
    {
        return the_ppbox().submit_msg(msg, size);
    }

    PPBOX_DECL PP_err PPBOX_LogDump(
        PPBOX_OnLogDump callback,
        PP_uint level)
    {
        return the_ppbox().log_dump(callback, level);
    }

    PPBOX_DECL PP_handle PPBOX_ScheduleCallback(
        PP_uint delay, 
        PP_context user_data, 
        PPBOX_Callback callback)
    {
        return the_ppbox().schedule_callback(delay, user_data, callback);
    }

    PPBOX_DECL PP_err PPBOX_CancelCallback(
        PP_handle handle)
    {
        return the_ppbox().cancel_callback(handle);
    }


#if __cplusplus
}
#endif // __cplusplus
