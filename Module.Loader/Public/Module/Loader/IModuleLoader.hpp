#pragma once

#include <filesystem>
#include <memory>

namespace module
{
    class IModule;
}

namespace module::loader
{
    class IModuleLoader
    {
    public:
        virtual ~IModuleLoader() = default;

        virtual std::unique_ptr<IModule> load(const std::filesystem::path &path) = 0;
    };
}