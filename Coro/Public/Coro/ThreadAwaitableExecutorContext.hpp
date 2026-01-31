#pragma once

#include <memory>

namespace coro
{
    class ICoroResumerFactory;
    class IThreadAwaitableExecutorContext;

    std::unique_ptr<IThreadAwaitableExecutorContext>
    makeThreadAwaitableExecutorContext(ICoroResumerFactory &coroResumerFactory);
}