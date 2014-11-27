// Input.h

#ifndef _JUST_JUST_INPUT_H_
#define _JUST_JUST_INPUT_H_

#ifndef JUST_DISABLE_CAPTURE
#include <just/capture/CaptureModule.h>
#endif

#ifndef JUST_DISABLE_RTMPC
#include <just/rtmpc/RtmpcModule.h>
#endif

#ifndef JUST_DISABLE_MMSPC
#include <just/mmspc/MmspcModule.h>
#endif

#ifndef JUST_DISABLE_RTSPC
#include <just/rtspc/RtspcModule.h>
#endif

namespace just
{

    void input_init(
        util::daemon::Daemon & daemon)
    {
#ifndef JUST_DISABLE_CAPTURE
        util::daemon::use_module<just::capture::CaptureModule>(daemon);
#endif
#ifndef JUST_DISABLE_RTMPC
        util::daemon::use_module<just::rtmpc::RtmpcModule>(daemon);
#endif
#ifndef JUST_DISABLE_MMSPC
        util::daemon::use_module<just::mmspc::MmspcModule>(daemon);
#endif
#ifndef JUST_DISABLE_RTSPC
        util::daemon::use_module<just::rtspc::RtspcModule>(daemon);
#endif
    }

} // namespace just

#endif // _JUST_JUST_INPUT_H_
