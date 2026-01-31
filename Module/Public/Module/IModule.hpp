#pragma once

namespace mediator
{
    class IRequestSender;
}

namespace moduleloader
{
    class IModule
    {
    public:
        virtual ~IModule() = default;

        virtual mediator::IRequestSender &requestSender() = 0;
    };
}