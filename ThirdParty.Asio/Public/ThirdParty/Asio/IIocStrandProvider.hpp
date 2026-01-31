#pragma once

#include <boost/asio/io_context_strand.hpp>

namespace thirdparty::asio
{
    class IIoContextStrandProvider
    {
    public:
        virtual ~IIoContextStrandProvider() = default;

        virtual boost::asio::io_context::strand &getStrand() = 0;
    };
}
