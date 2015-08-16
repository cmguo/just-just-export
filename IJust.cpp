// IJust.cpp

#include "just/export/Common.h"
#include "just/export/Version.h"
#include "just/export/IJust.h"
using namespace just::error;

#include <just/common/CommonModule.h>
#include <just/common/Version.h>
//#include <just/common/ConfigMgr.h>
#include <just/common/Debuger.h>
#include <just/common/PortManager.h>
#ifndef JUST_DISABLE_DEBUGPROXY
#include <just/common/DebugProxy.h>
#endif
using namespace just::common;

#include "just/export/Core.h"
#include "just/export/Input.h"
#include "just/export/Output.h"
#include "just/export/Server.h"
#include "just/export/P2p.h"
#include "just/export/Callback.h"

#include <framework/logger/StreamRecord.h>
#include <framework/logger/Section.h>
#include <framework/filesystem/Path.h>
using namespace framework::logger;

#include <boost/asio/error.hpp>
#include <boost/thread/thread.hpp>
using namespace boost::system;

FRAMEWORK_LOGGER_DECLARE_MODULE_LEVEL("just.IJust", framework::logger::Debug);

namespace just
{

    class IJust
        : public util::daemon::Daemon
    {
    public:
        IJust()
            : util::daemon::Daemon("just.conf")
        {

            char const * default_argv[] = {
                "++framework.logger.Stream.0.file=$LOG/just.log", 
                "++framework.logger.Stream.0.append=true", 
                "++framework.logger.Stream.0.roll=true", 
                "++framework.logger.Stream.0.level=5", 
                "++framework.logger.Stream.0.size=102400", 
            };
            parse_cmdline(sizeof(default_argv) / sizeof(default_argv[0]), default_argv);

            framework::logger::load_config(global_logger(), config());

            just::common::log_versions();

            std::string just_name = "Just_" + framework::filesystem::bin_file().stem().string();
            just::common::CommonModule & common = 
                util::daemon::use_module<just::common::CommonModule>(*this, just_name);
            common.set_version(just::version());

            //util::daemon::use_module<just::common::ConfigMgr>(*this);
            util::daemon::use_module<just::common::Debuger>(*this);
            util::daemon::use_module<just::common::PortManager>(*this);
#ifndef JUST_DISABLE_DEBUGPROXY
            util::daemon::use_module<just::common::DebugProxy>(*this);
#endif
            core_init(*this);

            input_init(*this);

            output_init(*this);

            server_init(*this);

            p2p_init(*this);

            LOG_INFO("Just ready.");
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
#ifndef JUST_DISABLE_CERTIFY           
                just::certify::Certifier & cert = 
                    util::daemon::use_module<just::certify::Certifier>(*this);
                cert.set_auth_code(gid, pid, auth);
#endif				

#ifndef JUST_DISABLE_DAC
                just::dac::DacModule & dac = 
                    util::daemon::use_module<just::dac::DacModule>(*this);
                dac.set_auth_code(gid, pid, auth);
#endif
                start(ec);
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
                stop(ec);
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
            just::error::last_error(ec);
            return just::error::last_error_enum(ec);
        }

        PP_err last_error() const
        {
            return just::error::last_error_enum();
        }

        PP_str last_error_msg() const
        {
            static std::string err_msg;
            err_msg = just::error::last_error().message();
            return err_msg.c_str();
        }

        PP_str version() const
        {
            return just::version_string();
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
                return just_logic_error;
            }
            config().set(section,key,value);
            return just_success;
        }

        PP_err submit_msg(
            PP_str msg, 
            boost::int32_t size)
        {
#ifndef JUST_DISABLE_DAC
            just::dac::DacModule & dac = 
                util::daemon::use_module<just::dac::DacModule>(*this);
            dac.submit_log(msg, size);
#endif
            return just_success;
        }

    };

    util::daemon::Daemon & global_daemon()
    {
        static IJust the_just;
        return the_just;
    }

}

just::IJust & the_just()
{
    return static_cast<just::IJust &>(just::global_daemon());
}

using namespace just;

#if __cplusplus
extern "C" {
#endif // __cplusplus

    JUST_DECL PP_err JUST_StartEngine(
        PP_str gid, 
        PP_str pid, 
        PP_str auth)
    {
        return the_just().start_engine(gid, pid, auth);
    }

    JUST_DECL PP_ushort JUST_GetPort(
        PP_str moduleName)
    {
        return the_just().get_port(moduleName);
    }

    JUST_DECL PP_err JUST_StopEngine()
    {
        return the_just().stop_engine();
    }

    JUST_DECL PP_err JUST_GetLastError()
    {
        return the_just().last_error();
    }

    JUST_DECL PP_str JUST_GetLastErrorMsg()
    {
        return the_just().last_error_msg();
    }

    JUST_DECL PP_str JUST_GetVersion()
    {
        return the_just().version();
    }

    JUST_DECL PP_err JUST_SetConfig(
        PP_str module, 
        PP_str section, 
        PP_str key, 
        PP_str value)
    {
        return the_just().set_config(module, section, key, value);
    }

    JUST_DECL PP_err JUST_SubmitMessage(
        PP_str msg, 
        PP_uint size)
    {
        return the_just().submit_msg(msg, size);
    }

#if __cplusplus
}
#endif // __cplusplus
