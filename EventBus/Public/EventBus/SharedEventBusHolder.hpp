#pragma once

#include <memory>

namespace eventbus
{
    class IEventBus;

    class SharedEventBusHolder
    {
    private:
        std::shared_ptr<IEventBus> eventBusPtr_;

    public:
        explicit SharedEventBusHolder(std::shared_ptr<IEventBus> eventBusPtr)
            : eventBusPtr_(std::move(eventBusPtr))
        {
        }

        IEventBus &eventBus() { return *eventBusPtr_; }
    };
}