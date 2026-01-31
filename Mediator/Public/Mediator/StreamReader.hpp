#pragma once

#include <Mediator/Contracts/IStreamResponse.hpp>
#include <Utility/OptionalRef.hpp>

#include <functional>
#include <optional>

namespace mediator
{
    class IBaseStreamReader
    {
    public:
        virtual ~IBaseStreamReader() = default;

        virtual OptionalRef<const IStreamResponse> getCurrentOrNull() = 0;

        virtual bool hasMore() = 0;

        virtual void getNext(std::function<void()> callback) = 0;
    };

    template <class TResponse> class StreamReader
    {
    private:
        std::unique_ptr<IBaseStreamReader> baseReaderPtr_;

    public:
        explicit StreamReader(std::unique_ptr<IBaseStreamReader> baseReaderPtr)
            : baseReaderPtr_(std::move(baseReaderPtr))
        {
        }

        OptionalRef<const TResponse> getCurrentOrNull()
        {
            if (auto currentOpt = baseReaderPtr_->getCurrentOrNull(); currentOpt.hasValue())
            {
                return dynamic_cast<const TResponse &>(currentOpt.value());
            }
            return std::nullopt;
        }

        bool hasMore() { return baseReaderPtr_->hasMore(); }

        void getNext(std::function<void()> callback)
        {
            baseReaderPtr_->getNext(std::move(callback));
        }
    };
}