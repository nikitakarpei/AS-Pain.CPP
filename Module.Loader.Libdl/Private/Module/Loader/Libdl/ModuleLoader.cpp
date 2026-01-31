#include <Module/Loader/IModuleLoader.hpp>
#include <memory>

namespace module::loader::libdl::detail
{
    class ModuleLoader final : public IModuleLoader
    {
    public:
        std::unique_ptr<IModule> load(const std::filesystem::path &path) override {}
    };
}