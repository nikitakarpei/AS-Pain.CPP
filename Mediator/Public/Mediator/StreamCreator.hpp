#pragma once

#include <memory>

namespace eventbus
{
    class IEventBusFactory;
}

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
    class IStreamCreator;

    std::unique_ptr<IStreamCreator> makeStreamCreator(eventbus::IEventBusFactory &eventBusFactory,
                                                      ioc::IServiceProvider &serviceProvider,
                                                      task::ITaskExecutor &taskExecutor);
}