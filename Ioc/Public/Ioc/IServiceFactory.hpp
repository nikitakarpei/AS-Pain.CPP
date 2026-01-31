#pragma once

#include <memory>

namespace ioc
{
    class IServiceProvider;

    class IVoidServiceFactory
    {
    public:
        virtual ~IVoidServiceFactory() = default;

        virtual std::shared_ptr<void> createServiceVoid(ioc::IServiceProvider &serviceProvider) = 0;
    };

    template <class TService> class IServiceFactory : public IVoidServiceFactory
    {
    public:
        virtual std::shared_ptr<TService> createService(IServiceProvider &serviceProvider) = 0;

    private:
        std::shared_ptr<void> createServiceVoid(IServiceProvider &serviceProvider) final
        {
            return createService(serviceProvider);
        }
    };
}
