// Core.h

#ifndef _JUST_EXPORT_CORE_H_
#define _JUST_EXPORT_CORE_H_

#include <just/data/DataModule.h>
#include <just/demux/DemuxModule.h>
#ifndef JUST_DISABLE_MERGE
#include <just/merge/MergeModule.h>
#endif
#include <just/mux/MuxModule.h>

namespace just
{

    void core_init(
        util::daemon::Daemon & daemon)
    {
        util::daemon::use_module<just::data::DataModule>(daemon);
        util::daemon::use_module<just::demux::DemuxModule>(daemon);
#ifndef JUST_DISABLE_MERGE
        util::daemon::use_module<just::merge::MergeModule>(daemon);
#endif
        util::daemon::use_module<just::mux::MuxModule>(daemon);
    }

} // namespace just

#endif // _JUST_EXPORT_CORE_H_
