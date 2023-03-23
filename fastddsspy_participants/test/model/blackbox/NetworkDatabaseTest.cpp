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

#include <fastddsspy_participants/participant/SpyParticipant.hpp>

#include <ddspipe_core/core/DdsPipe.hpp>
#include <ddspipe_core/efficiency/payload/FastPayloadPool.hpp>

#include <ddspipe_participants/writer/auxiliar/BlankWriter.hpp>
#include <ddspipe_participants/reader/auxiliar/BlankReader.hpp>
#include <ddspipe_participants/reader/auxiliar/InternalReader.hpp>
#include <ddspipe_participants/participant/auxiliar/BlankParticipant.hpp>
#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>

using namespace eprosima;

namespace test {

const unsigned int DOMAIN = 222;


class SpyDdsParticipantMock : public ddspipe::participants::BlankParticipant
{
public:

    //! Constructor with Id of this participant
    SpyDdsParticipantMock(
            const ddspipe::core::types::ParticipantId& id)
            : ddspipe::participants::BlankParticipant(id)
    {
    }

    //! Override create_writer() IParticipant method
    std::shared_ptr<ddspipe::core::IWriter> create_writer(
            const ddspipe::core::ITopic& topic)
    {
        return std::make_shared<ddspipe::participants::BlankWriter>();
    }

    //! Override create_reader() IParticipant method
    std::shared_ptr<ddspipe::core::IReader> create_reader(
            const ddspipe::core::ITopic& topic)
    {
        if (topic.internal_type_discriminator() != spy::participants::INTERNAL_TOPIC_TYPE_PARTICIPANTS_INFO)
        {
            return std::make_shared<ddspipe::participants::BlankReader>();
        }

        return participant_reader;
    }

    std::shared_ptr<ddspipe::participants::InternalReader> participant_reader =
        std::make_shared<ddspipe::participants::InternalReader>(id_);

};

}

TEST(NetworkDatabaseTest, trivial)
{
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
    std::shared_ptr<spy::participants::SpyParticipantConfiguration> spy_configuration =
        std::make_shared<spy::participants::SpyParticipantConfiguration>();
    std::shared_ptr<spy::participants::SpyParticipant> spy_participant =
            std::make_shared<spy::participants::SpyParticipant>(
                spy_configuration,
                payload_pool,
                discovery_database,
                model);

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create and populate Participant Database
    std::shared_ptr<ddspipe::core::ParticipantsDatabase> participant_database =
            std::make_shared<ddspipe::core::ParticipantsDatabase>();

    participant_database->add_participant(
        spy_participant->id(),
        spy_participant
        );

    participant_database->add_participant(
        dds_participant->id(),
        dds_participant
        );

    // Create allowed topics list
    std::set<utils::Heritable<ddspipe::core::types::IFilterTopic>> lists = {};
    std::shared_ptr<ddspipe::core::AllowedTopicList> allowed_topics =
            std::make_shared<ddspipe::core::AllowedTopicList>(
                lists,
                lists);

    // Create Thread Pool
    unsigned int n_threads = 12;
    std::shared_ptr<utils::SlotThreadPool> thread_pool =
            std::make_shared<utils::SlotThreadPool>(n_threads);

    // Create DDS Pipe
    std::set<utils::Heritable<ddspipe::core::types::DistributedTopic>> builtin_topics = {};
    std::shared_ptr<ddspipe::core::DdsPipe> pipe =
            std::make_unique<ddspipe::core::DdsPipe>(
                allowed_topics,
                discovery_database,
                payload_pool,
                participant_database,
                thread_pool,
                builtin_topics,
                true
                );

    pipe->enable();


    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send N messages
    for (unsigned int i = 0; i < 10; i++)
    {
        auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
        new_data->info.name = "reader::" + std::to_string(i);
        new_data->info.active = true;
        ddspipe::core::types::Guid guid_data;
        new_data->info.guid = guid_data;
        dds_reader->simulate_data_reception(std::move(new_data));
    }
    sleep(1);

    std::cout << "Information database !!!" << std::endl;
    for (const auto& participant : model->participant_database_)
    {
        std::cout << "participant_database_!!!" << std::endl;
        std::cout << "guid: " << participant.first << std::endl;
        std::cout << "name: " << participant.second.name << std::endl;
    }

    for (const auto& endpoint : model->endpoint_database_)
    {
        std::cout << "endpoint_database_!!!" << std::endl;
        std::cout << "guid: " << endpoint.first << std::endl;
        std::cout << "guid: " << endpoint.second.guid << std::endl;
        std::cout << "topic name: " << endpoint.second.topic.m_topic_name << std::endl;
        std::cout << "type name: " << endpoint.second.topic.type_name << std::endl;
    }
    std::cout << "END !!!" << std::endl;

}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
