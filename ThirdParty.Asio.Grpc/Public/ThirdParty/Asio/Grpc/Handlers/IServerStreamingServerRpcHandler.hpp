#pragma once

#include <ThirdParty/Asio/Grpc/Concepts/ServerStreamingServerRpc.hpp>
#include <boost/asio/awaitable.hpp>

namespace thirdparty::asio::grpc
{
    template <ServerStreamingServerRpcConcept TRpc> class IServerStreamingServerRpcHandler
    {
    public:
        virtual ~IServerStreamingServerRpcHandler() = default;

        virtual boost::asio::awaitable<void> handle(TRpc &rpc, typename TRpc::Request &request) = 0;
    };
}