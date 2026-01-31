#pragma once

#include <cstdlib>
#include <exception>

#include <Mediator/Contracts/IRequest.hpp>

namespace mediator
{
    class IBaseRequestExceptionHandler
    {
    public:
        virtual ~IBaseRequestExceptionHandler() = default;

        virtual bool baseTryHandle(const IBaseRequest &request,
                                   const std::exception_ptr &exceptionPtr) = 0;
    };

    template <class TRequest> class IRequestExceptionHandler : public IBaseRequestExceptionHandler
    {
    public:
        virtual bool tryHandle(const TRequest &request, const std::exception_ptr &exceptionPtr) = 0;

    private:
        bool baseTryHandle(const IBaseRequest &request,
                           const std::exception_ptr &exceptionPtr) final
        {
            return tryHandle(dynamic_cast<const TRequest &>(request), exceptionPtr);
        }
    };
}
