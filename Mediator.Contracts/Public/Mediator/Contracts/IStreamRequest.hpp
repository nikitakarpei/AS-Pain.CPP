#pragma once

namespace mediator
{
    class IBaseStreamRequest
    {
    public:
        virtual ~IBaseStreamRequest() = default;
    };

    template <class TResponse> class IStreamRequest : public IBaseStreamRequest
    {
    public:
        using ResponseType = TResponse;

        ~IStreamRequest() override = default;
    };
}
