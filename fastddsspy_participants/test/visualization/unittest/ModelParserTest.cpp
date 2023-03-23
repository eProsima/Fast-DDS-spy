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

TEST(ModelParserTest, simple_participant)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    spy::participants::ParticipantInfo participant = {
                                                        "hello",
                                                        guid_data,
                                                        true
                                                    };
    model.participant_database_.add(guid_data, participant);

    // Obtain information from model
    std::vector<spy::participants::SimpleParticipantData> result;
    result = spy::participants::ModelParser::participants(model);

    // Create expected return
    std::vector<spy::participants::SimpleParticipantData> expected_result;
    expected_result.push_back({"hello", guid_data});

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

TEST(ModelParserTest, simple_endpoint_writer)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint = {
                                                    ddspipe::core::types::EndpointKind::writer,
                                                    guid_data,
                                                    topic

                                                };
    model.endpoint_database_.add(guid_data, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    expected_result.push_back({
                                guid_data,
                                "participant_name",
                                {
                                    topic.m_topic_name,
                                    topic.type_name
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
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint = {
                                                    ddspipe::core::types::EndpointKind::reader,
                                                    guid_data,
                                                    topic

                                                };
    model.endpoint_database_.add(guid_data, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::readers(model);

    // Create expected return
    std::vector<spy::participants::SimpleEndpointData> expected_result;
    expected_result.push_back({
                                guid_data,
                                "participant_name",
                                {
                                    topic.m_topic_name,
                                    topic.type_name
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
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint = {
                                                    ddspipe::core::types::EndpointKind::writer,
                                                    guid_data,
                                                    topic

                                                };
    model.endpoint_database_.add(guid_data, endpoint);

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
    ddspipe::core::types::Guid guid_data = ddspipe::core::testing::random_guid();
    ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic();
    spy::participants::EndpointInfo endpoint = {
                                                    ddspipe::core::types::EndpointKind::reader,
                                                    guid_data,
                                                    topic

                                                };
    model.endpoint_database_.add(guid_data, endpoint);

    // Obtain information from model
    std::vector<spy::participants::SimpleEndpointData> result;
    result = spy::participants::ModelParser::writers(model);

    // Check information
    ASSERT_EQ(result.size(), 0);
    ASSERT_EQ(model.endpoint_database_.size(), 1);
}

TEST(ModelParserTest, simple_topic)
{
    // Create model
    spy::participants::SpyModel model;
    // Fill model

    ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic();
    ddspipe::core::types::Guid guid_data_writer_1 = ddspipe::core::testing::random_guid();
    spy::participants::EndpointInfo endpoint_writer_1 = {
                                                        ddspipe::core::types::EndpointKind::writer,
                                                        guid_data_writer_1,
                                                        topic

                                                    };
    model.endpoint_database_.add(guid_data_writer_1, endpoint_writer_1);
    ddspipe::core::types::Guid guid_data_writer_2 = ddspipe::core::testing::random_guid(2);
    spy::participants::EndpointInfo endpoint_writer_2 = {
                                                        ddspipe::core::types::EndpointKind::writer,
                                                        guid_data_writer_2,
                                                        topic

                                                    };
    model.endpoint_database_.add(guid_data_writer_2, endpoint_writer_2);
    ddspipe::core::types::Guid guid_data_reader_1 = ddspipe::core::testing::random_guid(3);
    spy::participants::EndpointInfo endpoint_reader_1 = {
                                                        ddspipe::core::types::EndpointKind::reader,
                                                        guid_data_reader_1,
                                                        topic

                                                    };
    model.endpoint_database_.add(guid_data_reader_1, endpoint_reader_1);

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
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
