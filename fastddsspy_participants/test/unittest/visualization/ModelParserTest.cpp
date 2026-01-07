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

#include <cpp_utils/ros2_mangling.hpp>

#include <fastddsspy_participants/testing/random_values.hpp>
#include <fastddsspy_participants/testing/dynamic_types_utils.hpp>

#include <fastddsspy_participants/visualization/ModelParser.hpp>

using namespace eprosima;

std::vector<spy::participants::ParticipantInfo> fill_database_participants(
        spy::participants::SpyModel& model,
        int n_participants)
{
    // Fill model
    std::vector<spy::participants::ParticipantInfo> participants;
    for (int i = 0; i < n_participants; i++)
    {
        spy::participants::ParticipantInfo participant;
        spy::participants::random_participant_info(participant);
        model.participant_database_.add(participant.guid, participant);
        participants.push_back(participant);
    }
    return participants;
}

std::vector<spy::participants::EndpointInfoData> fill_database_endpoints(
        spy::participants::SpyModel& model,
        int n_readers,
        int n_writers,
        ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic())
{
    // Fill model
    std::vector<spy::participants::EndpointInfoData> endpoints;
    for (int i = 0; i < n_readers; i++)
    {
        spy::participants::EndpointInfoData endpoint_reader;
        spy::participants::random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, i,
                topic);
        model.endpoint_database_.add(endpoint_reader.info.guid, endpoint_reader);
        endpoints.push_back(endpoint_reader);
    }

    for (int i = 0; i < n_writers; i++)
    {
        spy::participants::EndpointInfoData endpoint_writer;
        spy::participants::random_endpoint_info(endpoint_writer, ddspipe::core::types::EndpointKind::writer, true,
                n_readers + i, topic);
        model.endpoint_database_.add(endpoint_writer.info.guid, endpoint_writer);
        endpoints.push_back(endpoint_writer);
    }
    return endpoints;
}

std::vector<spy::participants::EndpointInfoData> fill_database_endpoints_filtered(
        spy::participants::SpyModel& model,
        int n_readers,
        int n_writers,
        ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic())
{
    bool active = true;
    // Fill model
    std::vector<spy::participants::EndpointInfoData> endpoints;
    for (int i = 0; i < n_readers; i++)
    {
        spy::participants::EndpointInfoData endpoint_reader;
        spy::participants::random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, active, i,
                topic);
        model.endpoint_database_.add(endpoint_reader.info.guid, endpoint_reader);
        endpoints.push_back(endpoint_reader);
        active = !active;
    }

    for (int i = 0; i < n_writers; i++)
    {
        spy::participants::EndpointInfoData endpoint_writer;
        spy::participants::random_endpoint_info(endpoint_writer, ddspipe::core::types::EndpointKind::writer, active,
                n_readers + i, topic);
        model.endpoint_database_.add(endpoint_writer.info.guid, endpoint_writer);
        endpoints.push_back(endpoint_writer);
        active = !active;
    }
    return endpoints;
}

/*********
* TESTS **
*********/
/**
 * Test the functions of ModelParser.cpp adding elements to the database
 * and check if the functions return the correct information.
 * TODO: extend for multiple participants and getting multiple entities of a kind
 */

/**
 * Add a participant to the database and execute participants()
 * Check the result guid and name of the participant.
 */
TEST(ModelParserTest, simple_participant)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);

    // Obtain information from model
    std::vector<spy::participants::SimpleParticipantData> result;
    result = spy::participants::ModelParser::participants(model);

    // Create expected return
    std::vector<spy::participants::SimpleParticipantData> expected_result;
    for (const auto& it : participants)
    {
        expected_result.push_back({it.name, it.guid});
    }

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
}

/**
 * Add n participants to the database and eexecute participants()
 * Check the result guid and name of each participant.
 */
TEST(ModelParserTest, simple_participant_n_participants)
{
    // Create model
    spy::participants::SpyModel model;
    int n_participants = 4;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);

    // Obtain information from model
    std::vector<spy::participants::SimpleParticipantData> result;
    result = spy::participants::ModelParser::participants(model);

    // Create expected return
    std::vector<spy::participants::SimpleParticipantData> expected_result;
    for (const auto& it : participants)
    {
        expected_result.push_back({it.name, it.guid});
    }

    // Check information
    for (unsigned int i = 0; i++; model.participant_database_.size())
    {
        ASSERT_EQ(result[i].name, expected_result[i].name);
        ASSERT_EQ(result[i].guid, expected_result[i].guid);
    }

}

