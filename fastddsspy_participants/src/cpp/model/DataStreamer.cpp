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

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>

#include <cpp_utils/utils.hpp>

#include <fastddsspy_participants/model/DataStreamer.hpp>

namespace eprosima {
namespace spy {
namespace participants {


bool DataStreamer::activate_all(
        const std::shared_ptr<CallbackType>& callback)
{
    std::unique_lock<std::shared_timed_mutex> _(mutex_);

    // If type exist, this is the new topic to activate
    activated_ = true;
    activated_all_ = true;
    callback_ = callback;

    return true;
}

bool DataStreamer::activate(
        const ddspipe::core::types::DdsTopic& topic_to_activate,
        const std::shared_ptr<CallbackType>& callback)
{
    if (!is_topic_type_discovered(topic_to_activate))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_DATASTREAMER,
                "Type <" << topic_to_activate.type_name <<
                "> for topic <" << topic_to_activate.topic_name() << "> is not discovered.");
        return false;
    }

    std::unique_lock<std::shared_timed_mutex> _(mutex_);

    // If type exist, this is the new topic to activate
    activated_ = true;
    activated_topic_ = topic_to_activate;
    activated_all_ = false;
    callback_ = callback;

    return true;
}

void DataStreamer::deactivate()
{
    std::unique_lock<std::shared_timed_mutex> _(mutex_);
    activated_ = false;
    callback_.reset();
}

void DataStreamer::add_schema(
        const fastdds::dds::DynamicType::_ref_type& dynamic_type,
        const fastdds::dds::xtypes::TypeIdentifier& type_identifier)
{
    static_cast<void>(type_identifier);

    std::unique_lock<std::shared_timed_mutex> _(mutex_);

    // Add type to map if not yet
    // NOTE: it does not matter if it is already in set
    auto const type_name = dynamic_type->get_name().to_string();
    types_discovered_[type_name] = dynamic_type;

    EPROSIMA_LOG_INFO(FASTDDSSPY_DATASTREAMER, "\nAdding schema with name " << type_name << ".");
}

void DataStreamer::add_data(
        const ddspipe::core::types::DdsTopic& topic,
        ddspipe::core::types::RtpsPayloadData& data)
{
    TopicRateCalculator::add_data(topic, data);

    fastdds::dds::DynamicType::_ref_type dyn_type;

    {
        std::shared_lock<std::shared_timed_mutex> _(mutex_);

        if (!activated_)
        {
            // If not activated, do nothing.
            return;
        }

        if (activated_all_)
        {
            if (!is_topic_type_discovered_nts_(topic))
            {
                // If all activated, add it only if schema is available, otherwise skip
                EPROSIMA_LOG_WARNING(
                    FASTDDSSPY_DATASTREAMER,
                    "All activated but schema not is available.");
                return;
            }
        }
        else
        {
            if (!(activated_topic_ == topic))
            {
                // If not all activated, and this is not the activated topic skip
                EPROSIMA_LOG_WARNING(
                    FASTDDSSPY_DATASTREAMER,
                    "Not all activated, and this is not the activated topic.");
                return;
            }
        }

        EPROSIMA_LOG_INFO(
            FASTDDSSPY_DATASTREAMER,
            "Adding data in topic " << topic);

        auto it = types_discovered_.find(topic.type_name);
        if (it == types_discovered_.end())
        {
            // The topic that is supposed to be activated has no associated type. This should not happen
            utils::tsnh(STR_ENTRY
                    << "Topic <" << topic << "> must not be activated if its type is not registered.");
        }
        else
        {
            dyn_type = it->second;
        }
    }

    // This should be called without mutex taken
    // Here should only arrive if it must call callback. Otherwise must return somewhere before
    (*callback_)(topic, dyn_type, data);
}

bool DataStreamer::is_topic_type_discovered(
        const ddspipe::core::types::DdsTopic& topic) const noexcept
{
    std::shared_lock<std::shared_timed_mutex> _(mutex_);
    return is_topic_type_discovered_nts_(topic);
}

bool DataStreamer::is_topic_type_discovered_nts_(
        const ddspipe::core::types::DdsTopic& topic) const noexcept
{
    return types_discovered_.find(topic.type_name) != types_discovered_.end();
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
