// Output.h

#ifndef _JUST_EXPORT_OUTPUT_H_
#define _JUST_EXPORT_OUTPUT_H_

#ifndef JUST_DISABLE_DOWNLOAD
#  include <just/download/DownloadModule.h>
#endif
#ifndef JUST_DISABLE_HTTPD
#  include <just/httpd/HttpdModule.h>
#endif
#ifndef JUST_DISABLE_RTSPD
#  include <just/rtspd/RtspdModule.h>
#endif
#ifndef JUST_DISABLE_MMSPD
#  include <just/mmspd/MmspdModule.h>
#endif
#ifndef JUST_DISABLE_RTMPD
#  include <just/rtmpd/RtmpdModule.h>
#endif

namespace just
{

    void output_init(
        util::daemon::Daemon & daemon)
    {
#ifndef JUST_DISABLE_DOWNLOAD
        util::daemon::use_module<just::download::DownloadModule>(daemon);
#endif
#ifndef JUST_DISABLE_HTTPD
        util::daemon::use_module<just::httpd::HttpdModule>(daemon);
#endif
#ifndef JUST_DISABLE_RTSPD
        util::daemon::use_module<just::rtspd::RtspdModule>(daemon);
#endif
#ifndef JUST_DISABLE_MMSPD
        util::daemon::use_module<just::mmspd::MmspdModule>(daemon);
#endif
#ifndef JUST_DISABLE_RTMPD
        util::daemon::use_module<just::rtmpd::RtmpdModule>(daemon);
#endif
    }

} // namespace just

#endif // _JUST_EXPORT_OUTPUT_H_