/**
 * Add a participant, a DDS reader and a DDS writer (with ros2-types = false)
 * to the database and execute participants_verbose().
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, participants_verbose_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model;

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, n_readers, n_writers);

    // Obtain information from model
    std::vector<spy::participants::ComplexParticipantData> result;
    result = spy::participants::ModelParser::participants_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexParticipantData> expected_result;
    spy::participants::ComplexParticipantData fill_expected_result;

    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }

    for (const auto& it : participants)
    {
        spy::participants::ComplexParticipantData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.name = it.name;
        fill_expected_result.readers = readers;
        fill_expected_result.writers = writers;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.name, expected_result[i].name);
        unsigned int l = 0;
        for (const auto& writer : it.writers)
        {
            ASSERT_EQ(writer.topic_name, expected_result[i].writers[l].topic_name);
            ASSERT_EQ(writer.topic_type, expected_result[i].writers[l].topic_type);
            ASSERT_EQ(writer.number, expected_result[i].writers[l].number);
            l++;
        }
        l = 0;
        for (const auto& reader : it.readers)
        {
            ASSERT_EQ(reader.topic_name, expected_result[i].readers[l].topic_name);
            ASSERT_EQ(reader.topic_type, expected_result[i].readers[l].topic_type);
            ASSERT_EQ(reader.number, expected_result[i].readers[l].number);
            l++;
        }
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);
}

/**
 * Add a participant, a DDS reader and a DDS writer (with ros2-types = true)
 * to the database and execute participants_verbose().
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, participants_verbose_ros2_types_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model(true);

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, n_readers, n_writers);

    // Obtain information from model
    std::vector<spy::participants::ComplexParticipantData> result;
    result = spy::participants::ModelParser::participants_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexParticipantData> expected_result;
    spy::participants::ComplexParticipantData fill_expected_result;

    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }

    for (const auto& it : participants)
    {
        spy::participants::ComplexParticipantData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.name = it.name;
        fill_expected_result.readers = readers;
        fill_expected_result.writers = writers;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.name, expected_result[i].name);
        unsigned int l = 0;
        for (const auto& writer : it.writers)
        {
            ASSERT_EQ(writer.topic_name, expected_result[i].writers[l].topic_name);
            ASSERT_EQ(writer.topic_type, expected_result[i].writers[l].topic_type);
            ASSERT_EQ(writer.number, expected_result[i].writers[l].number);
            l++;
        }
        l = 0;
        for (const auto& reader : it.readers)
        {
            ASSERT_EQ(reader.topic_name, expected_result[i].readers[l].topic_name);
            ASSERT_EQ(reader.topic_type, expected_result[i].readers[l].topic_type);
            ASSERT_EQ(reader.number, expected_result[i].readers[l].number);
            l++;
        }
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);
}

/**
 * Add a participant, a ROS 2 reader and a ROS 2 writer (with ros2-types = false)
 * to the database and execute participants_verbose().
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, participants_verbose_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model;

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, n_readers, n_writers, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexParticipantData> result;
    result = spy::participants::ModelParser::participants_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexParticipantData> expected_result;
    spy::participants::ComplexParticipantData fill_expected_result;

    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }

    for (const auto& it : participants)
    {
        spy::participants::ComplexParticipantData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.name = it.name;
        fill_expected_result.readers = readers;
        fill_expected_result.writers = writers;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.name, expected_result[i].name);
        unsigned int l = 0;
        for (const auto& writer : it.writers)
        {
            ASSERT_EQ(writer.topic_name, expected_result[i].writers[l].topic_name);
            ASSERT_EQ(writer.topic_type, expected_result[i].writers[l].topic_type);
            ASSERT_EQ(writer.number, expected_result[i].writers[l].number);
            l++;
        }
        l = 0;
        for (const auto& reader : it.readers)
        {
            ASSERT_EQ(reader.topic_name, expected_result[i].readers[l].topic_name);
            ASSERT_EQ(reader.topic_type, expected_result[i].readers[l].topic_type);
            ASSERT_EQ(reader.number, expected_result[i].readers[l].number);
            l++;
        }
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);
}

/**
 * Add a participant, a ROS 2 reader and a ROS 2 writer (with ros2-types = true)
 * to the database and execute participants_verbose().
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, participants_verbose_ros2_types_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model(true);

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();

    endpoints = fill_database_endpoints(model, n_readers, n_writers, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexParticipantData> result;
    result = spy::participants::ModelParser::participants_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexParticipantData> expected_result;
    spy::participants::ComplexParticipantData fill_expected_result;

    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name),
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name),
                1
            });
        }

    }

    for (const auto& it : participants)
    {
        spy::participants::ComplexParticipantData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.name = it.name;
        fill_expected_result.readers = readers;
        fill_expected_result.writers = writers;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.name, expected_result[i].name);
        unsigned int l = 0;
        for (const auto& writer : it.writers)
        {
            ASSERT_EQ(writer.topic_name, expected_result[i].writers[l].topic_name);
            ASSERT_EQ(writer.topic_type, expected_result[i].writers[l].topic_type);
            ASSERT_EQ(writer.number, expected_result[i].writers[l].number);
            l++;
        }
        l = 0;
        for (const auto& reader : it.readers)
        {
            ASSERT_EQ(reader.topic_name, expected_result[i].readers[l].topic_name);
            ASSERT_EQ(reader.topic_type, expected_result[i].readers[l].topic_type);
            ASSERT_EQ(reader.number, expected_result[i].readers[l].number);
            l++;
        }
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);
}

/**
 * Add a participant, a DDS reader and a DDS writer (with ros2-types = false)
 * to the database and execute participants(guid).
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, complex_participant_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model;

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, n_readers, n_writers);

    // Obtain information from model
    spy::participants::ComplexParticipantData result;
    result = spy::participants::ModelParser::participants(model, spy::participants::random_guid_same_prefix());

    // Create expected return
    spy::participants::ComplexParticipantData expected_result;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }
    for (const auto& it : participants)
    {
        expected_result.guid = it.guid;
        expected_result.name = it.name;
        expected_result.readers = readers;
        expected_result.writers = writers;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.name, expected_result.name);
    unsigned int i = 0;
    for (const auto& writer : result.writers)
    {
        ASSERT_EQ(writer.topic_name, expected_result.writers[i].topic_name);
        ASSERT_EQ(writer.topic_type, expected_result.writers[i].topic_type);
        ASSERT_EQ(writer.number, expected_result.writers[i].number);
        i++;
    }
    i = 0;
    for (const auto& reader : result.readers)
    {
        ASSERT_EQ(reader.topic_name, expected_result.readers[i].topic_name);
        ASSERT_EQ(reader.topic_type, expected_result.readers[i].topic_type);
        ASSERT_EQ(reader.number, expected_result.readers[i].number);
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);

}

/**
 * Add a participant, a DDS reader and a DDS writer (with ros2-types = true)
 * to the database and execute participants(guid).
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, complex_participant_ros2_types_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model(true);

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, n_readers, n_writers);

    // Obtain information from model
    spy::participants::ComplexParticipantData result;
    result = spy::participants::ModelParser::participants(model, spy::participants::random_guid_same_prefix());

    // Create expected return
    spy::participants::ComplexParticipantData expected_result;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }
    for (const auto& it : participants)
    {
        expected_result.guid = it.guid;
        expected_result.name = it.name;
        expected_result.readers = readers;
        expected_result.writers = writers;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.name, expected_result.name);
    unsigned int i = 0;
    for (const auto& writer : result.writers)
    {
        ASSERT_EQ(writer.topic_name, expected_result.writers[i].topic_name);
        ASSERT_EQ(writer.topic_type, expected_result.writers[i].topic_type);
        ASSERT_EQ(writer.number, expected_result.writers[i].number);
        i++;
    }
    i = 0;
    for (const auto& reader : result.readers)
    {
        ASSERT_EQ(reader.topic_name, expected_result.readers[i].topic_name);
        ASSERT_EQ(reader.topic_type, expected_result.readers[i].topic_type);
        ASSERT_EQ(reader.number, expected_result.readers[i].number);
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);

}

/**
 * Add a participant, a ROS 2 reader and a ROS 2 writer (with ros2-types = false)
 * to the database and execute participants(guid).
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, complex_participant_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model;

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    // Fill model
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();

    endpoints = fill_database_endpoints(model, n_readers, n_writers, topic);

    // Obtain information from model
    spy::participants::ComplexParticipantData result;
    result = spy::participants::ModelParser::participants(model, spy::participants::random_guid_same_prefix());

    // Create expected return
    spy::participants::ComplexParticipantData expected_result;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                it.info.topic.m_topic_name,
                it.info.topic.type_name,
                1
            });
        }

    }
    for (const auto& it : participants)
    {
        expected_result.guid = it.guid;
        expected_result.name = it.name;
        expected_result.readers = readers;
        expected_result.writers = writers;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.name, expected_result.name);
    unsigned int i = 0;
    for (const auto& writer : result.writers)
    {
        ASSERT_EQ(writer.topic_name, expected_result.writers[i].topic_name);
        ASSERT_EQ(writer.topic_type, expected_result.writers[i].topic_type);
        ASSERT_EQ(writer.number, expected_result.writers[i].number);
        i++;
    }
    i = 0;
    for (const auto& reader : result.readers)
    {
        ASSERT_EQ(reader.topic_name, expected_result.readers[i].topic_name);
        ASSERT_EQ(reader.topic_type, expected_result.readers[i].topic_type);
        ASSERT_EQ(reader.number, expected_result.readers[i].number);
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);

}

/**
 * Add a participant, a ROS 2 reader and a ROS 2 writer (with ros2-types = true)
 * to the database and execute participants(guid).
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, complex_participant_ros2_types_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model(true);

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, n_readers, n_writers, topic);

    // Obtain information from model
    spy::participants::ComplexParticipantData result;
    result = spy::participants::ModelParser::participants(model, spy::participants::random_guid_same_prefix());

    // Create expected return
    spy::participants::ComplexParticipantData expected_result;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> writers;
    std::vector<spy::participants::ComplexParticipantData::Endpoint> readers;
    for (const auto& it : endpoints)
    {
        if (it.info.is_writer())
        {
            writers.push_back({
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name),
                1
            });
        }
        if (it.info.is_reader())
        {
            readers.push_back({
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name),
                1
            });
        }

    }
    for (const auto& it : participants)
    {
        expected_result.guid = it.guid;
        expected_result.name = it.name;
        expected_result.readers = readers;
        expected_result.writers = writers;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.name, expected_result.name);
    unsigned int i = 0;
    for (const auto& writer : result.writers)
    {
        ASSERT_EQ(writer.topic_name, expected_result.writers[i].topic_name);
        ASSERT_EQ(writer.topic_type, expected_result.writers[i].topic_type);
        ASSERT_EQ(writer.number, expected_result.writers[i].number);
        i++;
    }
    i = 0;
    for (const auto& reader : result.readers)
    {
        ASSERT_EQ(reader.topic_name, expected_result.readers[i].topic_name);
        ASSERT_EQ(reader.topic_type, expected_result.readers[i].topic_type);
        ASSERT_EQ(reader.number, expected_result.readers[i].number);
        i++;
    }
    ASSERT_EQ(model.participant_database_.size(), 1);
    ASSERT_EQ(model.endpoint_database_.size(), 2);

}

/**
 * Add a DDS writer (with ros2-types = false) to the database and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_dds_endpoint_writer)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a DDS writer (with ros2-types = true) to the database and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_dds_endpoint_writer_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 writer (with ros2-types = false) to the database and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_ros2_endpoint_writer)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 writer (with ros2-types = true) to the database and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_ros2_endpoint_writer_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name)
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a DDS reader (with ros2-types = false) to the database and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_dds_endpoint_reader)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a DDS reader (with ros2-types = true) to the database and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_dds_endpoint_reader_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 reader (with ros2-types = false) to the database and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_ros2_endpoint_reader)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 reader (with ros2-types = true) to the database and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_ros2_endpoint_reader_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name)
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a writer to the database and execute readers().
 * Check that returns nothing because no reader has been added.
 */
