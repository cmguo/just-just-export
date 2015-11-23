// P2p.h

#ifndef _JUST_EXPORT_P2P_H_
#define _JUST_EXPORT_P2P_H_

#ifndef JUST_DISABLE_PEER
#  include <just/peer/PeerModule.h>
#endif
#ifndef JUST_DISABLE_LIVE
#  include <just/live/LiveModule.h>
#endif
#ifndef JUST_DISABLE_TRIP
#  include <just/trip/TripModule.h>
#endif

namespace just
{

    void p2p_init(
        util::daemon::Daemon & daemon)
    {
#ifndef JUST_DISABLE_PEER
        util::daemon::use_module<just::peer::PeerModule>(daemon);
#endif
#ifndef JUST_DISABLE_LIVE
        util::daemon::use_module<just::live::LiveModule>(daemon);
#endif
#ifndef JUST_DISABLE_TRIP
        util::daemon::use_module<just::trip::TripModule>(daemon);
#endif
    }

} // namespace just

#endif // _JUST_EXPORT_P2P_H_
