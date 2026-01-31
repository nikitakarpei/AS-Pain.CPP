#pragma once

namespace mediator
{
    class IBaseRequest
    {
    public:
        virtual ~IBaseRequest() = default;
    };

    template <class TResponse> class IRequest : public IBaseRequest
    {
    public:
        using ResponseType = TResponse;

        ~IRequest() override = default;
    };
}
