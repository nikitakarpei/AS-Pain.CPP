#pragma once

#include <functional>

namespace coro
{
    class IAwaiterBase
    {
    public:
        virtual ~IAwaiterBase() = default;

        virtual void onCompleted(std::function<void()> continuation) = 0;

        virtual bool isCompleted() const = 0;
    };

    template <class TResult> class IAwaiter : public IAwaiterBase
    {
    public:
        using ResultType = TResult;

        virtual ~IAwaiter() override = default;

        virtual TResult getResult() = 0;
    };

    template <> class IAwaiter<void> : public IAwaiterBase
    {
    public:
        using ResultType = void;
    };
}