#include <Ioc/Detail/IIocContainer.hpp>
#include <Ioc/Detail/IServiceDescriptor.hpp>
#include <Ioc/Detail/IServiceDescriptors.hpp>
#include <Ioc/Detail/IocContainer.hpp>
#include <Ioc/Detail/ServiceDescriptor.hpp>
#include <Ioc/Detail/ServiceDescriptors.hpp>
#include <Ioc/Detail/ServiceLifetime.hpp>
#include <Ioc/IIocContainer.hpp>
#include <Ioc/IIocContainerBuilder.hpp>
#include <Utility/OptionalRef.hpp>
#include <memory>
#include <typeindex>

namespace ioc::detail
{
    class IocContainerBuilder final : public IIocContainerBuilder
    {
    private:
        std::unique_ptr<IServiceDescriptors> serviceDescriptorsPtr_ = makeServiceDescriptors();
        OptionalRef<detail::IIocContainer> parentContainerOpt_;

    public:
        explicit IocContainerBuilder() = default;

        explicit IocContainerBuilder(detail::IIocContainer &parentContainer)
            : parentContainerOpt_(parentContainer)
        {
        }

        void addScoped(const std::type_index &typeIndex,
                       std::unique_ptr<IVoidServiceFactory> factoryPtr) override
        {
            serviceDescriptorsPtr_->addDescriptor(
                typeIndex, makeServiceDescriptor(std::move(factoryPtr), ServiceLifetime::kScoped));
        }

        void addSingleton(const std::type_index &typeIndex,
                          std::unique_ptr<IVoidServiceFactory> factoryPtr) override
        {
            serviceDescriptorsPtr_->addDescriptor(
                typeIndex,
                makeServiceDescriptor(std::move(factoryPtr), ServiceLifetime::kSingleton));
        }

        void addTransient(const std::type_index &typeIndex,
                          std::unique_ptr<IVoidServiceFactory> factoryPtr) override
        {
            serviceDescriptorsPtr_->addDescriptor(
                typeIndex,
                makeServiceDescriptor(std::move(factoryPtr), ServiceLifetime::kTransient));
        }

        std::unique_ptr<ioc::IIocContainer> createContainer() override
        {
            return parentContainerOpt_.hasValue()
                       ? makeIocContainer(std::move(serviceDescriptorsPtr_),
                                          parentContainerOpt_.value())
                       : makeIocContainer(std::move(serviceDescriptorsPtr_));
        }
    };

    std::unique_ptr<IIocContainerBuilder>
    makeIocContainerBuilder(detail::IIocContainer &parentContainer)
    {
        return std::make_unique<IocContainerBuilder>(parentContainer);
    }
}

namespace ioc
{
    std::unique_ptr<IIocContainerBuilder> makeIocContainerBuilder()
    {
        return std::make_unique<detail::IocContainerBuilder>();
    }
}
