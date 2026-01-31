#include <EventBus/IEventBus.hpp>
#include <EventBus/IEventBusFactory.hpp>
#include <Ioc/IServiceProvider.hpp>
#include <Mediator/Detail/BaseStreamReader.hpp>
#include <Mediator/Detail/BaseStreamWriter.hpp>
#include <Mediator/Detail/IStreamRequestHandlerRunner.hpp>
#include <Mediator/IStreamCreator.hpp>
#include <Mediator/IStreamRequestHandler.hpp>
#include <Task/ITask.hpp>
#include <Task/ITaskExecutor.hpp>
#include <memory>
#include <utility>

namespace mediator::detail
{
    class StreamCreator final : public IStreamCreator
    {
    private:
        eventbus::IEventBusFactory &eventBusFactory_;
        ioc::IServiceProvider &serviceProvider_;
        task::ITaskExecutor &taskExecutor_;

    public:
        explicit StreamCreator(eventbus::IEventBusFactory &eventBusFactory,
                               ioc::IServiceProvider &serviceProvider,
                               task::ITaskExecutor &taskExecutor)
            : eventBusFactory_(eventBusFactory), serviceProvider_(serviceProvider),
              taskExecutor_(taskExecutor)
        {
        }

        std::unique_ptr<IBaseStreamReader>
        createBaseStream(const std::type_index &requestHandlerTypeIndex,
                         std::unique_ptr<IBaseStreamRequest> requestPtr) override
        {
            std::shared_ptr<IBaseStreamRequestHandler> requestHandlerPtr =
                serviceProvider_.getRequiredService<IBaseStreamRequestHandler>(
                    requestHandlerTypeIndex);

            std::shared_ptr<eventbus::IEventBus> eventBusPtr = eventBusFactory_.create();

            std::unique_ptr<IBaseStreamWriter> streamWriterPtr = makeBaseStreamWriter(eventBusPtr);

            std::unique_ptr<IBaseStreamReader> streamReaderPtr = makeBaseStreamReader(
                eventBusPtr,
                std::make_unique<StreamRequestHandlerRunner>(
                    taskExecutor_, std::make_unique<HandleTask>(std::move(requestHandlerPtr),
                                                                std::move(requestPtr),
                                                                std::move(streamWriterPtr))));

            return streamReaderPtr;
        }

        class StreamRequestHandlerRunner final : public IStreamRequestHandlerRunner
        {
        private:
            task::ITaskExecutor &taskExecutor_;
            std::unique_ptr<task::ITask> handleTaskPtr_;

        public:
            explicit StreamRequestHandlerRunner(task::ITaskExecutor &taskExecutor,
                                                std::unique_ptr<task::ITask> handleTaskPtr)
                : taskExecutor_(taskExecutor), handleTaskPtr_(std::move(handleTaskPtr))
            {
            }

            void run() override { taskExecutor_.execute(std::move(handleTaskPtr_)); }
        };

        class HandleTask final : public task::ITask
        {
        private:
            std::shared_ptr<IBaseStreamRequestHandler> requestHandlerPtr_;
            std::unique_ptr<IBaseStreamRequest> requestPtr_;
            std::unique_ptr<IBaseStreamWriter> streamWriterPtr_;

        public:
            explicit HandleTask(std::shared_ptr<IBaseStreamRequestHandler> requestHandlerPtr,
                                std::unique_ptr<IBaseStreamRequest> requestPtr,
                                std::unique_ptr<IBaseStreamWriter> streamWriterPtr)
                : requestHandlerPtr_(std::move(requestHandlerPtr)),
                  requestPtr_(std::move(requestPtr)), streamWriterPtr_(std::move(streamWriterPtr))
            {
            }

            coro::Awaitable<void> operator()() override
            {
                co_await requestHandlerPtr_->baseHandle(*requestPtr_, *streamWriterPtr_);
            }
        };
    };
}

namespace mediator
{
    std::unique_ptr<IStreamCreator> makeStreamCreator(eventbus::IEventBusFactory &eventBusFactory,
                                                      ioc::IServiceProvider &serviceProvider,
                                                      task::ITaskExecutor &taskExecutor)
    {
        return std::make_unique<detail::StreamCreator>(eventBusFactory, serviceProvider,
                                                       taskExecutor);
    }
}