#pragma once

#include <memory>

namespace ioc
{
    class IIocContainerBuilder;

    std::unique_ptr<IIocContainerBuilder> makeIocContainerBuilder();
}
