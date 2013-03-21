// P2p.h

#ifndef _PPBOX_PPBOX_P2P_H_
#define _PPBOX_PPBOX_P2P_H_

#ifndef PPBOX_DISABLE_PEER 
#  include <ppbox/peer/PeerModule.h>
#endif
#ifndef PPBOX_DISABLE_LIVE 
#  include <ppbox/live/LiveModule.h>
#endif

namespace ppbox
{

    void p2p_init(
        util::daemon::Daemon & daemon)
    {
#ifndef PPBOX_DISABLE_PEER
        util::daemon::use_module<ppbox::peer::PeerModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_LIVE
        util::daemon::use_module<ppbox::live::LiveModule>(daemon);
#endif
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_P2P_H_
