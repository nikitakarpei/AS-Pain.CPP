#pragma once

#include <optional>

template <class T> class OptionalRef
{
private:
    T *ptr_{nullptr};

    void throwIfNull()
    {
        if (!ptr_)
        {
            throw std::bad_optional_access();
        }
    }

public:
    OptionalRef() = default;

    OptionalRef(const std::nullopt_t &) {}

    OptionalRef(T &ref) : ptr_(std::addressof(ref)) {}

    bool hasValue() const { return ptr_; }

    const T &value() const
    {
        throwIfNull();
        return *ptr_;
    }

    T &value()
    {
        throwIfNull();
        return *ptr_;
    }

    const T *operator->() const
    {
        throwIfNull();
        return ptr_;
    }

    T *operator->()
    {
        throwIfNull();
        return ptr_;
    }
};
