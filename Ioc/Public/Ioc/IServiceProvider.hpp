#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <typeindex>
#include <vector>

namespace ioc
{
    class IServiceProvider
    {
    public:
        virtual ~IServiceProvider() = default;

        template <class T> std::shared_ptr<T> getRequiredService()
        {
            return getRequiredService<T>(typeid(T));
        }

        template <class T> std::shared_ptr<T> getRequiredService(const std::type_index &typeIndex)
        {
            return std::reinterpret_pointer_cast<T>(getRequiredVoidService(typeIndex));
        }

        template <class T> std::shared_ptr<T> getService() { return getService<T>(typeid(T)); }

        template <class T> std::shared_ptr<T> getService(const std::type_index &typeIndex)
        {
            return std::reinterpret_pointer_cast<T>(getVoidService(typeIndex));
        }

        template <class T> std::vector<std::shared_ptr<T>> getServices()
        {
            return getServices<T>(typeid(T));
        }

        template <class T>
        std::vector<std::shared_ptr<T>> getServices(const std::type_index &typeIndex)
        {
            std::vector<std::shared_ptr<void>> voidServices = getVoidServices(typeIndex);
            std::vector<std::shared_ptr<T>> services;
            services.reserve(voidServices.size());
            std::ranges::transform(voidServices, std::back_inserter(services),
                                   [](const std::shared_ptr<void> &voidServicePtr)
                                   { return std::reinterpret_pointer_cast<T>(voidServicePtr); });
            return services;
        }

    protected:
        virtual std::shared_ptr<void> getRequiredVoidService(const std::type_index &typeIndex) = 0;

        virtual std::shared_ptr<void> getVoidService(const std::type_index &typeIndex) = 0;

        virtual std::vector<std::shared_ptr<void>>
        getVoidServices(const std::type_index &typeIndex) = 0;
    };
}
