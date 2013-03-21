// Output.h

#ifndef _PPBOX_PPBOX_OUTPUT_H_
#define _PPBOX_PPBOX_OUTPUT_H_

#ifndef PPBOX_DISABLE_DOWNLOAD
#  include <ppbox/download/DownloadModule.h>
#endif
#ifndef PPBOX_DISABLE_HTTPD
#  include <ppbox/httpd/HttpdModule.h>
#endif
#ifndef PPBOX_DISABLE_RTSPD
#  include <ppbox/rtspd/RtspdModule.h>
#endif
#ifndef PPBOX_DISABLE_MMSPD
#  include <ppbox/mmspd/MmspdModule.h>
#endif
#ifndef PPBOX_DISABLE_RTMPD
#  include <ppbox/rtmpd/RtmpdModule.h>
#endif

namespace ppbox
{

    void output_init(
        util::daemon::Daemon & daemon)
    {
#ifndef PPBOX_DISABLE_DOWNLOAD
        util::daemon::use_module<ppbox::download::DownloadModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_HTTPD
        util::daemon::use_module<ppbox::httpd::HttpdModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_RTSPD
        util::daemon::use_module<ppbox::rtspd::RtspdModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_MMSPD
        util::daemon::use_module<ppbox::mmspd::MmspdModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_RTMPD
        util::daemon::use_module<ppbox::rtmpd::RtmpdModule>(daemon);
#endif
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_OUTPUT_H_
