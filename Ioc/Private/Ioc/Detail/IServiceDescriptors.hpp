#pragma once

#include <Utility/Fwd/OptionalRef.hpp>
#include <functional>
#include <memory>
#include <typeindex>
#include <vector>

namespace ioc::detail
{
    class IServiceDescriptor;

    class IServiceDescriptors
    {
    public:
        virtual ~IServiceDescriptors() = default;

        virtual void addDescriptor(const std::type_index &typeIndex,
                                   std::unique_ptr<IServiceDescriptor> descriptorPtr) = 0;

        virtual OptionalRef<IServiceDescriptor>
        getDescriptorOrNull(const std::type_index &typeIndex) = 0;

        virtual std::vector<std::reference_wrapper<IServiceDescriptor>>
        getDescriptors(const std::type_index &typeIndex) = 0;
    };
}
