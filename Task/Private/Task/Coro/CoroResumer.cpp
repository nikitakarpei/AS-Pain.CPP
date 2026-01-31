#include <Coro/ICoroResumer.hpp>
#include <Coro/ICoroResumerFactory.hpp>
#include <Task/ITask.hpp>
#include <Task/ITaskExecutor.hpp>

namespace task
{
    class CoroResumer final : public coro::ICoroResumer
    {
    private:
        ITaskExecutor &executor_;

    public:
        explicit CoroResumer(ITaskExecutor &executor) : executor_(executor) {}

        void resume(std::coroutine_handle<> handle) override
        {
            class ResumeTask final : public ITask
            {
            private:
                std::coroutine_handle<> handle_;

            public:
                explicit ResumeTask(std::coroutine_handle<> handle) : handle_(std::move(handle)) {}

                coro::Awaitable<void> operator()() override
                {
                    handle_.resume();
                    co_return;
                }
            };

            executor_.execute(std::make_unique<ResumeTask>(std::move(handle)));
        }
    };

    class CoroResumerFactory final : public coro::ICoroResumerFactory
    {
    private:
        ITaskExecutor &executor_;

    public:
        explicit CoroResumerFactory(ITaskExecutor &executor) : executor_(executor) {}

        std::unique_ptr<coro::ICoroResumer> create() override
        {
            return std::make_unique<CoroResumer>(executor_);
        }
    };

    std::unique_ptr<coro::ICoroResumerFactory> makeCoroResumerFactory(ITaskExecutor &executor)
    {
        return std::make_unique<CoroResumerFactory>(executor);
    }
}