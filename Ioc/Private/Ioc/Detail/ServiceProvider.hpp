#pragma once

#include <memory>

namespace ioc
{
    class IServiceProvider;
}

namespace ioc::detail
{
    class IIocContainer;
    class IServiceScope;

    std::unique_ptr<IServiceProvider> makeServiceProvider(IIocContainer &container,
                                                          IServiceScope &rootScope,
                                                          IServiceScope &localScope);
}
