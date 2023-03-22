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
// limitations under the License.

#include <cpp_utils/testing/gtest_aux.hpp>
#include <gtest/gtest.h>

#include <fastddsspy_participants/model/SpyModel.hpp>
#include <fastddsspy_participants/participant/SpyParticipant.hpp>
#include <fastddsspy_participants/participant/SpyDdsParticipant.hpp>

#include <ddspipe_core/core/DdsPipe.hpp>
#include <ddspipe_core/efficiency/payload/FastPayloadPool.hpp>
#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

using namespace eprosima;

namespace test {

const unsigned int DOMAIN = 222;

class SpyParticipantMock : public spy::participants::SpyParticipant
{
public:

    using SpyParticipant::SpyParticipant;
    using SpyParticipant::model_;

};

class SpyDdsParticipantMock : public spy::participants::SpyDdsParticipant
{
public:

    using SpyDdsParticipant::SpyDdsParticipant;

};

}

TEST(NetworkDatabaseTest, trivial)
{

    // // Create Configuration
    spy::yaml::Configuration configuration();

    // Create Participant configuration
    std::shared_ptr<ddspipe::participants::ParticipantConfiguration> participant_configuration =
            std::make_shared<ddspipe::participants::ParticipantConfiguration>();

    // Create Simple Participant configuration
    std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration> simple_participant_configuration =
            std::make_shared<ddspipe::participants::SimpleParticipantConfiguration>();

    // Create Payload Pool
    std::shared_ptr<ddspipe::core::PayloadPool> payload_pool =
            std::make_shared<ddspipe::core::FastPayloadPool>();

    // Create Discovery Database
    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database =
            std::make_shared<ddspipe::core::DiscoveryDatabase>();

    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    // Create participants
    std::shared_ptr<test::SpyParticipantMock> spy_participant =
            std::make_shared<test::SpyParticipantMock>(
                configuration.spy_configuration,
                payload_pool,
                discovery_database,
                model);

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(
                configuration.simple_configuration,
                payload_pool,
                discovery_database);

    // Create and populate Participant Database
    // std::shared_ptr<ddspipe::core::ParticipantsDatabase> participant_database =
    //         std::make_shared<ddspipe::core::ParticipantsDatabase>();

    // participant_database->add_participant(
    //     spy_participant->id(),
    //     spy_participant
    //     );

    // Init Dds Participant
    // dds_participant->init();

    // participant_database->add_participant(
    //     dds_participant->id(),
    //     dds_participant
    //     );


    // ddspipe::core::types::DomainId domainId;
    // domainId.domain_id = test::DOMAIN;
    // configuration.simple_configuration->domain = domainId;

    // // Create allowed topics list
    // std::shared_ptr<ddspipe::core::AllowedTopicList> allowed_topics =
    //         std::make_shared<ddspipe::core::AllowedTopicList>(
    //             configuration.allowlist,
    //             configuration.blocklist);

    // // Create Thread Pool
    // std::shared_ptr<utils::SlotThreadPool> thread_pool =
    //         std::make_shared<utils::SlotThreadPool>(configuration.n_threads);

    // // Create DDS Pipe
    // std::shared_ptr<ddspipe::core::DdsPipe> pipe =
    //         std::make_unique<ddspipe::core::DdsPipe>(
    //             allowed_topics,
    //             discovery_database,
    //             payload_pool,
    //             participant_database,
    //             thread_pool,
    //             configuration.builtin_topics,
    //             true
    //             );

    // pipe->enable();
    // // spy::participants::MockDdsParticipant dds_participant;
    // // spy::participants::NetworkDatabase network_database;
    // // network_database.participant_database_;
    // // network_database.endpoint_database_;
    // std::cout << "participant_database_!!!!!!!!!!1: " << spy_participant->model_->participant_database_.size() << std::endl;
    // std::cout << "endpoint_database_: " << spy_participant.model.network_database.endpoint_database_ << std::endl;
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
