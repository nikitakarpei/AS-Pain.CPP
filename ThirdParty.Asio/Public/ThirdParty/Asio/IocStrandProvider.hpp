#pragma once

#include <memory>

namespace boost::asio
{
    class io_context;
}

namespace thirdparty::asio
{
    class IIoContextStrandProvider;

    std::unique_ptr<IIoContextStrandProvider>
    makeIoContextStrandProvider(boost::asio::io_context &ioContext);
}
