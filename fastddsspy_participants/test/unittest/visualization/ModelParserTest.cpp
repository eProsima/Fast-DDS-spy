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

#include <ddspipe_core/testing/random_values.hpp>

#include <fastddsspy_participants/visualization/ModelParser.hpp>

using namespace eprosima;

void random_participant_info(
        spy::participants::ParticipantInfo& participant_data,
        bool active = true,
        unsigned int seed = 0)
{
    participant_data.name = ddspipe::core::testing::random_participant_id();
    participant_data.active = active;
    participant_data.guid = ddspipe::core::testing::random_guid(seed);
}

void random_endpoint_info(
        spy::participants::EndpointInfo& endpoint_data,
        ddspipe::core::types::EndpointKind kind,
        bool active = true,
        unsigned int seed = 0,
        ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic(rand() % 15))
{
    endpoint_data.active = active;
    endpoint_data.kind = kind;
    endpoint_data.guid = ddspipe::core::testing::random_guid(seed);
    endpoint_data.topic = topic;
    endpoint_data.discoverer_participant_id = ddspipe::core::testing::random_participant_id(seed);
}

TEST(ModelParserTest, simple_participant)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);

    // Obtain information from model
    std::vector<spy::participants::SimpleParticipantData> result;
    result = spy::participants::ModelParser::participants(model);

    // Create expected return
    std::vector<spy::participants::SimpleParticipantData> expected_result;
    expected_result.push_back({participant.name, participant.guid});

    // Check information
    ASSERT_EQ(result[0].name, expected_result[0].name);
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
}

TEST(ModelParserTest, simple_participant_two_participants)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::Guid guid_data1 = ddspipe::core::testing::random_guid();
    spy::participants::ParticipantInfo participant1 = {
                                                        "hello1",
                                                        guid_data1,
                                                        true
                                                    };
    model.participant_database_.add(guid_data1, participant1);
    ddspipe::core::types::Guid guid_data2 = ddspipe::core::testing::random_guid();
    spy::participants::ParticipantInfo participant2 = {
                                                        "hello2",
                                                        guid_data2,
                                                        true
                                                    };
    model.participant_database_.add(guid_data2, participant2);

    // Obtain information from model
    std::vector<spy::participants::SimpleParticipantData> result;
    result = spy::participants::ModelParser::participants(model);

    // Create expected return
    std::vector<spy::participants::SimpleParticipantData> expected_result;
    expected_result.push_back({"hello1", guid_data1});
    expected_result.push_back({"hello2", guid_data2});

    // Check information
    unsigned int i = 0;
    for (const auto& participant : model.participant_database_)
    {
        ASSERT_EQ(result[i].name, expected_result[i].name);
        ASSERT_EQ(result[i].guid, expected_result[i].guid);
        i++;
    }

}

TEST(ModelParserTest, participants_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);
    // Endpoints
    spy::participants::EndpointInfo endpoint_writer;
    random_endpoint_info(endpoint_writer, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint_writer.guid, endpoint_writer);

    spy::participants::EndpointInfo endpoint_reader;
    random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, 3);
    model.endpoint_database_.add(endpoint_reader.guid, endpoint_reader);

    // Obtain information from model
    std::vector<spy::participants::ComplexParticipantData> result;
    result = spy::participants::ModelParser::participants_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexParticipantData> expected_result;
    spy::participants::ComplexParticipantData fill_expected_result;
    spy::participants::ComplexParticipantData::Endpoint writers = {
        endpoint_writer.topic.m_topic_name,
        endpoint_writer.topic.type_name,
        1
    };
    spy::participants::ComplexParticipantData::Endpoint readers = {
        endpoint_reader.topic.m_topic_name,
        endpoint_reader.topic.type_name,
        1
    };
    fill_expected_result.guid = participant.guid;
    fill_expected_result.name = participant.name;
    fill_expected_result.readers.push_back(readers);
    fill_expected_result.writers.push_back(writers);
    expected_result.push_back(fill_expected_result);

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


TEST(ModelParserTest, complex_participant)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    // Participant
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);
    // Endpoints
    spy::participants::EndpointInfo endpoint_writer;
    random_endpoint_info(endpoint_writer, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint_writer.guid, endpoint_writer);

    spy::participants::EndpointInfo endpoint_reader;
    random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, 3);
    model.endpoint_database_.add(endpoint_reader.guid, endpoint_reader);

    // Obtain information from model
    spy::participants::ComplexParticipantData result;
    result = spy::participants::ModelParser::participants(model, participant.guid);

    // Create expected return
    spy::participants::ComplexParticipantData expected_result;
    spy::participants::ComplexParticipantData::Endpoint writers = {
        endpoint_writer.topic.m_topic_name,
        endpoint_writer.topic.type_name,
        1
    };
    spy::participants::ComplexParticipantData::Endpoint readers = {
        endpoint_reader.topic.m_topic_name,
        endpoint_reader.topic.type_name,
        1
    };
    expected_result.guid = participant.guid;
    expected_result.name = participant.name;
    expected_result.readers.push_back(readers);
    expected_result.writers.push_back(writers);

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

