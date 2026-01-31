#include "ServiceScope.hpp"

#include <Ioc/Detail/IServiceProviderFactory.hpp>
#include <Ioc/Detail/IServiceScope.hpp>
#include <Ioc/Detail/ServiceScope.hpp>
#include <Ioc/IServiceProvider.hpp>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace ioc::detail
{
    class ServiceScope final : public IServiceScope
    {
    private:
        IServiceProviderFactory &providerFactory_;
        IServiceScope &rootScope_;

        std::mutex mutex_;
        std::unordered_map<std::type_index, std::shared_ptr<void>> sharedInstances_;

    public:
        explicit ServiceScope(IServiceProviderFactory &providerFactory)
            : providerFactory_(providerFactory), rootScope_(*this)
        {
        }

        explicit ServiceScope(IServiceProviderFactory &providerFactory, IServiceScope &rootScope)
            : providerFactory_(providerFactory), rootScope_(rootScope)
        {
        }

        void addService(const std::type_index &typeIndex,
                        std::shared_ptr<void> instancePtr) override
        {
            std::unique_lock lock(mutex_);

            auto [_, success] = sharedInstances_.try_emplace(typeIndex, std::move(instancePtr));
            if (!success)
            {
                throw std::exception(); //
            }
        }

        std::shared_ptr<void> getServiceOrNull(const std::type_index &typeIndex) override
        {
            std::unique_lock lock(mutex_);

            auto iterator = sharedInstances_.find(typeIndex);
            if (iterator != sharedInstances_.end())
            {
                return iterator->second;
            }

            return nullptr;
        }

        std::unique_ptr<ioc::IServiceScope> createChildScope() override
        {
            return makeServiceScope(providerFactory_, rootScope_);
        }

        std::unique_ptr<IServiceProvider> createServiceProvider() override
        {
            return providerFactory_.createProvider(rootScope_, *this);
        }
    };

    std::unique_ptr<IServiceScope> makeServiceScope(IServiceProviderFactory &providerFactory)
    {
        return std::make_unique<ServiceScope>(providerFactory);
    }

    std::unique_ptr<IServiceScope> makeServiceScope(IServiceProviderFactory &providerFactory,
                                                    IServiceScope &rootScope)
    {
        return std::make_unique<ServiceScope>(providerFactory, rootScope);
    }
}
