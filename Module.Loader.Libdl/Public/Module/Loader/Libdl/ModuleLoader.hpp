#pragma once

#include <memory>

namespace module::loader
{
    class IModuleLoader;
}

namespace module::loader::libdl
{
    std::unique_ptr<IModuleLoader> makeModuleLoader();
}