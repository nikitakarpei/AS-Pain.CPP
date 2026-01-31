#include <Coro/IAwaiter.hpp>
#include <EventBus/EventBusSubscriber.hpp>
#include <EventBus/IEventBus.hpp>
#include <EventBus/IEventBusListener.hpp>
#include <EventBus/SharedEventBusHolder.hpp>
#include <Mediator/Contracts/IStreamResponse.hpp>
#include <Mediator/Detail/Events/NewStreamResponseEvent.hpp>
#include <Mediator/Detail/Events/StreamReaderDestroyedEvent.hpp>
#include <Mediator/Detail/Events/StreamWriterDestroyedEvent.hpp>
#include <Mediator/StreamWriter.hpp>
#include <exception>
#include <memory>
#include <mutex>

namespace mediator::detail
{
    class BaseStreamWriter : public IBaseStreamWriter,
                             public eventbus::SharedEventBusHolder,
                             public eventbus::EventBusSubscriber<StreamReaderDestroyedEvent>
    {
    private:
        std::mutex mutex_;
        bool canWrite_{true};

    public:
        explicit BaseStreamWriter(std::shared_ptr<eventbus::IEventBus> eventBusPtr)
            : SharedEventBusHolder(std::move(eventBusPtr)), EventBusSubscriber(eventBus())
        {
        }

        ~BaseStreamWriter() override { eventBus().emit(StreamWriterDestoyedEvent{}); }

        class WriteAwaiter final : public coro::IAwaiter<void>
        {
        private:
            eventbus::IEventBus &eventBus_;
            std::unique_ptr<IStreamResponse> responsePtr_;

        public:
            explicit WriteAwaiter(eventbus::IEventBus &eventBus,
                                  std::unique_ptr<IStreamResponse> responsePtr)
                : eventBus_(eventBus), responsePtr_(std::move(responsePtr))
            {
            }

            void onCompleted(std::function<void()> continuation) override
            {
                NewStreamResponseEvent event;
                event.responsePtr = std::move(responsePtr_);
                event.continuation = std::move(continuation);

                eventBus_.emit(std::move(event));
            }

            bool isCompleted() const override { return false; }
        };

        coro::Awaitable<void> baseWrite(std::unique_ptr<IStreamResponse> responsePtr) override
        {
            std::unique_lock lock(mutex_);

            if (!canWrite_)
            {
                throw std::exception(); // TODO
            }

            lock.unlock();

            co_await WriteAwaiter(eventBus(), std::move(responsePtr));
        }

        void onEvent(const StreamReaderDestroyedEvent &) override
        {
            std::unique_lock lock(mutex_);

            canWrite_ = false;
        }
    };

    std::unique_ptr<IBaseStreamWriter>
    makeBaseStreamWriter(std::shared_ptr<eventbus::IEventBus> eventBusPtr)
    {
        return std::make_unique<BaseStreamWriter>(std::move(eventBusPtr));
    }
}