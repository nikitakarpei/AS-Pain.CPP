#pragma once

namespace coro
{
    class ICoroResumerFactory;

    class IThreadAwaitableExecutorContext
    {
    public:
        static IThreadAwaitableExecutorContext &instance();

        virtual ~IThreadAwaitableExecutorContext() = default;

        virtual ICoroResumerFactory &coroResumerFactory() = 0;
    };
}