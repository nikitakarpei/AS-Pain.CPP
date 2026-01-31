#pragma once

#include <EventBus/IEventBusEvent.hpp>
#include <functional>
#include <memory>

namespace mediator
{
    class IStreamResponse;
}

namespace mediator::detail
{
    struct NewStreamResponseEvent : eventbus::IEventBusEvent
    {
        std::shared_ptr<IStreamResponse> responsePtr;
        std::function<void()> continuation;
    };
}