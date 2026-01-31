#include <Utility/OptionalRef.hpp>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>


#include "IServiceDescriptor.hpp"
#include "IServiceDescriptors.hpp"

namespace ioc::detail
{
    class ServiceDescriptors : public IServiceDescriptors
    {
    private:
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<IServiceDescriptor>>>
            descriptors_;

    public:
        void addDescriptor(const std::type_index &typeIndex,
                           std::unique_ptr<IServiceDescriptor> descriptorPtr) override
        {
            descriptors_[typeIndex].push_back(std::move(descriptorPtr));
        }

        OptionalRef<IServiceDescriptor>
        getDescriptorOrNull(const std::type_index &typeIndex) override
        {
            auto iterator = descriptors_.find(typeIndex);
            if (iterator != descriptors_.end())
            {
                return *iterator->second.rbegin()->get();
            }

            return std::nullopt;
        }

        std::vector<std::reference_wrapper<IServiceDescriptor>>
        getDescriptors(const std::type_index &typeIndex) override
        {
            auto iterator = descriptors_.find(typeIndex);
            if (iterator != descriptors_.end())
            {
                std::vector<std::reference_wrapper<IServiceDescriptor>> descriptors;
                descriptors.reserve(iterator->second.size());
                std::ranges::transform(iterator->second, std::back_inserter(descriptors),
                                       [](const std::unique_ptr<IServiceDescriptor> &descriptorPtr)
                                       { return std::ref(*descriptorPtr); });

                return descriptors;
            }

            return {};
        }
    };

    std::unique_ptr<IServiceDescriptors> makeServiceDescriptors()
    {
        return std::make_unique<ServiceDescriptors>();
    }
}
