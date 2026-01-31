#include <Ioc/Detail/IIocContainer.hpp>
#include <Ioc/Detail/IServiceDescriptor.hpp>
#include <Ioc/Detail/IServiceDescriptors.hpp>
#include <Ioc/Detail/IServiceScope.hpp>
#include <Ioc/Detail/ServiceLifetime.hpp>
#include <Ioc/IServiceFactory.hpp>
#include <Ioc/IServiceProvider.hpp>
#include <Utility/OptionalRef.hpp>
#include <exception>
#include <memory>
#include <typeindex>
#include <vector>

namespace ioc::detail
{
    class ServiceProvider final : public ioc::IServiceProvider
    {
    private:
        IIocContainer &container_;
        IServiceScope &rootScope_;
        IServiceScope &localScope_;

    public:
        explicit ServiceProvider(IIocContainer &container, IServiceScope &rootScope,
                                 IServiceScope &localScope)
            : container_(container), rootScope_(rootScope), localScope_(localScope)
        {
        }

        std::shared_ptr<void> getRequiredVoidService(const std::type_index &typeIndex) override
        {
            if (auto instancePtr = getVoidService(typeIndex))
            {
                return instancePtr;
            }

            throw std::exception(); //
        }

        std::shared_ptr<void> getVoidService(const std::type_index &typeIndex) override
        {
            OptionalRef<IIocContainer> containerOpt(container_);

            while (containerOpt.hasValue())
            {
                IIocContainer &container = containerOpt.value();

                OptionalRef<IServiceDescriptor> descriptorOpt =
                    container.descriptors().getDescriptorOrNull(typeIndex);

                if (descriptorOpt.hasValue())
                {
                    return processDescriptor(typeIndex, descriptorOpt.value());
                }

                containerOpt = container.getParentContainerOrNull();
            }

            return nullptr;
        }

        std::vector<std::shared_ptr<void>>
        getVoidServices(const std::type_index &typeIndex) override
        {
            std::vector<std::shared_ptr<void>> voidServices;

            OptionalRef<IIocContainer> containerOpt(container_);

            while (containerOpt.hasValue())
            {
                IIocContainer &container = containerOpt.value();

                for (IServiceDescriptor &descriptor :
                     container.descriptors().getDescriptors(typeIndex))
                {
                    voidServices.push_back(processDescriptor(typeIndex, descriptor));
                }

                containerOpt = container.getParentContainerOrNull();
            }

            return voidServices;
        }

        std::shared_ptr<void> processDescriptor(const std::type_index &typeIndex,
                                                IServiceDescriptor &descriptor)
        {
            switch (descriptor.lifetime())
            {
            case ServiceLifetime::kSingleton:
                return getOrBuildService(typeIndex, descriptor.factory(), rootScope_);

            case ServiceLifetime::kScoped:
                return getOrBuildService(typeIndex, descriptor.factory(), localScope_);

            case ServiceLifetime::kTransient:
                return descriptor.factory().createServiceVoid(*this);
            }

            throw std::exception(); //
        }

        std::shared_ptr<void> getOrBuildService(const std::type_index &typeIndex,
                                                IVoidServiceFactory &serviceFactory,
                                                IServiceScope &scope)
        {
            std::shared_ptr<void> instancePtr = scope.getServiceOrNull(typeIndex);

            if (!instancePtr)
            {
                instancePtr = serviceFactory.createServiceVoid(*this);
                scope.addService(typeIndex, instancePtr);
            }

            return instancePtr;
        }
    };

    std::unique_ptr<IServiceProvider> makeServiceProvider(IIocContainer &container,
                                                          IServiceScope &rootScope,
                                                          IServiceScope &localScope)
    {
        return std::make_unique<ServiceProvider>(container, rootScope, localScope);
    }
} // namespace ioc::detail
