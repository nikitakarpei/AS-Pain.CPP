#include "IocContainer.hpp"

#include <Ioc/Detail/IIocContainer.hpp>
#include <Ioc/Detail/IServiceDescriptors.hpp>
#include <Ioc/Detail/IServiceProviderFactory.hpp>
#include <Ioc/Detail/IServiceScope.hpp>
#include <Ioc/Detail/IocContainerBuilder.hpp>
#include <Ioc/Detail/ServiceProviderFactory.hpp>
#include <Ioc/Detail/ServiceScope.hpp>
#include <Ioc/IIocContainer.hpp>
#include <Ioc/IIocContainerBuilder.hpp>
#include <Ioc/IServiceProvider.hpp>
#include <Utility/OptionalRef.hpp>
#include <memory>
#include <utility>

namespace ioc::detail
{
    class IocContainer final : public IIocContainer
    {
    private:
        std::unique_ptr<IServiceDescriptors> serviceDescriptorsPtr_;
        OptionalRef<IIocContainer> parentContainerOpt_;

        std::unique_ptr<IServiceProviderFactory> serviceProviderFactoryPtr_ =
            makeServiceProviderFactory(*this);
        std::unique_ptr<IServiceScope> rootServiceScopePtr_ =
            makeServiceScope(*serviceProviderFactoryPtr_);

    public:
        explicit IocContainer(std::unique_ptr<IServiceDescriptors> serviceDescriptorsPtr)
            : serviceDescriptorsPtr_(std::move(serviceDescriptorsPtr))
        {
        }

        explicit IocContainer(std::unique_ptr<IServiceDescriptors> serviceDescriptorsPtr,
                              IIocContainer &parentContainer)
            : serviceDescriptorsPtr_(std::move(serviceDescriptorsPtr)),
              parentContainerOpt_(parentContainer)
        {
        }

        IServiceDescriptors &descriptors() override { return *serviceDescriptorsPtr_; }

        OptionalRef<IIocContainer> getParentContainerOrNull() override
        {
            return parentContainerOpt_;
        }

        std::unique_ptr<IIocContainerBuilder> createChildContainerBuilder() override
        {
            return makeIocContainerBuilder(*this);
        }

        std::unique_ptr<IServiceScope> createChildScope() override
        {
            return rootServiceScopePtr_->createChildScope();
        }

        std::unique_ptr<IServiceProvider> createServiceProvider() override
        {
            return rootServiceScopePtr_->createServiceProvider();
        }
    };

    std::unique_ptr<IIocContainer>
    makeIocContainer(std::unique_ptr<IServiceDescriptors> descriptorsPtr)
    {
        return std::make_unique<IocContainer>(std::move(descriptorsPtr));
    }

    std::unique_ptr<IIocContainer>
    makeIocContainer(std::unique_ptr<IServiceDescriptors> descriptorsPtr,
                     IIocContainer &parentContainer)
    {
        return std::make_unique<IocContainer>(std::move(descriptorsPtr), parentContainer);
    }
}
