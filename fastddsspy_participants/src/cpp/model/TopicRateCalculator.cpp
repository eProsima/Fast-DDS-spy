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

#include <fastddsspy_participants/model/TopicRateCalculator.hpp>

namespace eprosima {
namespace spy {
namespace participants {

void TopicRateCalculator::add_data(
        const ddspipe::core::types::DdsTopic& topic,
        ddspipe::core::types::RtpsPayloadData& data)
{
    std::unique_lock<RateByTopicMapType> _(data_by_topic_);
    auto& rate_data = get_or_create_data_rate_from_topic_nts_(topic);

    // If is first data, set initial time
    if (rate_data.data_received == 0)
    {
        rate_data.first_data_time = data.source_timestamp;
    }

    // Set reception time
    rate_data.last_data_time = data.source_timestamp;

    // Increase in 1 the number of data received
    rate_data.data_received++;
}

TopicRateCalculator::RateType TopicRateCalculator::get_topic_rate(
        const ddspipe::core::types::DdsTopic& topic) const noexcept
{
    std::shared_lock<RateByTopicMapType> _(data_by_topic_);

    DataRateInfo data;
    bool exist = get_data_rate_from_topic_nts_(topic, data);
    if (!exist)
    {
        return 0;
    }

    // If there is only one data (or in a special case) first and last could be the same and produce a 0 division
    float seconds_elapsed = data.last_data_time.seconds() - data.first_data_time.seconds();
    if (seconds_elapsed == 0)
    {
        // TODO decide what to do in this case
        return data.data_received;
    }

    return static_cast<float>(data.data_received) / seconds_elapsed;
}

TopicRateCalculator::DataRateInfo& TopicRateCalculator::get_or_create_data_rate_from_topic_nts_(
        const ddspipe::core::types::DdsTopic& topic)
{
    // Create data or return the one already stored
    return data_by_topic_[topic];
}

bool TopicRateCalculator::get_data_rate_from_topic_nts_(
        const ddspipe::core::types::DdsTopic& topic,
        TopicRateCalculator::DataRateInfo& data) const noexcept
{
    auto it = data_by_topic_.find(topic);
    if (data_by_topic_.find(topic) == data_by_topic_.end())
    {
        return false;
    }
    data = it->second;
    return true;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
