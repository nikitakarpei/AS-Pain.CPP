#pragma once

#include <memory>

namespace ioc::detail
{
    class IIocContainer;
    class IServiceProviderFactory;

    std::unique_ptr<IServiceProviderFactory> makeServiceProviderFactory(IIocContainer &container);
}
