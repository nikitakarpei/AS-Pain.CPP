#pragma once

#include <memory>

namespace task
{
    class ITask;

    class ITaskExecutor
    {
    public:
        virtual ~ITaskExecutor() = default;

        virtual void execute(std::unique_ptr<ITask> taskPtr) = 0;
    };
}
