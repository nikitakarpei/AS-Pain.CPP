#pragma once

#include <Mediator/Contracts/IRequest.hpp>
#include <Mediator/IResponseHandler.hpp>
#include <memory>
#include <typeindex>

namespace mediator
{
    template <class> class IRequestHandler;

    template <class> class IRequestExceptionHandler;

    class IRequestSender
    {
    public:
        virtual ~IRequestSender() = default;

        template <class TRequest>
        void
        send(std::unique_ptr<TRequest> requestPtr,
             std::unique_ptr<IResponseHandler<typename TRequest::ResponseType>> responseHandlerPtr)
        {
            return send(typeid(IRequestHandler<TRequest>),
                        typeid(IRequestExceptionHandler<TRequest>), std::move(requestPtr),
                        std::move(responseHandlerPtr));
        }

    protected:
        virtual void send(const std::type_index &requestHandlerTypeIndex,
                          const std::type_index &requestExceptionHandlerTypeIndex,
                          std::unique_ptr<IBaseRequest> requestPtr,
                          std::unique_ptr<IBaseResponseHandler> responseHandlerPtr) = 0;
    };
}
