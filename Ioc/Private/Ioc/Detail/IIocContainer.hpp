#pragma once

#include <Ioc/IIocContainer.hpp>
#include <Utility/Fwd/OptionalRef.hpp>

namespace ioc::detail
{
    class IServiceDescriptors;

    class IIocContainer : public ioc::IIocContainer
    {
    public:
        virtual IServiceDescriptors &descriptors() = 0;

        virtual OptionalRef<IIocContainer> getParentContainerOrNull() = 0;
    };
}
