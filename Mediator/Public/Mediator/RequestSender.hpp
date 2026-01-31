#pragma once

#include <memory>

namespace ioc
{
    class IServiceProvider;
}

namespace task
{
    class ITaskExecutor;
}

namespace mediator
{
    class IRequestSender;

    std::unique_ptr<IRequestSender> makeRequestSender(ioc::IServiceProvider &serviceProvider,
                                                      task::ITaskExecutor &taskExecutor);
}