TEST(ModelParserTest, simple_endpoint_writer_readers)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Check information
    ASSERT_EQ(result.size(), 0);
    ASSERT_EQ(model.endpoint_database_.size(), 1);
}

/**
 * Add a reader to the database and execute writers().
 * Check that returns nothing because no writer has been added.
 */
TEST(ModelParserTest, simple_endpoint_reader_writers)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Check information
    ASSERT_EQ(result.size(), 0);
    ASSERT_EQ(model.endpoint_database_.size(), 1);
}

/**
 * Add a DDS reader and a participant (with ros2-types = false) to the database
 * and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, dds_endpoint_reader_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a DDS reader and a participant (with ros2-types = true) to the database
 * and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, dds_endpoint_reader_verbose_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a ROS 2 reader and a participant (with ros2-types = false) to the database
 * and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, ros2_endpoint_reader_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a ROS 2 reader and a participant (with ros2-types = true) to the database
 * and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, ros2_endpoint_reader_verbose_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        fill_expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a DDS writer and a participant (with ros2-types = false) to the database
 * and execute writers_verbose().
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, dds_endpoint_writer_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a DDS writer and a participant (with ros2-types = true) to the database
 * and execute writers_verbose().
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, dds_endpoint_writer_verbose_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a ROS 2 writer and a participant (with ros2-types = false) to the database
 * and execute writers_verbose().
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, ros2_endpoint_writer_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a ROS 2 writer and a participant (with ros2-types = true) to the database
 * and execute writers_verbose().
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, ros2_endpoint_writer_verbose_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        fill_expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add a DDS writer and a participant (with ros2-types = false) to the database
 * and execute writers(guid).
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, complex_dds_endpoint_writer)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a DDS writer and a participant (with ros2-types = true) to the database
 * and execute writers(guid).
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, complex_dds_endpoint_writer_ros2_types)
{

    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a ROS 2 writer and a participant (with ros2-types = false) to the database
 * and execute writers(guid).
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, complex_ros2_endpoint_writer)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a ROS 2 writer and a participant (with ros2-types = true) to the database
 * and execute writers(guid).
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, complex_ros2_endpoint_writer_ros2_types)
{

    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 0, 1, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a DDS reader and a participant (with ros2-types = false) to the database
 * and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_dds_endpoint_reader)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a DDS reader and a participant (with ros2-types = true) to the database
 * and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_dds_endpoint_reader_ros2_types)
{

    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a ROS 2 reader and a participant (with ros2-types = false) to the database
 * and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_ros2_endpoint_reader)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a ROS 2 reader and a participant (with ros2-types = true) to the database
 * and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_ros2_endpoint_reader_ros2_types)
{

    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints(model, 1, 0, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = false) with the same topic
 * to the database and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = true) with the same topic
 * to the database and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_dds_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = false) with the same topic
 * to the database and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = true) with the same topic
 * to the database and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_ros2_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        utils::demangle_if_ros_topic(topic.m_topic_name),
        utils::demangle_if_ros_type(topic.type_name),
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = false) with the same topic
 * to the database and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = true) with the same topic
 * to the database and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_dds_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = false) with the same topic
 * to the database and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = true) with the same topic
 * to the database and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_ros2_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = utils::demangle_if_ros_topic(topic.m_topic_name);
    fill_expected_result.type = utils::demangle_if_ros_type(topic.type_name);
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = false) with the same topic
 * to the database and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_dds_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add one DDS reader and two DDS writers (with ros2-types = true) with the same topic
 * to the database and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_dds_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = false) with the same topic
 * to the database and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_ros2_endpoints)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add one ROS 2 reader and two ROS 2 writers (with ros2-types = true) with the same topic
 * to the database and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_ros2_endpoints_ros2_types)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = utils::demangle_if_ros_topic(topic.m_topic_name);
    expected_result.type = utils::demangle_if_ros_type(topic.type_name);
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Test topics_keys returns empty for topics without type discovered
 */
