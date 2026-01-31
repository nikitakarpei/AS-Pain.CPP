#pragma once

#include <memory>

namespace mediator
{
    class IResponse;

    class IBaseResponseHandler
    {
    public:
        virtual ~IBaseResponseHandler() = default;

        virtual void baseHandle(std::unique_ptr<IResponse> responsePtr) = 0;
    };

    template <class TResponse> class IResponseHandler : public IBaseResponseHandler
    {
    public:
        virtual void handle(std::unique_ptr<TResponse> responsePtr) = 0;

    private:
        void baseHandle(std::unique_ptr<IResponse> responsePtr) final
        {
            if (auto *p = dynamic_cast<TResponse *>(responsePtr.get()))
            {
                responsePtr.release();
                return handle(std::unique_ptr<TResponse>(p));
            }
            return handle(nullptr);
        }
    };
}
