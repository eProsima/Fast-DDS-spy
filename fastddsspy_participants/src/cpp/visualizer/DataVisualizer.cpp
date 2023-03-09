// Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License\.

#include <mutex>

#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicPubSubType.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicDataHelper.hpp>

#include <fastddsspy_participants/visualizer/DataVisualizer.hpp>

namespace eprosima {
namespace spy {
namespace participants {

using eprosima::ddspipe::core::types::operator<<;

bool DataVisualizer::activate(
        const ddspipe::core::types::DdsTopic& topic_to_activate,
        std::ostream* target /* = &std::cout */)
{
    if (!is_topic_type_discovered(topic_to_activate))
    {
        logWarning(FASTDDSSPY_DATAVISUALIZER,
            "Type <" << topic_to_activate.type_name <<
            "> for topic <" << topic_to_activate.topic_name() << "> is not discovered.");
        return false;
    }

    std::unique_lock<std::shared_mutex> _(mutex_);

    // If type exist, this is the new topic to activate
    activated_ = true;
    activated_topic_ = topic_to_activate;
    target_ = target;

    return true;
}

void DataVisualizer::deactivate()
{
    std::unique_lock<std::shared_mutex> _(mutex_);
    activated_ = false;
}

void DataVisualizer::add_schema(
        const fastrtps::types::DynamicType_ptr& dynamic_type)
{
    std::unique_lock<std::shared_mutex> _(mutex_);

    // Add type to map if not yet
    // NOTE: it does not matter if it is already in set
    types_discovered_[dynamic_type->get_name()] = dynamic_type;
}

void DataVisualizer::add_data(
        const ddspipe::core::types::DdsTopic& topic,
        ddspipe::core::types::RtpsPayloadData& data)
{
    std::shared_lock<std::shared_mutex> _(mutex_);

    if (activated_ && topic == activated_topic_)
    {
        // TODO: make map search more safe
        print_data_nts_(types_discovered_[topic.type_name], data);
    }
}

bool DataVisualizer::is_topic_type_discovered(const ddspipe::core::types::DdsTopic& topic) const noexcept
{
    std::shared_lock<std::shared_mutex> _(mutex_);
    return is_topic_type_discovered_nts_(topic);
}

bool DataVisualizer::is_topic_type_discovered_nts_(const ddspipe::core::types::DdsTopic& topic) const noexcept
{
    return types_discovered_.find(topic.type_name) != types_discovered_.end();
}

void DataVisualizer::print_data_nts_(
        fastrtps::types::DynamicType_ptr& type,
        ddspipe::core::types::RtpsPayloadData& data) const noexcept
{
    // Create PubSub Type
    fastrtps::types::DynamicPubSubType pubsub_type(type);
    fastrtps::types::DynamicData* dyn_data = fastrtps::types::DynamicDataFactory::get_instance()->create_data(type);

    pubsub_type.deserialize(&data.payload, dyn_data);

    // TODO this does not make much sense as print does not allow to choose target
    // This must change in dynamic types
    *target_ << "\n";
    fastrtps::types::DynamicDataHelper::print(dyn_data);
    *target_ << std::endl;

    fastrtps::types::DynamicDataFactory::get_instance()->delete_data(dyn_data);
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