TEST(ModelParserTest, topics_keys_type_not_discovered)
{
    spy::participants::SpyModel model;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "TestTopic";
    topic.type_name = "TestType";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Add endpoints but don't register type
    fill_database_endpoints(model, 1, 2, topic);
    
    // Execute topics_keys
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    
    // Should return empty because type is not discovered
    ASSERT_EQ(result.size(), 0);
}

/**
 * Test topics_keys with single key field and single instance
 */
TEST(ModelParserTest, topics_keys_single_key_single_instance)
{
    spy::participants::SpyModel model;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "TestTopic";
    topic.type_name = "TestType";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create type with single key
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("TestType", key_names);
    
    // Register type
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type, type_id);
    
    // Add endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 1, topic);
    
    // Create and add data with specific key
    auto writer_guid = endpoints[1].info.guid; // Get writer GUID
    std::map<std::string, int32_t> key_values = {{"id", 123}};
    auto data_ptr = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    
    // Move the data to add_data (since RtpsPayloadData has deleted copy)
    model.add_data(topic, *data_ptr);
    
    // Execute topics_keys
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    
    // Verify results
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].topic_name, "TestTopic");
    ASSERT_EQ(result[0].key_fields.size(), 1);
    ASSERT_EQ(result[0].key_fields[0], "id");
    ASSERT_EQ(result[0].instance_count, 1);
    ASSERT_EQ(result[0].instances.size(), 1);
    
    // Verify instance contains key value (JSON format)
    ASSERT_TRUE(result[0].instances[0].find("123") != std::string::npos);
}