TEST(ModelParserTest, simple_endpoint_writer)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    expected_result.push_back({
                                endpoint.guid,
                                endpoint.discoverer_participant_id,
                                {
                                    endpoint.topic.m_topic_name,
                                    endpoint.topic.type_name
                                }

                            });

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

TEST(ModelParserTest, simple_endpoint_reader)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::reader, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    expected_result.push_back({
                                endpoint.guid,
                                endpoint.discoverer_participant_id,
                                {
                                    endpoint.topic.m_topic_name,
                                    endpoint.topic.type_name
                                }
                            });

    // Check information
    ASSERT_EQ(result[0].guid, expected_result[0].guid);
    ASSERT_EQ(result[0].topic.topic_name, expected_result[0].topic.topic_name);
    ASSERT_EQ(result[0].topic.topic_type, expected_result[0].topic.topic_type);
}

TEST(ModelParserTest, simple_endpoint_writer_readers)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Check information
    ASSERT_EQ(result.size(), 0);
    ASSERT_EQ(model.endpoint_database_.size(), 1);
}

TEST(ModelParserTest, simple_endpoint_reader_writers)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::reader, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Check information
    ASSERT_EQ(result.size(), 0);
    ASSERT_EQ(model.endpoint_database_.size(), 1);
}

TEST(ModelParserTest, endpoint_reader_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);

    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::reader, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::readers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    spy::participants::ComplexEndpointData fill_expected_result;
    fill_expected_result.guid = endpoint.guid;
    fill_expected_result.participant_name = participant.name;
    fill_expected_result.topic.topic_name = endpoint.topic.m_topic_name;
    fill_expected_result.topic.topic_type = endpoint.topic.type_name;
    if (endpoint.topic.topic_qos.durability_qos)    // TODO move to YamlWriter
    {
        fill_expected_result.qos.durability = "transient-local";
    }
    else
    {
        fill_expected_result.qos.durability = "volatile";
    }
    if (endpoint.topic.topic_qos.reliability_qos)
    {
        fill_expected_result.qos.reliability = "best-effort";
    }
    else
    {
        fill_expected_result.qos.reliability = "reliable";
    }
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.participant_name, expected_result[i].participant_name);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

TEST(ModelParserTest, endpoint_writer_verbose)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);

    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    std::vector<spy::participants::ComplexEndpointData> result;
    result = spy::participants::ModelParser::writers_verbose(model);

    // Create expected return
    std::vector<spy::participants::ComplexEndpointData> expected_result;
    spy::participants::ComplexEndpointData fill_expected_result;
    fill_expected_result.guid = endpoint.guid;
    fill_expected_result.participant_name = participant.name;
    fill_expected_result.topic.topic_name = endpoint.topic.m_topic_name;
    fill_expected_result.topic.topic_type = endpoint.topic.type_name;
    if (endpoint.topic.topic_qos.durability_qos)    // TODO move to YamlWriter
    {
        fill_expected_result.qos.durability = "transient-local";
    }
    else
    {
        fill_expected_result.qos.durability = "volatile";
    }
    if (endpoint.topic.topic_qos.reliability_qos)
    {
        fill_expected_result.qos.reliability = "best-effort";
    }
    else
    {
        fill_expected_result.qos.reliability = "reliable";
    }
    expected_result.push_back(fill_expected_result);

    // Check information
    unsigned int i = 0;
    for (const auto& it : result)
    {
        ASSERT_EQ(it.guid, expected_result[i].guid);
        ASSERT_EQ(it.participant_name, expected_result[i].participant_name);
        ASSERT_EQ(it.topic.topic_name, expected_result[i].topic.topic_name);
        ASSERT_EQ(it.topic.topic_type, expected_result[i].topic.topic_type);
        ASSERT_EQ(it.qos.durability, expected_result[i].qos.durability);
        ASSERT_EQ(it.qos.reliability, expected_result[i].qos.reliability);

        i++;
    }

}

