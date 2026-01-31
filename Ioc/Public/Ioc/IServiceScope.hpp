#pragma once

#include <memory>

namespace ioc
{
    class IServiceProvider;

    class IServiceScope
    {
    public:
        virtual ~IServiceScope() = default;

        virtual std::unique_ptr<IServiceScope> createChildScope() = 0;

        virtual std::unique_ptr<IServiceProvider> createServiceProvider() = 0;
    };
}
