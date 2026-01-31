#pragma once

namespace ioc::detail
{
    enum class ServiceLifetime
    {
        kSingleton = 1,
        kScoped = 2,
        kTransient = 3
    };
}
