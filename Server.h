// Server.h

#ifndef _PPBOX_PPBOX_SERVER_H_
#define _PPBOX_PPBOX_SERVER_H_

#ifndef PPBOX_DISABLE_CERTIFY 
#  include <ppbox/certify/Certifier.h>
#endif
#ifndef PPBOX_DISABLE_DAC 
#  include <ppbox/dac/DacModule.h>
#endif
#include <ppbox/cdn/CdnModule.h>

namespace ppbox
{

    void server_init(
        util::daemon::Daemon & daemon)
    {
#ifndef PPBOX_DISABLE_CERTIFY
        util::daemon::use_module<ppbox::certify::Certifier>(daemon);
#endif
#ifndef PPBOX_DISABLE_DAC
        util::daemon::use_module<ppbox::dac::DacModule>(daemon);
#endif
        util::daemon::use_module<ppbox::cdn::CdnModule>(daemon);
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_SERVER_H_
