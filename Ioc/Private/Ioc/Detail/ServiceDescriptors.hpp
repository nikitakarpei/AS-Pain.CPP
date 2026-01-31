#pragma once

#include <memory>

namespace ioc::detail
{
    class IServiceDescriptors;

    std::unique_ptr<IServiceDescriptors> makeServiceDescriptors();
}