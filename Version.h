// Version.h

#ifndef _PPBOX_PPBOX_VERSION_H_
#define _PPBOX_PPBOX_VERSION_H_

#include "ppbox/ppbox/Define.h"

#include <framework/system/AutoVersion.h>

#include <ppbox/common/Version.h>
#ifndef PPBOX_DISABLE_CERTIFY
#  include <ppbox/certify/Version.h>
#endif
#include <ppbox/demux/Version.h>
#ifndef PPBOX_DISABLE_DAC
#  include <ppbox/dac/Version.h>
#endif
#ifndef PPBOX_DISABLE_MUX
#  include <ppbox/mux/Version.h>
#endif
#ifndef PPBOX_DISABLE_DOWNLOAD
#  include <ppbox/download/Version.h>
#endif
#ifndef PPBOX_DISABLE_LIVE
#  include <ppbox/live/Version.h>
#endif
#ifndef PPBOX_DISABLE_VOD
#  include <ppbox/vod/Version.h>
#endif
#ifndef PPBOX_DISABLE_PEER
#  include <ppbox/peer/Version.h>
#endif

#endif // _PPBOX_PPBOX_VERSION_H_
