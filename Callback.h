// Callback.h

#ifndef _JUST_JUST_CALLBACK_H_
#define _JUST_JUST_CALLBACK_H_

#include <just/common/RedirectCall.h>

namespace just
{

    inline just::common::RedirectCall & redirect_call()
    {
        static just::common::RedirectCall g_redirect_call;
        return g_redirect_call;
    }

} // namespace just

#endif // _JUST_JUST_CALLBACK_H_
