#include <Ioc/Detail/IServiceProviderFactory.hpp>
#include <Ioc/Detail/IocContainer.hpp>
#include <Ioc/Detail/ServiceProvider.hpp>
#include <Ioc/IServiceProvider.hpp>
#include <memory>


namespace ioc::detail
{
    class ServiceProviderFactory final : public IServiceProviderFactory
    {
    private:
        IIocContainer &container_;

    public:
        explicit ServiceProviderFactory(IIocContainer &container) : container_(container) {}

        std::unique_ptr<IServiceProvider> createProvider(IServiceScope &rootScope,
                                                         IServiceScope &localScope) override
        {
            return makeServiceProvider(container_, rootScope, localScope);
        }
    };

    std::unique_ptr<IServiceProviderFactory> makeServiceProviderFactory(IIocContainer &container)
    {
        return std::make_unique<ServiceProviderFactory>(container);
    }
}