/**
 * Test topics_keys with multiple key fields and multiple instances
 */
TEST(ModelParserTest, topics_keys_multiple_keys_multiple_instances)
{
    spy::participants::SpyModel model;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "MultiKeyTopic";
    topic.type_name = "MultiKeyType";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create type with multiple keys
    std::vector<std::string> key_names = {"sensor_id", "location_id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("MultiKeyType", key_names);
    
    // Register type
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type, type_id);
    
    // Add endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 1, topic);
    auto writer_guid = endpoints[1].info.guid;
    
    // Add multiple instances
    std::vector<std::map<std::string, int32_t>> instances_keys = {
        {{"sensor_id", 1}, {"location_id", 100}},
        {{"sensor_id", 2}, {"location_id", 100}},
        {{"sensor_id", 1}, {"location_id", 200}}
    };
    
    for (const auto& keys : instances_keys)
    {
        auto data_ptr = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, keys, writer_guid);
        model.add_data(topic, *data_ptr);
    }
    
    // Execute topics_keys
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    
    // Verify results
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].topic_name, "MultiKeyTopic");
    ASSERT_EQ(result[0].key_fields.size(), 2);
    ASSERT_TRUE(std::find(result[0].key_fields.begin(), result[0].key_fields.end(), "sensor_id") 
                != result[0].key_fields.end());
    ASSERT_TRUE(std::find(result[0].key_fields.begin(), result[0].key_fields.end(), "location_id") 
                != result[0].key_fields.end());
    ASSERT_EQ(result[0].instance_count, 3);
    ASSERT_EQ(result[0].instances.size(), 3);
}

