#pragma once

#include <memory>

namespace eventbus
{
    class IEventBus;
}

namespace mediator
{
    class IBaseStreamWriter;
}

namespace mediator::detail
{
    std::unique_ptr<IBaseStreamWriter>
    makeBaseStreamWriter(std::shared_ptr<eventbus::IEventBus> eventBusPtr);
}