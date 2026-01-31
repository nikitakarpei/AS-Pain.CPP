#pragma once

#include <memory>

namespace coro
{
    class ICoroResumer;

    class ICoroResumerFactory
    {
    public:
        virtual ~ICoroResumerFactory() = default;

        virtual std::unique_ptr<ICoroResumer> create() = 0;
    };
}