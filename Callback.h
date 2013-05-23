// Callback.h

#ifndef _PPBOX_PPBOX_CALLBACK_H_
#define _PPBOX_PPBOX_CALLBACK_H_

#include "IPpbox.h"

#include <ppbox/common/VargCall.h>

namespace ppbox
{

    inline ppbox::common::VargCall & varg_call()
    {
        static ppbox::common::VargCall g_varg_call;
        return g_varg_call;
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_CALLBACK_H_
