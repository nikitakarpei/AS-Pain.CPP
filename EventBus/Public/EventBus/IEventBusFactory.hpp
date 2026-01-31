#pragma once

#include <memory>

namespace eventbus
{
    class IEventBus;

    class IEventBusFactory
    {
    public:
        virtual ~IEventBusFactory() = default;

        virtual std::unique_ptr<IEventBus> create() = 0;
    };
}