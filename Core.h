// Core.h

#ifndef _PPBOX_PPBOX_CORE_H_
#define _PPBOX_PPBOX_CORE_H_

#include <ppbox/data/DataModule.h>
#include <ppbox/demux/DemuxModule.h>
#include <ppbox/merge/MergeModule.h>
#  include <ppbox/mux/MuxModule.h>

namespace ppbox
{

    void core_init(
        util::daemon::Daemon & daemon)
    {
        util::daemon::use_module<ppbox::data::DataModule>(daemon);
        util::daemon::use_module<ppbox::demux::DemuxModule>(daemon);
        util::daemon::use_module<ppbox::merge::MergeModule>(daemon);
        util::daemon::use_module<ppbox::mux::MuxModule>(daemon);
    }

} // namespace ppbox

#endif // _PPBOX_PPBOX_CORE_H_
