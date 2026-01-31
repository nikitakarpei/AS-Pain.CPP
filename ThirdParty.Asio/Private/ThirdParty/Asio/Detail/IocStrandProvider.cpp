#include <ThirdParty/Asio/IIocStrandProvider.hpp>
#include <memory>

namespace thirdparty::asio::detail
{
    class IocStrandProvider final : public IIoContextStrandProvider
    {
    private:
        boost::asio::io_context::strand strand_;

    public:
        explicit IocStrandProvider(boost::asio::io_context &ioContext) : strand_(ioContext) {}

        boost::asio::io_context::strand &getStrand() override { return strand_; }
    };
}

namespace thirdparty::asio
{
    std::unique_ptr<IIoContextStrandProvider>
    makeIoContextStrandProvider(boost::asio::io_context &ioContext)
    {
        return std::make_unique<detail::IocStrandProvider>(ioContext);
    }
}
