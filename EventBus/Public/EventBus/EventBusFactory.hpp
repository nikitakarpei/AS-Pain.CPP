#pragma once

#include <memory>

namespace eventbus
{
    class IEventBusFactory;

    std::unique_ptr<IEventBusFactory> makeEventBusFactory();
}