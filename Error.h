// Error.h

#ifndef _JUST_EXPORT_ERROR_H_
#define _JUST_EXPORT_ERROR_H_

namespace just
{

    namespace error {

        enum errors
        {
            success = 0, 
            not_start, 
            already_start, 
            not_open, 
            already_open, 
            operation_canceled, 
            would_block, 
            stream_end, 
            logic_error, 
            network_error, 
            demux_error, 
            certify_error, 
            download_error,
            other_error = 1024, 
        };

        namespace detail {

            class category
                : public boost::system::error_category
            {
            public:
                const char* name() const BOOST_SYSTEM_NOEXCEPT
                {
                    return "just/export";
                }

                std::string message(int value) const
                {
                    if (value == success)
                        return "Everything is ok";
                    if (value == not_start)
                        return "JUST has not started";
                    if (value == already_start)
                        return "JUST has already started";
                    if (value == not_open)
                        return "JUST has not opened";
                    if (value == already_open)
                        return "JUST has already opened";
                    if (value == operation_canceled)
                        return "JUST operation canceled";
                    if (value == would_block)
                        return "JUST stream would block";
                    if (value == stream_end)
                        return "JUST stream end";
                    if (value == logic_error)
                        return "JUST logic error";
                    if (value == network_error)
                        return "JUST network error";
                    if (value == demux_error)
                        return "JUST demux error";
                    if (value == certify_error)
                        return "JUST certify error";
                    if (value == download_error)
                        return "JUST download error";
                    return "JUST error";
                }
            };

        } // namespace detail

        inline const boost::system::error_category & get_category()
        {
            static detail::category instance;
            return instance;
        }

        static boost::system::error_category const & just_category = get_category();

        inline boost::system::error_code make_error_code(
            errors e)
        {
            return boost::system::error_code(
                static_cast<int>(e), get_category());
        }

        inline boost::system::error_code & __last_error()
        {
            static boost::system::error_code ec;
            return ec;
        }

        inline boost::system::error_code const & last_error()
        {
            return __last_error();
        }

        inline void last_error(
            boost::system::error_code const & ec)
        {
            __last_error() = ec;
        }

        error::errors last_error_enum(
            boost::system::error_code const & ec);

        inline error::errors last_error_enum()
        {
            return last_error_enum(last_error());
        }

    } // namespace error

} // namespace just

namespace boost
{
    namespace system
    {

        template<>
        struct is_error_code_enum<just::error::errors>
        {
            BOOST_STATIC_CONSTANT(bool, value = true);
        };

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
        using just::error::make_error_code;
#endif

    }
}

#endif // _JUST_EXPORT_ERROR_H_
