#include <Ioc/IServiceFactory.hpp>
#include <memory>
#include <utility>

#include "IServiceDescriptor.hpp"

namespace ioc::detail
{
    class ServiceDescriptor : public IServiceDescriptor
    {
    private:
        std::unique_ptr<IVoidServiceFactory> factoryPtr_;
        ServiceLifetime lifetime_;

    public:
        explicit ServiceDescriptor(std::unique_ptr<IVoidServiceFactory> factoryPtr,
                                   ServiceLifetime lifetime)
            : factoryPtr_(std::move(factoryPtr)), lifetime_(lifetime)
        {
        }

        IVoidServiceFactory &factory() override { return *factoryPtr_; }

        ServiceLifetime lifetime() override { return lifetime_; }
    };

    std::unique_ptr<IServiceDescriptor>
    makeServiceDescriptor(std::unique_ptr<IVoidServiceFactory> factoryPtr, ServiceLifetime lifetime)
    {
        return std::make_unique<ServiceDescriptor>(std::move(factoryPtr), lifetime);
    }
}
