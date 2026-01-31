#pragma once

#include <Ioc/IServiceScope.hpp>
#include <memory>

namespace ioc
{
    class IIocContainerBuilder;
    class IServiceScope;

    class IIocContainer : public IServiceScope
    {
    public:
        virtual std::unique_ptr<IIocContainerBuilder> createChildContainerBuilder() = 0;
    };
}
