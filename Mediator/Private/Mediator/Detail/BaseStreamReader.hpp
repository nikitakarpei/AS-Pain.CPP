#pragma once

#include <memory>

namespace eventbus
{
    class IEventBus;
}

namespace mediator
{
    class IBaseStreamReader;
}

namespace mediator::detail
{
    class IStreamRequestHandlerRunner;

    std::unique_ptr<IBaseStreamReader>
    makeBaseStreamReader(std::shared_ptr<eventbus::IEventBus> eventBusPtr,
                         std::unique_ptr<IStreamRequestHandlerRunner> handlerRunnerPtr);
}