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
    std::vector<spy::participants::SimpleParticipantData> desire_result;
    desire_result.push_back({"hello", guid_data});

    // Check information
    ASSERT_EQ(result[0].name, desire_result[0].name);
    ASSERT_EQ(result[0].guid, desire_result[0].guid);
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
    std::vector<spy::participants::SimpleParticipantData> desire_result;
    desire_result.push_back({"hello1", guid_data1});
    desire_result.push_back({"hello2", guid_data2});

    // Check information
    unsigned int i = 0;
    for (const auto& participant : model.participant_database_)
    {
        ASSERT_EQ(result[i].name, desire_result[i].name);
        ASSERT_EQ(result[i].guid, desire_result[i].guid);
        i++;
    }

}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
