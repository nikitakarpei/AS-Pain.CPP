#pragma once

#include <memory>

namespace thirdparty::asio
{
    class IIoContextStrandProvider;
}

namespace task
{
    class ITaskExecutor;

    std::unique_ptr<ITaskExecutor>
    makeAsioTaskExecutor(thirdparty::asio::IIoContextStrandProvider &ioContextStrandProvider);
}
