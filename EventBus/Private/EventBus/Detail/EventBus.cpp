#include <EventBus/IEventBus.hpp>
#include <EventBus/IEventBusEvent.hpp>
#include <EventBus/IEventBusFactory.hpp>
#include <EventBus/IEventBusListener.hpp>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace eventbus::detail
{
    class EventBus final : public IEventBus
    {
    private:
        struct ListenerInfo
        {
            IBaseEventBusListener *baseListenerPtr{nullptr};
            bool wasDeleted{false};
        };

        std::unordered_map<std::type_index, std::unordered_map<void *, ListenerInfo>> listeners_;
        std::size_t depth_{0};
        std::vector<std::function<void()>> postponedActions_;

    public:
        void emit(const IEventBusEvent &event) override
        {
            auto iterator = listeners_.find(typeid(event));
            if (iterator == listeners_.end())
            {
                return;
            }

            ++depth_;

            for (auto &[_, info] : iterator->second)
            {
                if (info.wasDeleted)
                {
                    continue;
                }

                try
                {
                    info.baseListenerPtr->onBaseEvent(event);
                }
                catch (...)
                {
                    // TODO
                }
            }

            --depth_;

            if (depth_ == 0)
            {
                auto actions = std::move(postponedActions_);

                for (const auto &action : actions)
                {
                    action();
                }
            }
        }

        void subscribe(const std::type_index &eventTypeIndex,
                       IBaseEventBusListener &baseListener) override
        {
            auto action = [this, eventTypeIndex, &baseListener]()
            {
                IBaseEventBusListener *baseListenerPtr = std::addressof(baseListener);

                ListenerInfo info{.baseListenerPtr = baseListenerPtr};

                listeners_[eventTypeIndex].emplace(baseListenerPtr, info);
            };

            if (depth_ > 0)
            {
                postponedActions_.push_back(std::move(action));
            }
            else
            {
                action();
            }
        }

        void unsubscribe(const std::type_index &eventTypeIndex,
                         IBaseEventBusListener &baseListener) override
        {
            auto action = [this, eventTypeIndex, &baseListener]()
            { listeners_[eventTypeIndex].erase(std::addressof(baseListener)); };

            if (depth_ > 0)
            {
                auto &eventListeners = listeners_[eventTypeIndex];

                auto iterator = eventListeners.find(std::addressof(baseListener));
                if (iterator == eventListeners.end())
                {
                    return;
                }

                ListenerInfo &listenerInfo = iterator->second;
                listenerInfo.wasDeleted = true;

                postponedActions_.push_back(std::move(action));
            }
            else
            {
                action();
            }
        }
    };

    class EventBusFactory final : public IEventBusFactory
    {
    public:
        std::unique_ptr<IEventBus> create() override { return std::make_unique<EventBus>(); }
    };
}

namespace eventbus
{
    std::unique_ptr<IEventBusFactory> makeEventBusFactory()
    {
        return std::make_unique<detail::EventBusFactory>();
    }
}
