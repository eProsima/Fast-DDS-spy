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

#pragma once

#include <tuple>

#include <cpp_utils/types/Atomicable.hpp>

#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/dds/Payload.hpp>

#include <ddspipe_participants/participant/dynamic_types/ISchemaHandler.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
class TopicRateCalculator : public ddspipe::participants::ISchemaHandler
{
public:

    void add_data(
            const ddspipe::core::types::DdsTopic& topic,
            ddspipe::core::types::RtpsPayloadData& data) override;

    using RateType = float;

    RateType get_topic_rate(
            const ddspipe::core::types::DdsTopic& topic) const noexcept;

protected:

    struct DataRateInfo
    {
        ddspipe::core::types::DataTime first_data_time;
        unsigned int data_received;
        ddspipe::core::types::DataTime last_data_time;
    };

    bool get_data_rate_from_topic_nts_(
            const ddspipe::core::types::DdsTopic& topic,
            DataRateInfo& data) const noexcept;

    DataRateInfo& get_or_create_data_rate_from_topic_nts_(
            const ddspipe::core::types::DdsTopic& topic);

    using RateByTopicMapType = utils::SharedAtomicable<std::map<ddspipe::core::types::DdsTopic, DataRateInfo>>;

    mutable RateByTopicMapType data_by_topic_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
