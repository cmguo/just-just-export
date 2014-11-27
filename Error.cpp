// IPpbox.cpp

#include "just/just/Common.h"

#include <just/certify/CertifyError.h>
#include <just/demux/base/DemuxError.h>
#include <just/mux/MuxError.h>

#include <util/protocol/http/HttpError.h>

#include <boost/asio/error.hpp>

namespace just
{

    namespace error
    {

        error::errors last_error_enum(
            boost::system::error_code const & ec)
        {
            if (!ec) {
                return success;
            } else if (ec == boost::asio::error::would_block) {
                return would_block;
            } else if (ec == just::avformat::error::end_of_stream) {
                return stream_end;
            } else if (ec == boost::asio::error::operation_aborted) {
                return operation_canceled;
            } else if (ec == boost::asio::error::connection_aborted) {
                return operation_canceled;
            } else if (ec.category() == just::error::get_category()) {
                return (error::errors)ec.value();
            } else if (ec.category() == just::certify::error::get_category()) {
                return certify_error;
#ifdef JUST_CERTIFY_ERROR_AUTH_ERRORS
            } else if (ec.category() == just::certify::error::get_auth_category()) {
                return certify_error;
#endif
#ifdef JUST_CERTIFY_ERROR_MUTEX_ERRORS
            } else if (ec.category() == just::certify::error::get_mutex_category()) {
                return certify_error;
#endif
            } else if (ec.category() == just::demux::error::get_category()) {
                return demux_error;
            } else if (ec.category() == framework::system::logic_error::get_category()) {
                return just::error::logic_error;
            } else if (ec.category() == util::protocol::http_error::get_category()) {
                return network_error;
            } else if (ec.category() == boost::asio::error::get_system_category()) {
                return network_error;
            } else if (ec.category() == boost::asio::error::get_netdb_category()) {
                return network_error;
            } else if (ec.category() == boost::asio::error::get_addrinfo_category()) {
                return network_error;
            } else if (ec.category() == boost::asio::error::get_misc_category()) {
                return network_error;
            } else {
                return other_error;
            }
        }

    } // namespace error

} // namespace just
