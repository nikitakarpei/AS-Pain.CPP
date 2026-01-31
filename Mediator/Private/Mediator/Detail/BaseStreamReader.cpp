#include <Coro/Awaitable.hpp>
#include <EventBus/EventBusSubscriber.hpp>
#include <EventBus/IEventBus.hpp>
#include <EventBus/IEventBusListener.hpp>
#include <EventBus/SharedEventBusHolder.hpp>
#include <Mediator/Detail/Events/NewStreamResponseEvent.hpp>
#include <Mediator/Detail/Events/StreamReaderDestroyedEvent.hpp>
#include <Mediator/Detail/Events/StreamWriterDestroyedEvent.hpp>
#include <Mediator/Detail/IStreamRequestHandlerRunner.hpp>
#include <Mediator/StreamReader.hpp>
#include <Task/ITask.hpp>
#include <Task/ITaskExecutor.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>

namespace mediator::detail
{
    class BaseStreamReader final
        : public IBaseStreamReader,
          public eventbus::SharedEventBusHolder,
          public eventbus::EventBusSubscriber<NewStreamResponseEvent, StreamWriterDestoyedEvent>
    {
    private:
        std::mutex mutex_;
        std::shared_ptr<IStreamResponse> currentResponsePtr_;
        std::function<void()> resumeCallback_;
        std::function<void()> onNextCallback_;
        bool hasMore_{true};

    public:
        explicit BaseStreamReader(std::shared_ptr<eventbus::IEventBus> eventBusPtr,
                                  std::shared_ptr<IStreamRequestHandlerRunner> handlerRunnerPtr)
            : SharedEventBusHolder(std::move(eventBusPtr)), EventBusSubscriber(eventBus())
        {
            resumeCallback_ = [handlerRunnerPtr{std::move(handlerRunnerPtr)}]()
            { handlerRunnerPtr->run(); };
        }

        ~BaseStreamReader() override { eventBus().emit(StreamReaderDestroyedEvent{}); }

        OptionalRef<const IStreamResponse> getCurrentOrNull() override
        {
            std::unique_lock lock(mutex_);

            if (currentResponsePtr_)
            {
                return *currentResponsePtr_;
            }
            return std::nullopt;
        }

        bool hasMore() override
        {
            std::unique_lock lock(mutex_);

            return hasMore_;
        }

        void getNext(std::function<void()> callback) override
        {
            std::unique_lock lock(mutex_);

            onNextCallback_ = std::move(callback);

            currentResponsePtr_.reset();

            if (auto resumeCallback = std::move(resumeCallback_))
            {
                resumeCallback_ = nullptr;

                lock.unlock();

                resumeCallback();
            }
        }

        void onEvent(const NewStreamResponseEvent &event) override
        {
            std::unique_lock lock(mutex_);

            currentResponsePtr_ = event.responsePtr;
            resumeCallback_ = event.continuation;

            if (auto onNextCallback = std::move(onNextCallback_))
            {
                onNextCallback_ = nullptr;

                lock.unlock();

                onNextCallback();
            }
        }

        void onEvent(const StreamWriterDestoyedEvent &) override
        {
            std::unique_lock lock(mutex_);

            hasMore_ = false;

            if (auto onNextCallback = std::move(onNextCallback_))
            {
                onNextCallback_ = nullptr;

                lock.unlock();

                onNextCallback();
            }
        }
    };

    std::unique_ptr<IBaseStreamReader>
    makeBaseStreamReader(std::shared_ptr<eventbus::IEventBus> eventBusPtr,
                         std::unique_ptr<IStreamRequestHandlerRunner> handlerRunnerPtr)
    {
        return std::make_unique<BaseStreamReader>(std::move(eventBusPtr),
                                                  std::move(handlerRunnerPtr));
    }
}