// Input.h

#ifndef _PPBOX_PPBOX_INPUT_H_
#define _PPBOX_PPBOX_INPUT_H_

#ifndef PPBOX_DISABLE_RTMPC
#include <ppbox/rtmpc/RtmpcModule.h>
#endif

#ifndef PPBOX_DISABLE_MMSPC
#include <ppbox/mmspc/MmspcModule.h>
#endif

namespace ppbox
{

    void input_init(
        util::daemon::Daemon & daemon)
    {
#ifndef PPBOX_DISABLE_RTMPC
        util::daemon::use_module<ppbox::rtmpc::RtmpcModule>(daemon);
#endif
#ifndef PPBOX_DISABLE_MMSPC
        util::daemon::use_module<ppbox::mmspc::MmspcModule>(daemon);
#endif
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_INPUT_H_
