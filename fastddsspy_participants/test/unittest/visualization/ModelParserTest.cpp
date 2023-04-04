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

#include <fastddsspy_participants/testing/random_values.hpp>

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

std::vector<spy::participants::EndpointInfo> fill_database_endpoints(
        spy::participants::SpyModel& model,
        int n_readers,
        int n_writers,
        ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic(rand() % 15))
{
    // Fill model
    std::vector<spy::participants::EndpointInfo> endpoints;
    for (int i = 0; i < n_readers; i++)
    {
        spy::participants::EndpointInfo endpoint_reader;
        spy::participants::random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, i,
                topic);
        model.endpoint_database_.add(endpoint_reader.guid, endpoint_reader);
        endpoints.push_back(endpoint_reader);
    }

    for (int i = 0; i < n_writers; i++)
    {
        spy::participants::EndpointInfo endpoint_writer;
        spy::participants::random_endpoint_info(endpoint_writer, ddspipe::core::types::EndpointKind::writer, true,
                n_readers + i, topic);
        model.endpoint_database_.add(endpoint_writer.guid, endpoint_writer);
        endpoints.push_back(endpoint_writer);
    }
    return endpoints;
}

/*********
* TESTS **
*********/
/**
 * Test the functions of ModelParser.cpp adding elements to the database
 * and check if the functions return the correct information.
 */

/**
 * Add a participant to the database and eexecute participants()
 * Check the result guid and name of the participant.
 */
TEST(ModelParserTest, simple_participant)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
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
 * Add a participant, a reader and a writer to the database and execute
 * participants_verbose().
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, participants_verbose)
{
    // Create model
    spy::participants::SpyModel model;

    int n_participants = 1;
    int n_readers = 1;
    int n_writers = 1;
    // Fill model
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, n_participants);
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
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
        if (it.is_writer())
        {
            writers.push_back({
                it.topic.m_topic_name,
                it.topic.type_name,
                1
            });
        }
        if (it.is_reader())
        {
            readers.push_back({
                it.topic.m_topic_name,
                it.topic.type_name,
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
 * Add a participant, a reader and a writer to the database and execute
 * participants(guid).
 * Check the result guid, name, writers and readers of that participant.
 */
TEST(ModelParserTest, complex_participant)
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
    std::vector<spy::participants::EndpointInfo> endpoints;
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
        if (it.is_writer())
        {
            writers.push_back({
                it.topic.m_topic_name,
                it.topic.type_name,
                1
            });
        }
        if (it.is_reader())
        {
            readers.push_back({
                it.topic.m_topic_name,
                it.topic.type_name,
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
 * Add a writer to the database and execute writers().
 * Check the result guid, topic name and topic type of that writer.
 */
TEST(ModelParserTest, simple_endpoint_writer)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.guid,
            it.discoverer_participant_id,
            {
                it.topic.m_topic_name,
                it.topic.type_name
            }

        });
    }

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

/**
 * Add a reader to the database and execute readers().
 * Check the result guid, topic name and topic type of that reader.
 */
TEST(ModelParserTest, simple_endpoint_reader)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.push_back({
            it.guid,
            it.discoverer_participant_id,
            {
                it.topic.m_topic_name,
                it.topic.type_name
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
 * Add a reader and a participant to the database and execute readers_verbose().
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, endpoint_reader_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.topic.topic_name = it.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.topic.type_name;
        fill_expected_result.qos.durability = it.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.topic.topic_qos.reliability_qos;
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
 * Add a writer and a participant to the database and execute writers_verbose().
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, endpoint_writer_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    for (const auto& it : endpoints)
    {
        spy::participants::ComplexEndpointData fill_expected_result;
        fill_expected_result.guid = it.guid;
        fill_expected_result.topic.topic_name = it.topic.m_topic_name;
        fill_expected_result.topic.topic_type = it.topic.type_name;
        fill_expected_result.qos.durability = it.topic.topic_qos.durability_qos;
        fill_expected_result.qos.reliability = it.topic.topic_qos.reliability_qos;
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
 * Add a writer and a participant to the database and execute writers(guid).
 * Check the result guid, topic and qos of that writer.
 */
TEST(ModelParserTest, complex_endpoint_writer)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 0, 1);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoints[0].guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.guid;
        expected_result.topic.topic_name = it.topic.m_topic_name;
        expected_result.topic.topic_type = it.topic.type_name;
        expected_result.qos.durability = it.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add a reader and a participant to the database and execute readers(guid).
 * Check the result guid, topic and qos of that reader.
 */
TEST(ModelParserTest, complex_endpoint_reader)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    std::vector<spy::participants::ParticipantInfo> participants;
    participants = fill_database_participants(model, 1);
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 0);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoints[0].guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    for (const auto& it : endpoints)
    {
        expected_result.guid = it.guid;
        expected_result.topic.topic_name = it.topic.m_topic_name;
        expected_result.topic.topic_type = it.topic.type_name;
        expected_result.qos.durability = it.topic.topic_qos.durability_qos;
        expected_result.qos.reliability = it.topic.topic_qos.reliability_qos;
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

/**
 * Add one reader and two writers with the same topic
 * to the database and execute topics().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, simple_topic)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::SimpleTopicData> result;
    result = spy::participants::ModelParser::topics(model);

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
    // TODO test Rate
}

/**
 * Add one reader and two writers with the same topic
 * to the database and execute topics_verbose().
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, topics_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    std::vector<spy::participants::ComplexTopicData> result;
    result = spy::participants::ModelParser::topics_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData> expected_result;
    spy::participants::ComplexTopicData fill_expected_result;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (it.is_reader())
        {
            datareaders.push_back({it.guid});
        }
        if (it.is_writer())
        {
            datawriters.push_back({it.guid});
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
        // TODO test Rate
        i++;
    }

}

/**
 * Add one reader and two writers with the same topic
 * to the database and execute topics(name).
 * Check the result name, type, writers and readers
 * of that topic.
 */
TEST(ModelParserTest, complex_topic)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    // Endpoints
    std::vector<spy::participants::EndpointInfo> endpoints;
    endpoints = fill_database_endpoints(model, 1, 2, topic);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::topics(model, topic.m_topic_name);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    for (const auto& it : endpoints)
    {
        if (it.is_reader())
        {
            datareaders.push_back({it.guid});
        }
        if (it.is_writer())
        {
            datawriters.push_back({it.guid});
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
    // TODO test Rate
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
