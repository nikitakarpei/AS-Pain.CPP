#pragma once

#include <EventBus/IEventBusEvent.hpp>

namespace mediator::detail
{
    struct StreamReaderDestroyedEvent : eventbus::IEventBusEvent
    {
    };
}