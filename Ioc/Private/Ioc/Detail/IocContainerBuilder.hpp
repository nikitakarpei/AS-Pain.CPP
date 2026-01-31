#pragma once

#include <memory>

namespace ioc
{
    class IIocContainerBuilder;
}

namespace ioc::detail
{
    class IIocContainer;

    std::unique_ptr<IIocContainerBuilder> makeIocContainerBuilder(IIocContainer &parentContainer);
}
