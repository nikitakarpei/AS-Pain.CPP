#pragma once

namespace mediator::detail
{
    class IStreamRequestHandlerRunner
    {
    public:
        virtual ~IStreamRequestHandlerRunner() = default;

        virtual void run() = 0;
    };
}