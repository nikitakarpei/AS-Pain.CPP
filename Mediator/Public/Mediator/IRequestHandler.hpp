#pragma once

#include <memory>

namespace mediator
{
    class IBaseRequest;
    class IResponse;

    class IBaseRequestHandler
    {
    public:
        virtual ~IBaseRequestHandler() = default;

        virtual std::unique_ptr<IResponse> baseHandle(const IBaseRequest &request) = 0;
    };

    template <class TRequest> class IRequestHandler : public IBaseRequestHandler
    {
    public:
        virtual std::unique_ptr<typename TRequest::ResponseType>
        handle(const TRequest &request) = 0;

    private:
        std::unique_ptr<IResponse> baseHandle(const IBaseRequest &request) final
        {
            return handle(dynamic_cast<const TRequest &>(request));
        }
    };
}
