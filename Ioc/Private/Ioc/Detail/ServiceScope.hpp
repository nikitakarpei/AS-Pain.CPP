#pragma once

#include <memory>

namespace ioc::detail
{
    class IServiceProviderFactory;
    class IServiceScope;

    std::unique_ptr<IServiceScope> makeServiceScope(IServiceProviderFactory &providerFactory);

    std::unique_ptr<IServiceScope> makeServiceScope(IServiceProviderFactory &providerFactory,
                                                    IServiceScope &rootScope);
}
