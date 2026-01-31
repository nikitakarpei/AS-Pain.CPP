#pragma once

#include <EventBus/IEventBusEvent.hpp>

namespace mediator::detail
{
    struct StreamWriterDestoyedEvent : eventbus::IEventBusEvent
    {
    };
}