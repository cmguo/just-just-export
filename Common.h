// Common.h

#include <ppbox/common/Common.h>

#include "ppbox/ppbox/Error.h"

namespace ppbox
{
    util::daemon::Daemon & global_daemon();
}

#ifdef BOOST_HAS_DECLSPEC
#  define PPBOX_DECL __declspec(dllexport)
#else
#  define PPBOX_DECL __attribute__ ((visibility("default")))
#endif

#include "ppbox/ppbox/IPpboxBoostTypes.h"
#include "ppbox/ppbox/import_func.h"
