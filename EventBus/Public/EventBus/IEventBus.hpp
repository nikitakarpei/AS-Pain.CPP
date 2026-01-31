#pragma once

#include <EventBus/IEventBusListener.hpp>
#include <typeindex>

namespace eventbus
{
    class IEventBusEvent;

    class IBaseEventBusListener;

    class IEventBus
    {
    public:
        virtual ~IEventBus() = default;

        virtual void emit(const IEventBusEvent &event) = 0;

        template <class TEvent> void subscribe(IEventBusListener<TEvent> &listener)
        {
            subscribe(typeid(TEvent), static_cast<IBaseEventBusListener &>(listener));
        }

        template <class TEvent> void unsubscribe(IEventBusListener<TEvent> &listener)
        {
            unsubscribe(typeid(TEvent), static_cast<IBaseEventBusListener &>(listener));
        }

    protected:
        virtual void subscribe(const std::type_index &eventTypeIndex,
                               IBaseEventBusListener &baseListener) = 0;

        virtual void unsubscribe(const std::type_index &eventTypeIndex,
                                 IBaseEventBusListener &baseListener) = 0;
    };
}