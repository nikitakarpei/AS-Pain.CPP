#pragma once

#include <Ioc/IServiceScope.hpp>
#include <memory>
#include <typeindex>

namespace ioc::detail
{
    class IServiceScope : public ioc::IServiceScope
    {
    public:
        virtual void addService(const std::type_index &typeIndex,
                                std::shared_ptr<void> instancePtr) = 0;

        virtual std::shared_ptr<void> getServiceOrNull(const std::type_index &typeIndex) = 0;
    };
}
