#pragma once

namespace eventbus
{
    class IEventBusEvent
    {
    public:
        virtual ~IEventBusEvent() = default;
    };
}