/**
 * Test topics_keys with ROS2 types enabled
 */
TEST(ModelParserTest, topics_keys_ros2_types)
{
    spy::participants::SpyModel model(true); // Enable ROS2 types
    
    // Create ROS2 topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/sensor_data";
    topic.type_name = "sensor_msgs::msg::dds_::Temperature_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create type with key
    std::vector<std::string> key_names = {"sensor_id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("sensor_msgs::msg::dds_::Temperature_", key_names);
    
    // Register type
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type, type_id);
    
    // Add endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 1, topic);
    
    // Add data
    auto writer_guid = endpoints[1].info.guid;
    std::map<std::string, int32_t> key_values = {{"sensor_id", 42}};
    auto data_ptr = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    model.add_data(topic, *data_ptr);
    
    // Execute topics_keys
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    
    // Verify ROS2 demangling
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].topic_name, "/sensor_data"); // Demangled
    ASSERT_EQ(result[0].instance_count, 1);
}

/**
 * Test topics_keys with wildcard filter matching specific topics
 */
TEST(ModelParserTest, topics_keys_with_topic_filter)
{
    spy::participants::SpyModel model;
    
    // Create two topics
    ddspipe::core::types::DdsTopic topic1;
    topic1.m_topic_name = "MatchingTopic";
    topic1.type_name = "Type1";
    topic1.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    ddspipe::core::types::DdsTopic topic2;
    topic2.m_topic_name = "OtherTopic";
    topic2.type_name = "Type2";
    topic2.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create types and register
    auto dyn_type1 = eprosima::spy::participants::testing::create_test_type_with_keys("Type1", {"id"});
    auto dyn_type2 = eprosima::spy::participants::testing::create_test_type_with_keys("Type2", {"key"});
    
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type1, type_id);
    model.add_schema(dyn_type2, type_id);
    
    // Add endpoints for both topics
    fill_database_endpoints(model, 1, 1, topic1);
    fill_database_endpoints(model, 1, 1, topic2);
    
    // Create filter that only matches first topic
    ddspipe::core::types::WildcardDdsFilterTopic filter;
    filter.topic_name.set_value("Matching*");
    
    // Execute topics_keys with filter
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(model, filter);
    
    // Should only return matching topic
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].topic_name, "MatchingTopic");
}

/**
 * Test topics_keys instance cleanup when writer disappears
 */
TEST(ModelParserTest, topics_keys_writer_removal)
{
    spy::participants::SpyModel model;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "TestTopic";
    topic.type_name = "TestType";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create type with key
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("TestType", key_names);
    
    // Register type
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type, type_id);
    
    // Add endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints(model, 1, 1, topic);
    auto writer_guid = endpoints[1].info.guid;
    
    // Add data
    std::map<std::string, int32_t> key_values = {{"id", 123}};
    auto data_ptr = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    model.add_data(topic, *data_ptr);
    
    // Verify instance exists
    std::vector<spy::participants::TopicKeysData> result1;
    result1 = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    ASSERT_EQ(result1[0].instance_count, 1);
    
    // Simulate writer going inactive
    model.on_writer_discovered(writer_guid, topic.m_topic_name, false);
    
    // Verify instance removed
    std::vector<spy::participants::TopicKeysData> result2;
    result2 = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    ASSERT_EQ(result2[0].instance_count, 0);
}

/**
 * Test topics_keys with type without keys
 */
