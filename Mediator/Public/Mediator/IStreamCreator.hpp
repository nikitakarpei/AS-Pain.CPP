#pragma once

#include <Mediator/Contracts/IStreamRequest.hpp>
#include <Mediator/StreamReader.hpp>
#include <memory>
#include <typeindex>

namespace mediator
{
    template <class> class IStreamRequestHandler;

    class IStreamCreator
    {
    public:
        virtual ~IStreamCreator() = default;

        template <class TRequest>
        StreamReader<typename TRequest::ResponseType>
        createStream(std::unique_ptr<TRequest> requestPtr)
        {
            return StreamReader<typename TRequest::ResponseType>(
                createBaseStream(typeid(IStreamRequestHandler<TRequest>), std::move(requestPtr)));
        }

    protected:
        virtual std::unique_ptr<IBaseStreamReader>
        createBaseStream(const std::type_index &requestHandlerTypeIndex,
                         std::unique_ptr<IBaseStreamRequest> requestPtr) = 0;
    };
}