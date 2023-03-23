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
#include <ddspipe_core/testing/random_values.hpp>

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

std::shared_ptr<ddspipe::core::DdsPipe> create_pipe(
    std::shared_ptr<spy::participants::SpyModel>& model,
    std::shared_ptr<test::SpyDdsParticipantMock>& dds_participant)
{
    // Create Payload Pool
    std::shared_ptr<ddspipe::core::PayloadPool> payload_pool =
            std::make_shared<ddspipe::core::FastPayloadPool>();

    // Create Discovery Database
    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database =
            std::make_shared<ddspipe::core::DiscoveryDatabase>();

    // Create and populate Participant Database
    std::shared_ptr<ddspipe::core::ParticipantsDatabase> participant_database =
            std::make_shared<ddspipe::core::ParticipantsDatabase>();

    // Create participants
    std::shared_ptr<spy::participants::SpyParticipantConfiguration> spy_configuration =
        std::make_shared<spy::participants::SpyParticipantConfiguration>();
    std::shared_ptr<spy::participants::SpyParticipant> spy_participant =
            std::make_shared<spy::participants::SpyParticipant>(
                spy_configuration,
                payload_pool,
                discovery_database,
                model);

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
    return pipe;
}

TEST(ParticipantDatabaseTest, trivial)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send message
    auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
    new_data->info.name = "participant::trivial";
    new_data->info.active = true;
    new_data->info.guid = ddspipe::core::testing::random_guid();
    dds_reader->simulate_data_reception(std::move(new_data));

    // Wait
    sleep(1);

    // Check information
    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, name_participant)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send message
    auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
    new_data->info.name = "participant::test";
    new_data->info.active = true;
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    new_data->info.guid = guid_data;
    dds_reader->simulate_data_reception(std::move(new_data));

    // Wait
    sleep(1);

    // Check information
    ASSERT_EQ(model->participant_database_.at(guid_data).name, "participant::test");
    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, guid)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send message
    auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
    new_data->info.name = "participant::test";
    new_data->info.active = true;
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    new_data->info.guid = guid_data;
    dds_reader->simulate_data_reception(std::move(new_data));

    // Wait
    sleep(1);

    // Check information
    ASSERT_EQ(model->participant_database_.at(guid_data).guid, guid_data);

    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, active_true)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send message
    auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
    new_data->info.name = "participant::test";
    new_data->info.active = true;
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    new_data->info.guid = guid_data;
    dds_reader->simulate_data_reception(std::move(new_data));

    // Wait
    sleep(1);

    // Check information
    ASSERT_TRUE(model->participant_database_.at(guid_data).active);

    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, active_false)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send message
    auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
    new_data->info.name = "participant::test";
    new_data->info.active = false;
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    new_data->info.guid = guid_data;
    dds_reader->simulate_data_reception(std::move(new_data));

    // Wait
    sleep(1);

    // Check information
    ASSERT_FALSE(model->participant_database_.at(guid_data).active);

    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, n_msgs_name)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send messages
    for (unsigned int i = 0; i < 10; i++)
    {
        auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
        new_data->info.name = "participant::test"+std::to_string(i);
        new_data->info.active = true;
        ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
        new_data->info.guid = guid_data;
        dds_reader->simulate_data_reception(std::move(new_data));
    }

    unsigned int i = 0;
    for (const auto& participant : model->participant_database_)
    {
        ASSERT_EQ(participant.second.name, "participant::test"+std::to_string(i));
        i++;
    }

    // Wait
    sleep(1);

    // Check information
    ASSERT_EQ(model->participant_database_.size(), 1);
}

TEST(ParticipantDatabaseTest, n_msgs_guid)
{
    // Create Model
    std::shared_ptr<spy::participants::SpyModel> model =
            std::make_shared<spy::participants::SpyModel>();

    const ddspipe::core::types::ParticipantId id_ = "DDS Spy mock participant";

    std::shared_ptr<test::SpyDdsParticipantMock> dds_participant =
            std::make_shared<test::SpyDdsParticipantMock>(id_);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe = create_pipe(model, dds_participant);
    pipe->enable();

    auto dds_reader = dds_participant->participant_reader;
    ASSERT_NE(dds_reader, nullptr);

    // Send messages
    std::vector<ddspipe::core::types::Guid> guid_data;
    for (unsigned int i = 0; i < 10; i++)
    {
        auto new_data = std::make_unique<spy::participants::ParticipantInfoData>();
        new_data->info.name = "participant::test"+std::to_string(i);
        new_data->info.active = true;
        guid_data.push_back(ddspipe::core::testing::random_guid());
        new_data->info.guid = guid_data[i];
        dds_reader->simulate_data_reception(std::move(new_data));
    }

    unsigned int i = 0;
    for (const auto& participant : model->participant_database_)
    {
        ASSERT_EQ(participant.first, guid_data[i]);
        i++;
    }

    // Wait
    sleep(1);

    // Check information
    ASSERT_EQ(model->participant_database_.size(), 1);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