TEST(ModelParserTest, topics_keys_no_keys)
{
    spy::participants::SpyModel model;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "NoKeyTopic";
    topic.type_name = "NoKeyType";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    
    // Create type with NO keys
    auto factory = fastdds::dds::DynamicTypeBuilderFactory::get_instance();
    auto type_desc = fastdds::dds::traits<fastdds::dds::TypeDescriptor>::make_shared();
    type_desc->kind(fastdds::dds::TK_STRUCTURE);
    type_desc->name("NoKeyType");
    
    auto type_builder = factory->create_type(type_desc);
    
    // Add only non-key field
    auto value_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
    value_desc->name("value");
    value_desc->type(factory->get_primitive_type(fastdds::dds::TK_INT32));
    value_desc->id(0);
    value_desc->is_key(false);
    type_builder->add_member(value_desc);
    
    auto dyn_type = type_builder->build();
    
    // Register type
    fastdds::dds::xtypes::TypeIdentifier type_id;
    model.add_schema(dyn_type, type_id);
    
    // Add endpoints
    fill_database_endpoints(model, 1, 1, topic);
    
    // Execute topics_keys
    std::vector<spy::participants::TopicKeysData> result;
    result = spy::participants::ModelParser::topics_keys(
        model, ddspipe::core::types::WildcardDdsFilterTopic());
    
    // Should return topic with empty key fields
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].topic_name, "NoKeyTopic");
    ASSERT_EQ(result[0].key_fields.size(), 0);
    ASSERT_EQ(result[0].instance_count, 0);
}


/**
 * Add two DDS writers (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers().
 * Check the result guid, topic name and topic type of the writer who pass.
 */
TEST(ModelParserTest, simple_dds_endpoint_writer_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add two DDS writers (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute writers().
 * Check the result guid, topic name and topic type of the writer who pass.
 */
TEST(ModelParserTest, simple_dds_endpoint_writer_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 writers (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_ros2_endpoint_writer_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a ROS 2 writers (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_ros2_endpoint_writer_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name)
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}


/**
 * Add a DDS readers (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_dds_endpoint_reader_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add two DDS readers (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_dds_endpoint_reader_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add two ROS 2 readers (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_ros2_endpoint_reader_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                it.info.topic.m_topic_name,
                it.info.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add two ROS 2 readers (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_ros2_endpoint_reader_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.push_back({
            it.info.guid,
            it.info.discoverer_participant_id,
            {
                utils::demangle_if_ros_topic(it.info.topic.m_topic_name),
                utils::demangle_if_ros_type(it.info.topic.type_name)
            }

        });
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add two DDS readers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, dds_endpoint_reader_verbose_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two DDS reader and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, dds_endpoint_reader_verbose_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two ROS 2 readers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers_verbose().
 * Check the result guid, topic and qos of the reader who pass.
 */
TEST(ModelParserTest, ros2_endpoint_reader_verbose_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two ROS 2 readers and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers_verbose().
 * Check the result guid, topic and qos of the reader who pass.
 */
TEST(ModelParserTest, ros2_endpoint_reader_verbose_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        fill_expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }


    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two DDS writers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers_verbose().
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, dds_endpoint_writer_verbose_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two DDS writers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers_verbose().
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, dds_endpoint_writer_verbose_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two ROS 2 writers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers_verbose().
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, ros2_endpoint_writer_verbose_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = it.info.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.info.topic.type_name;
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two ROS 2 writers and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute writers_verbose().
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, ros2_endpoint_writer_verbose_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.info.guid;
        fill_expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        fill_expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        fill_expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
        expected_result.push_back(fill_expected_result);
    }

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

/**
 * Add two DDS writers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers(guid).
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, complex_dds_endpoint_writer_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two DDS writers and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute writers(guid).
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, complex_dds_endpoint_writer_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two ROS 2 writers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute writers(guid).
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, complex_ros2_endpoint_writer_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Fill model
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a ROS 2 writers and a participant (with ros2-types = true) to the database
 * and execute writers(guid).
 * Check the result guid, topic and qos of the writer who pass.
 */
TEST(ModelParserTest, complex_ros2_endpoint_writer_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints_filtered(model, 0, 2, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two DDS readers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers(guid).
 * Check the result guid, topic and qos of the reader who pass.
 */
TEST(ModelParserTest, complex_dds_endpoint_reader_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two DDS readers and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers(guid).
 * Check the result guid, topic and qos of the reader who pass.
 */
TEST(ModelParserTest, complex_dds_endpoint_reader_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 0);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two ROS 2 readers and a participant (with ros2-types = false) to the database
 * (1 pass the partition filter the other not) and execute readers(guid).
 * Check the result guid, topic and qos of the reader who pass.
 */
TEST(ModelParserTest, complex_ros2_endpoint_reader_filtered)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = it.info.topic.m_topic_name;
        expected_result.topic.topic_type = it.info.topic.type_name;
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two ROS 2 readers and a participant (with ros2-types = true) to the database
 * (1 pass the partition filter the other not) and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_ros2_endpoint_reader_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    endpoints = fill_database_endpoints_filtered(model, 2, 0, topic);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].info.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        expected_result.guid = it.info.guid;
        expected_result.topic.topic_name = utils::demangle_if_ros_topic(it.info.topic.m_topic_name);
        expected_result.topic.topic_type = utils::demangle_if_ros_type(it.info.topic.type_name);
        expected_result.qos.durability = it.info.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.info.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_dds_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = true) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_dds_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_ros2_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        topic.m_topic_name,
        topic.type_name,
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic_ros2_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::SimpleTopicData> expected_result;
    expected_result.push_back({
        utils::demangle_if_ros_topic(topic.m_topic_name),
        utils::demangle_if_ros_type(topic.type_name),
        2,
        1,
        {
            10,
            "Hz"
        }
    });

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].type, expected_result[0].type);
    ASSERT_EQ(result[0].datawriters, expected_result[0].datawriters);
    ASSERT_EQ(result[0].datareaders, expected_result[0].datareaders);
    ASSERT_FALSE(result[0].rate.rate);
}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_dds_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_dds_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_ros2_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = topic.m_topic_name;
    fill_expected_result.type = topic.type_name;
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = true) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose_ros2_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(
        model, ddspipe::core::types::WildcardDdsFilterTopic());

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    fill_expected_result.name = utils::demangle_if_ros_topic(topic.m_topic_name);
    fill_expected_result.type = utils::demangle_if_ros_type(topic.type_name);
    fill_expected_result.datawriters = datawriters;
    fill_expected_result.datareaders = datareaders;
    expected_result.push_back(fill_expected_result);

    // Check information
    ASSERT_EQ(result.size(), expected_result.size());

    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.name, expected_result[i].name);
        ASSERT_EQ(it.type, expected_result[i].type);
        unsigned int l = 0;
        for (const auto& datawriter : it.datawriters)
        {
            ASSERT_EQ(datawriter.guid, expected_result[i].datawriters[l].guid);
            l++;
        }
        l = 0;
        for (const auto& datareader : it.datareaders)
        {
            ASSERT_EQ(datareader.guid, expected_result[i].datareaders[l].guid);
            l++;
        }
        ASSERT_FALSE(it.rate.rate);
        ASSERT_FALSE(it.discovered);
        i++;
    }

}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_dds_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add two DDS readers and four DDS writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_dds_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    //ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = false) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_ros2_endpoints_filtered)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;

    //ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}

