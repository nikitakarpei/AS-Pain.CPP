#pragma once

#include <agrpc/server_rpc.hpp>
#include <concepts>

namespace thirdparty::asio::grpc
{
    template <class T>
    concept ServerStreamingServerRpcConcept = requires {
        { T::TYPE } -> std::convertible_to<agrpc::ServerRPCType>;
    } && (T::TYPE == agrpc::ServerRPCType::SERVER_STREAMING);
}