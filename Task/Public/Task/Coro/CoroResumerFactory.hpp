#pragma once

#include <memory>

namespace coro
{
    class ICoroResumerFactory;
}

namespace task
{
    class ITaskExecutor;

    std::unique_ptr<coro::ICoroResumerFactory> makeCoroResumerFactory(ITaskExecutor &executor);
}