/**
 * Add two ROS 2 readers and four ROS 2 writers (with ros2-types = true) with the same topic
 * to the database (half of them pass the filter, the other half not) and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic_ros2_endpoints_ros2_types_filtered)
{
    // Create model
    spy::participants::SpyModel model(true);
    // Fill model
    // Topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "rt/hello";
    topic.type_name = "std_msgs::msg::dds_::String_";
    topic.topic_qos = ddspipe::core::testing::random_topic_qos();
    // Endpoints
    std::vector<spy::participants::EndpointInfoData> endpoints;
    // (simulate that the half of the endpoints do not pass the partition filter)
    endpoints = fill_database_endpoints_filtered(model, 2, 4, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::complex_topic_data(model, topic);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (!it.info.active)
        {
            continue;
        }

        std::ostringstream ss;
        ss << it.info.guid;
        std::string partition = "";
        const auto partition_it = it.info.specific_partitions.find(ss.str());
        if (partition_it != it.info.specific_partitions.end())
        {
            partition = partition_it->second;
        }

        if (it.info.is_reader())
        {
            datareaders.push_back({it.info.guid, partition});
        }
        if (it.info.is_writer())
        {
            datawriters.push_back({it.info.guid, partition});
        }
    }
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = utils::demangle_if_ros_topic(topic.m_topic_name);
    expected_result.type = utils::demangle_if_ros_type(topic.type_name);
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;


    //ASSERT_EQ(result.size(), expected_result.size());

    // Check information
    ASSERT_EQ(result.name, expected_result.name);
    ASSERT_EQ(result.type, expected_result.type);
    unsigned int i = 0;
    for (const auto& datawriter : result.datawriters)
    {
        ASSERT_EQ(datawriter.guid, expected_result.datawriters[i].guid);
        i++;
    }
    i = 0;
    for (const auto& datareader : result.datareaders)
    {
        ASSERT_EQ(datareader.guid, expected_result.datareaders[i].guid);
        i++;
    }
    ASSERT_FALSE(result.rate.rate);
    ASSERT_FALSE(result.discovered);
}




int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
