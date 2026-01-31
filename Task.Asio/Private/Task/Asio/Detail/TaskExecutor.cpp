#include <Coro/BasicCoroutine.hpp>
#include <Coro/ThreadAwaitableExecutorContext.hpp>
#include <Task/Coro/CoroResumerFactory.hpp>
#include <Task/ITask.hpp>
#include <Task/ITaskExecutor.hpp>
#include <ThirdParty/Asio/IIocStrandProvider.hpp>
#include <memory>

namespace task::detail
{
    class AsioTaskExecutor final : public ITaskExecutor
    {
    private:
        boost::asio::io_context::strand &strand_;

        std::unique_ptr<coro::ICoroResumerFactory> coroResumerFactory_ =
            makeCoroResumerFactory(*this);

    public:
        explicit AsioTaskExecutor(boost::asio::io_context::strand &strand) : strand_(strand) {}

        void execute(std::unique_ptr<ITask> taskPtr) override
        {
            return execute(std::shared_ptr<ITask>(std::move(taskPtr)));
        }

        void execute(std::shared_ptr<ITask> taskPtr)
        {
            strand_.post(
                [this, taskPtr{std::move(taskPtr)}]()
                {
                    auto contextPtr =
                        coro::makeThreadAwaitableExecutorContext(*coroResumerFactory_);
                    [&]() -> coro::BasicCoroutine
                    {
                        auto taskPtrCopy{taskPtr};
                        co_await (*taskPtrCopy)();
                    }();
                });
        }
    };
}

namespace task
{
    std::unique_ptr<ITaskExecutor>
    makeAsioTaskExecutor(thirdparty::asio::IIoContextStrandProvider &ioContextStrandProvider)
    {
        return std::make_unique<detail::AsioTaskExecutor>(ioContextStrandProvider.getStrand());
    }
}