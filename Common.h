// Common.h

#include <just/common/Common.h>

#include "just/just/Error.h"

namespace just
{
    util::daemon::Daemon & global_daemon();
}

#ifdef BOOST_HAS_DECLSPEC
#  define JUST_DECL __declspec(dllexport)
#else
#  define JUST_DECL __attribute__ ((visibility("default")))
#endif

#include "just/just/IPpboxBoostTypes.h"
#include "just/just/IPpbox.h"
