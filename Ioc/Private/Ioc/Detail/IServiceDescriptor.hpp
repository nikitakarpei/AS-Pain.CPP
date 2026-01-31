#pragma once

namespace ioc
{
    class IVoidServiceFactory;
}

namespace ioc::detail
{
    enum class ServiceLifetime;

    class IServiceDescriptor
    {
    public:
        virtual ~IServiceDescriptor() = default;

        virtual IVoidServiceFactory &factory() = 0;

        virtual ServiceLifetime lifetime() = 0;
    };
}
