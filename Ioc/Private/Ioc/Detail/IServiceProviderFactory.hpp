#pragma once

#include <memory>

namespace ioc
{
    class IServiceProvider;
}

namespace ioc::detail
{
    class IServiceScope;

    class IServiceProviderFactory
    {
    public:
        virtual ~IServiceProviderFactory() = default;

        virtual std::unique_ptr<IServiceProvider> createProvider(IServiceScope &rootScope,
                                                                 IServiceScope &localScope) = 0;
    };
}
