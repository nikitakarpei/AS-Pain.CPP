#pragma once

namespace eventbus
{
    class IEventBusEvent;

    class IBaseEventBusListener
    {
    public:
        virtual ~IBaseEventBusListener() = default;

        virtual void onBaseEvent(const IEventBusEvent &event) = 0;
    };

    template <typename TEvent> class IEventBusListener : public IBaseEventBusListener
    {
    public:
        virtual void onEvent(const TEvent &event) = 0;

    private:
        void onBaseEvent(const IEventBusEvent &event) final
        {
            onEvent(dynamic_cast<const TEvent &>(event));
        }
    };
}