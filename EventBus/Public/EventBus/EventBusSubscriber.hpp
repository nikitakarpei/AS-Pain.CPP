#pragma once

#include <EventBus/IEventBus.hpp>

namespace eventbus
{
    template <class... TEvents> class EventBusSubscriber : protected IEventBusListener<TEvents>...
    {
    private:
        IEventBus &eventBus_;

    protected:
        explicit EventBusSubscriber(IEventBus &eventBus) : eventBus_(eventBus)
        {
            (eventBus_.subscribe<TEvents>(*this), ...);
        }

        virtual ~EventBusSubscriber() { (eventBus_.unsubscribe<TEvents>(*this), ...); }

        using IEventBusListener<TEvents>::onEvent...;
    };
}