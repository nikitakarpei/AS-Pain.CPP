#pragma once

#include <memory>

namespace ioc::detail
{
    class IIocContainer;
    class IServiceDescriptors;

    std::unique_ptr<IIocContainer>
    makeIocContainer(std::unique_ptr<IServiceDescriptors> descriptorsPtr);

    std::unique_ptr<IIocContainer>
    makeIocContainer(std::unique_ptr<IServiceDescriptors> descriptorsPtr,
                     IIocContainer &parentContainer);
}
