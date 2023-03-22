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
#include <ddspipe_participants/writer/auxiliar/BaseWriter.hpp>
#include <ddspipe_participants/reader/auxiliar/BaseReader.hpp>
#include <ddspipe_participants/participant/auxiliar/BlankParticipant.hpp>

using namespace eprosima;

namespace test {

const unsigned int DOMAIN = 222;

struct SpyDdsDataMock : public ddspipe::core::IRoutingData
{
public:

    // bool operator ==(
    //         const SpyDdsDataMock& other) const;

    ddspipe::core::types::TopicInternalTypeDiscriminator internal_type_discriminator() const noexcept
    {
        return spy::participants::INTERNAL_TOPIC_TYPE_PARTICIPANTS_INFO;
    }

    std::string data;
};

class SpyDdsWriterMock : public ddspipe::participants::BaseWriter
{
public:

    SpyDdsWriterMock(
            const ddspipe::core::types::ParticipantId& id)
            : ddspipe::participants::BaseWriter(id)
    {
    }

    utils::ReturnCode write_nts_(
            ddspipe::core::IRoutingData& data) noexcept
    {
        return utils::ReturnCode::RETCODE_OK;
    }


};

class SpyDdsReaderMock : public ddspipe::participants::BaseReader
{
public:

    SpyDdsReaderMock(
            const ddspipe::core::types::ParticipantId& id)
            : ddspipe::participants::BaseReader(id)
    {
    }

    void simulate_data_reception(
        test::SpyDdsDataMock& data)
    {
        data_queue_.push_back(std::move(data));
        on_data_available_();
    }

    utils::ReturnCode take_nts_(
            std::unique_ptr<ddspipe::core::IRoutingData>& data) noexcept
    {
        return utils::ReturnCode::RETCODE_OK;
    }

protected:
    std::vector<test::SpyDdsDataMock> data_queue_;
};


class SpyDdsParticipantMock : public ddspipe::participants::BlankParticipant
{
public:

    //! Constructor with Id of this participant
    SpyDdsParticipantMock(
            const ddspipe::core::types::ParticipantId& id)
            : ddspipe::participants::BlankParticipant(id)
    {

    }

    //! Override id() IParticipant method
    ddspipe::core::types::ParticipantId id() const noexcept
    {
        return id_;
    }

    //! Override is_repeater() IParticipant method
    bool is_repeater() const noexcept
    {
        return false;
    }

    //! Override is_rtps_kind() IParticipant method
    bool is_rtps_kind() const noexcept
    {
        return false;
    }

    //! Override create_writer() IParticipant method
    std::shared_ptr<ddspipe::core::IWriter> create_writer(
            const ddspipe::core::ITopic& topic)
    {
        return std::make_shared<test::SpyDdsWriterMock>(id_);
    }

    //! Override create_reader() IParticipant method
    std::shared_ptr<ddspipe::core::IReader> create_reader(
            const ddspipe::core::ITopic& topic)
    {
        return std::make_shared<test::SpyDdsReaderMock>(id_);
    }

    std::shared_ptr<test::SpyDdsWriterMock> get_writer(
            const ddspipe::core::ITopic& topic)
    {
        return std::make_shared<test::SpyDdsWriterMock>(id_);
    }

    std::shared_ptr<test::SpyDdsReaderMock> get_reader(
            const ddspipe::core::ITopic& topic)
    {
        return std::make_shared<test::SpyDdsReaderMock>(id_);
    }

protected:

    //! Participant Id
    const ddspipe::core::types::ParticipantId id_;
};


}

TEST(NetworkDatabaseTest, trivial)
{
    // Create Configuration
    spy::yaml::Configuration configuration;

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
    std::shared_ptr<spy::participants::SpyParticipant> spy_participant =
            std::make_shared<spy::participants::SpyParticipant>(
                configuration.spy_configuration,
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


    ddspipe::core::types::DomainId domainId;
    domainId.domain_id = test::DOMAIN;
    configuration.simple_configuration->domain = domainId;

    // Create allowed topics list
    std::shared_ptr<ddspipe::core::AllowedTopicList> allowed_topics =
            std::make_shared<ddspipe::core::AllowedTopicList>(
                configuration.allowlist,
                configuration.blocklist);

    // Create Thread Pool
    std::shared_ptr<utils::SlotThreadPool> thread_pool =
            std::make_shared<utils::SlotThreadPool>(configuration.n_threads);

    // Create DDS Pipe
    std::shared_ptr<ddspipe::core::DdsPipe> pipe =
            std::make_unique<ddspipe::core::DdsPipe>(
                allowed_topics,
                discovery_database,
                payload_pool,
                participant_database,
                thread_pool,
                configuration.builtin_topics,
                true
                );

    pipe->enable();

    // Create reader and writer
    ddspipe::core::types::DistributedTopic topic_1;
    topic_1.m_topic_name = "topic1";
    auto dds_reader = dds_participant->get_reader(topic_1);
    ASSERT_NE(dds_reader, nullptr);

    auto spy_reader = spy_participant->create_writer(topic_1);
    ASSERT_NE(spy_reader, nullptr);

    // Send N messages
    for (unsigned int i = 0; i < 10; i++)
    {
        test::SpyDdsDataMock new_data;
        new_data.data = "reader::" + std::to_string(i);
        dds_reader->simulate_data_reception(new_data);
    }

    std::cout << "Information database !!!" << std::endl;
    for (const auto& participant : model->participant_database_)
    {
        std::cout << "participant_database_: " << std::endl;
        std::cout << "guid: " << participant.first << std::endl;
        std::cout << "name: " << participant.second.name << std::endl;
    }

    for (const auto& endpoint : model->endpoint_database_)
    {
        std::cout << "endpoint_database_: " << std::endl;
        std::cout << "guid: " << endpoint.first << std::endl;
        std::cout << "guid: " << endpoint.second.guid << std::endl;
        std::cout << "topic name: " << endpoint.second.topic.m_topic_name << std::endl;
        std::cout << "type name: " << endpoint.second.topic.type_name << std::endl;
    }

}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
