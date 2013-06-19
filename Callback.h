// Callback.h

#ifndef _PPBOX_PPBOX_CALLBACK_H_
#define _PPBOX_PPBOX_CALLBACK_H_

#include "IPpbox.h"

#include <ppbox/common/RedirectCall.h>

namespace ppbox
{

    inline ppbox::common::RedirectCall & redirect_call()
    {
        static ppbox::common::RedirectCall g_redirect_call;
        return g_redirect_call;
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_CALLBACK_H_
