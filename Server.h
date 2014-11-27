// Server.h

#ifndef _JUST_JUST_SERVER_H_
#define _JUST_JUST_SERVER_H_

#ifndef JUST_DISABLE_CERTIFY 
#  include <just/certify/Certifier.h>
#endif
#ifndef JUST_DISABLE_DAC 
#  include <just/dac/DacModule.h>
#endif
#include <just/cdn/CdnModule.h>

namespace just
{

    void server_init(
        util::daemon::Daemon & daemon)
    {
#ifndef JUST_DISABLE_CERTIFY
        util::daemon::use_module<just::certify::Certifier>(daemon);
#endif
#ifndef JUST_DISABLE_DAC
        util::daemon::use_module<just::dac::DacModule>(daemon);
#endif
        util::daemon::use_module<just::cdn::CdnModule>(daemon);
    }

} // namespace just

#endif // _JUST_JUST_SERVER_H_
