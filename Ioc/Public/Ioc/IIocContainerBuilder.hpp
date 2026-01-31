#pragma once

#include <Ioc/IServiceFactory.hpp>
#include <memory>
#include <typeindex>

namespace ioc
{
    class IIocContainer;

    class IIocContainerBuilder
    {
    public:
        virtual ~IIocContainerBuilder() = default;

        template <class TService>
        void addScoped(std::unique_ptr<IServiceFactory<TService>> factoryPtr)
        {
            return addScoped(typeid(TService), std::move(factoryPtr));
        }

        template <class TService>
        void addSingleton(std::unique_ptr<IServiceFactory<TService>> factoryPtr)
        {
            return addSingleton(typeid(TService), std::move(factoryPtr));
        }

        template <class TService>
        void addTransient(std::unique_ptr<IServiceFactory<TService>> factoryPtr)
        {
            return addTransient(typeid(TService), std::move(factoryPtr));
        }

        virtual std::unique_ptr<IIocContainer> createContainer() = 0;

    protected:
        virtual void addScoped(const std::type_index &typeIndex,
                               std::unique_ptr<IVoidServiceFactory> factoryPtr) = 0;

        virtual void addSingleton(const std::type_index &typeIndex,
                                  std::unique_ptr<IVoidServiceFactory> factoryPtr) = 0;

        virtual void addTransient(const std::type_index &typeIndex,
                                  std::unique_ptr<IVoidServiceFactory> factoryPtr) = 0;
    };
}
