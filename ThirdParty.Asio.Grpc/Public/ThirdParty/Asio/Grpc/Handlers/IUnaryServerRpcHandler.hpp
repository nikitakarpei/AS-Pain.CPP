#pragma once

#include <ThirdParty/Asio/Grpc/Concepts/UnaryServerRpc.hpp>
#include <boost/asio/awaitable.hpp>

namespace thirdparty::asio::grpc
{
    template <UnaryServerRpcConcept TRpc> class IUnaryServerRpcHandler
    {
    public:
        virtual ~IUnaryServerRpcHandler() = default;

        virtual boost::asio::awaitable<void> handle(TRpc &rpc, typename TRpc::Request &request) = 0;
    };
}