#include <Coro/ICoroResumerFactory.hpp>
#include <Coro/IThreadAwaitableExecutorContext.hpp>
#include <exception>

namespace coro
{
    class ThreadAwaitableExecutorContext final : public IThreadAwaitableExecutorContext
    {
    private:
        static thread_local ThreadAwaitableExecutorContext *instancePtr_;

        ICoroResumerFactory &coroResumerFactory_;

    public:
        explicit ThreadAwaitableExecutorContext(ICoroResumerFactory &coroResumerFactory)
            : coroResumerFactory_(coroResumerFactory)
        {
            if (instancePtr_)
            {
                throw std::exception(); // TODO
            }

            instancePtr_ = this;
        }

        ~ThreadAwaitableExecutorContext() { instancePtr_ = nullptr; }

        static ThreadAwaitableExecutorContext &instance()
        {
            if (instancePtr_)
            {
                return *instancePtr_;
            }

            throw std::exception(); // TODO
        }

        ICoroResumerFactory &coroResumerFactory() override { return coroResumerFactory_; }
    };

    inline thread_local ThreadAwaitableExecutorContext
        *ThreadAwaitableExecutorContext::instancePtr_ = nullptr;

    IThreadAwaitableExecutorContext &IThreadAwaitableExecutorContext::instance()
    {
        return ThreadAwaitableExecutorContext::instance();
    }

    std::unique_ptr<IThreadAwaitableExecutorContext>
    makeThreadAwaitableExecutorContext(ICoroResumerFactory &coroResumerFactory)
    {
        return std::make_unique<ThreadAwaitableExecutorContext>(coroResumerFactory);
    }
}