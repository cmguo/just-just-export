// Core.h

#ifndef _JUST_EXPORT_CORE_H_
#define _JUST_EXPORT_CORE_H_

#include <just/data/DataModule.h>
#include <just/demux/DemuxModule.h>
#include <just/merge/MergeModule.h>
#  include <just/mux/MuxModule.h>

namespace just
{

    void core_init(
        util::daemon::Daemon & daemon)
    {
        util::daemon::use_module<just::data::DataModule>(daemon);
        util::daemon::use_module<just::demux::DemuxModule>(daemon);
        util::daemon::use_module<just::merge::MergeModule>(daemon);
        util::daemon::use_module<just::mux::MuxModule>(daemon);
    }

} // namespace just

#endif // _JUST_EXPORT_CORE_H_
