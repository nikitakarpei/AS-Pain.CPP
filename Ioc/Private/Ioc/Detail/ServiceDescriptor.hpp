#pragma once

#include <memory>

namespace ioc
{
    class IVoidServiceFactory;
}

namespace ioc::detail
{
    class IServiceDescriptor;
    enum class ServiceLifetime;

    std::unique_ptr<IServiceDescriptor>
    makeServiceDescriptor(std::unique_ptr<IVoidServiceFactory> factoryPtr,
                          ServiceLifetime lifetime);
}