TEST(ModelParserTest, complex_endpoint_writer)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);

    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::writer, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::writers(model, endpoint.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    expected_result.guid = endpoint.guid;
    expected_result.participant_name = participant.name;
    expected_result.topic.topic_name = endpoint.topic.m_topic_name;
    expected_result.topic.topic_type = endpoint.topic.type_name;
    if (endpoint.topic.topic_qos.durability_qos)    // TODO move to YamlWriter
    {
        expected_result.qos.durability = "transient-local";
    }
    else
    {
        expected_result.qos.durability = "volatile";
    }
    if (endpoint.topic.topic_qos.reliability_qos)
    {
        expected_result.qos.reliability = "best-effort";
    }
    else
    {
        expected_result.qos.reliability = "reliable";
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.participant_name, expected_result.participant_name);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

TEST(ModelParserTest, complex_endpoint_reader)
{

    // Create model
    spy::participants::SpyModel model;
    // Fill model
    spy::participants::ParticipantInfo participant;
    random_participant_info(participant);
    model.participant_database_.add(participant.guid, participant);


    spy::participants::EndpointInfo endpoint;
    random_endpoint_info(endpoint, ddspipe::core::types::EndpointKind::reader, true, 2);
    model.endpoint_database_.add(endpoint.guid, endpoint);

    // Obtain information from model
    spy::participants::ComplexEndpointData result;
    result = spy::participants::ModelParser::readers(model, endpoint.guid);

    // Create expected return
    spy::participants::ComplexEndpointData expected_result;
    expected_result.guid = endpoint.guid;
    expected_result.participant_name = participant.name;
    expected_result.topic.topic_name = endpoint.topic.m_topic_name;
    expected_result.topic.topic_type = endpoint.topic.type_name;
    if (endpoint.topic.topic_qos.durability_qos)    // TODO move to YamlWriter
    {
        expected_result.qos.durability = "transient-local";
    }
    else
    {
        expected_result.qos.durability = "volatile";
    }
    if (endpoint.topic.topic_qos.reliability_qos)
    {
        expected_result.qos.reliability = "best-effort";
    }
    else
    {
        expected_result.qos.reliability = "reliable";
    }

    // Check information
    ASSERT_EQ(result.guid, expected_result.guid);
    ASSERT_EQ(result.participant_name, expected_result.participant_name);
    ASSERT_EQ(result.topic.topic_name, expected_result.topic.topic_name);
    ASSERT_EQ(result.topic.topic_type, expected_result.topic.topic_type);
    ASSERT_EQ(result.qos.durability, expected_result.qos.durability);
    ASSERT_EQ(result.qos.reliability, expected_result.qos.reliability);
}

TEST(ModelParserTest, simple_topic)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint_writer_1;
    random_endpoint_info(endpoint_writer_1, ddspipe::core::types::EndpointKind::writer, true, 1, topic);
    model.endpoint_database_.add(endpoint_writer_1.guid, endpoint_writer_1);
    spy::participants::EndpointInfo endpoint_writer_2;
    random_endpoint_info(endpoint_writer_2, ddspipe::core::types::EndpointKind::writer, true, 2, topic);
    model.endpoint_database_.add(endpoint_writer_2.guid, endpoint_writer_2);
    spy::participants::EndpointInfo endpoint_reader;
    random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, 3, topic);
    model.endpoint_database_.add(endpoint_reader.guid, endpoint_reader);

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

TEST(ModelParserTest, complex_topic)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::DdsTopic topic;
    topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint_writer_1;
    random_endpoint_info(endpoint_writer_1, ddspipe::core::types::EndpointKind::writer, true, 1, topic);
    model.endpoint_database_.add(endpoint_writer_1.guid, endpoint_writer_1);
    spy::participants::EndpointInfo endpoint_writer_2;
    random_endpoint_info(endpoint_writer_2, ddspipe::core::types::EndpointKind::writer, true, 2, topic);
    model.endpoint_database_.add(endpoint_writer_2.guid, endpoint_writer_2);
    spy::participants::EndpointInfo endpoint_reader;
    random_endpoint_info(endpoint_reader, ddspipe::core::types::EndpointKind::reader, true, 3, topic);
    model.endpoint_database_.add(endpoint_reader.guid, endpoint_reader);

    // Obtain information from model
    spy::participants::ComplexTopicData result;
    result = spy::participants::ModelParser::topics(model, topic.m_topic_name);

    // Create expected return
    std::vector<spy::participants::ComplexTopicData::Endpoint> datawriters;
    datawriters.push_back({endpoint_writer_1.guid});
    datawriters.push_back({endpoint_writer_2.guid});
    std::vector<spy::participants::ComplexTopicData::Endpoint> datareaders;
    datareaders.push_back({endpoint_reader.guid});
    spy::participants::ComplexTopicData expected_result;
    expected_result.name = topic.m_topic_name;
    expected_result.type = topic.type_name;
    expected_result.datawriters = datawriters;
    expected_result.datareaders = datareaders;
    expected_result.discovered = true;

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
    // ASSERT_EQ(result.discovered, expected_result.discovered); fail???
    // TODO test Rate
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
