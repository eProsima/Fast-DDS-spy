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

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/utils.hpp>

#include <cpp_utils/user_interface/CommandReader.hpp>
#include <cpp_utils/macros/custom_enumeration.hpp>

#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>
#include <ddspipe_yaml/YamlWriter.hpp>

#include <fastddsspy_participants/library/config.h>
#include <fastddsspy_participants/model/SpyModel.hpp>
#include <fastddsspy_participants/visualization/ModelParser.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>
#include "yaml-cpp/yaml.h"

#include "Controller.hpp"
#include "Command.hpp"

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace eprosima {
namespace spy {

// Braces + indentation, arrays single-line
static void print_json_arrays_inline(
        const json& j,
        std::ostream& o,
        int indent,
        int step)
{
    if (j.is_object())
    {
        o << "{\n";
        auto it = j.begin();
        if (it != j.end())
        {
            o << std::string(indent + step, ' ');
            o << json(it.key()).dump() << ": ";              // Quoted key
            print_json_arrays_inline(it.value(), o, indent + step, step);
            ++it;
            for (; it != j.end(); ++it)
            {
                o << ",\n";
                o << std::string(indent + step, ' ');
                o << json(it.key()).dump() << ": ";              // Quoted key
                print_json_arrays_inline(it.value(), o, indent + step, step);
            }
        }
        o << "\n" << std::string(indent, ' ') << "}";
    }
    else if (j.is_array())
    {
        // Inline if all elements are primitives; otherwise, pretty multi-line.
        bool all_primitives = std::all_of(j.begin(), j.end(),
                        [](const json& e)
                        {
                            return e.is_primitive();
                        });

        std::size_t i = 0;
        if (all_primitives)
        {
            o << "[";
            if (i < j.size())
            {
                // Primitives: let nlohmann handle quoting/escaping
                o << j[i++].dump();
                for (; i < j.size(); ++i)
                {
                    // Primitives: let nlohmann handle quoting/escaping
                    o << ", " << j[i].dump();
                }
            }
            o << "]";
        }
        else
        {
            // Structured elements -> readable multi-line
            o << "[\n";
            if (i < j.size())
            {
                o << std::string(indent + step, ' ');
                print_json_arrays_inline(j[i++], o, indent + step, step);
                for (; i < j.size(); ++i)
                {
                    o << ",\n" << std::string(indent + step, ' ');
                    print_json_arrays_inline(j[i], o, indent + step, step);
                }
            }
            o << "\n" << std::string(indent, ' ') << "]";
        }
    }
    else
    {
        // Scalars: let nlohmann handle quoting/escaping/types
        o << j.dump();
    }
}

// Wrapper
static std::string format_json_arrays_inline(
        const std::string& json_str,
        int indent_step = 4)
{
    json j = json::parse(json_str);
    std::ostringstream out;
    print_json_arrays_inline(j, out, /*indent*/ 0, indent_step);
    return out.str();
}

Controller::Controller(
        const yaml::Configuration& configuration)
    : backend_(configuration)
    , model_(backend_.model())
    , configuration_(configuration)
{
    // Do nothing
}

void Controller::run()
{
    view_.print_initial();
    utils::Command<CommandValue> command;
    command.command = CommandValue::participant;
    while (command.command != CommandValue::exit)
    {
        command = input_.wait_next_command();
        // refresh the database if a filter partition is active.
        // this checks if there is a new endpoint that does not
        // pass the filter and disable it
        // TODO. danip remove
        //refresh_database();
        run_command_(command);
    }
}

void Controller::one_shot_run(
        const std::vector<std::string>& args)
{
    utils::sleep_for(configuration_.one_shot_wait_time_ms);
    run_command_(input_.parse_as_command(args));
}

void Controller::refresh_database()
{
    // vector of inactive endpoints to disable it after
    // the filter is applied in all active endpoints
    std::vector<ddspipe::core::types::Guid> v_guid_disable;

    // check if there is a filter partition
    if (partition_filter_set_.size() > 0)
    {
        for (const auto& endpoint: model_->endpoint_database_)
        {
            if (!endpoint.second.info.active)
            {
                // the curent endpoint is not active
                continue;
            }

            // get the source guid of the endpoint
            std::ostringstream ss_guid;
            ss_guid << endpoint.second.info.guid;

            // get the partition set of the endpoint
            std::string endpoint_partitions, curr_partition;
            int i, n;
            bool pass = false;

            endpoint_partitions = endpoint.second.info.specific_partitions.find(ss_guid.str())->second;
            curr_partition = "";
            i = 0;
            n = endpoint_partitions.size();
            // iterate to separete the partitions from the set
            while (i < n)
            {
                if (endpoint_partitions[i] == '|')
                {
                    // check if the current partition is in the filter
                    for (std::string filter_p: partition_filter_set_)
                    {
                        if (utils::match_pattern(filter_p, curr_partition) ||
                                utils::match_pattern(curr_partition, filter_p))
                        {
                            // the current partition matches
                            // with one partition of the filter
                            pass = true;
                            break;
                        }
                    }

                    if (pass)
                    {
                        // it is not necessary to check other partitions
                        // already satisfies the filter partition
                        break;
                    }

                    // reset and check more
                    curr_partition = "";
                }
                else
                {
                    curr_partition += endpoint_partitions[i];
                }

                i++;
            }

            // empty or last partition of the set
            for (std::string filter_p: partition_filter_set_)
            {
                if (utils::match_pattern(filter_p, curr_partition) ||
                        utils::match_pattern(curr_partition, filter_p))
                {
                    // matches with one partition of the filter
                    pass = true;
                    break;
                }
            }

            if (!pass)
            {
                // the active endpoint does not pass the filter
                // (this endpoint is recently discovered
                // without using a filter command)
                v_guid_disable.push_back(endpoint.first);
            }

        }

        // disable the endpoints that did not pass the filter
        for (const auto& curr_guid: v_guid_disable)
        {
            // get the endpoint associated with the guid and disable it.
            auto endpoint_tmp = model_->endpoint_database_.find(curr_guid)->second;
            endpoint_tmp.info.active = false;
            // modify with the new active value
            model_->endpoint_database_.add_or_modify(curr_guid, endpoint_tmp);
        }
    }
}

utils::ReturnCode Controller::reload_configuration(
        yaml::Configuration& new_configuration)
{
    return backend_.reload_configuration(new_configuration);
}

void Controller::run_command_(
        const utils::Command<CommandValue>& command)
{
    switch (command.command)
    {
        case CommandValue::participant:
            participants_command_(command.arguments);
            break;

        case CommandValue::datareader:
            readers_command_(command.arguments);
            break;

        case CommandValue::datawriter:
            writers_command_(command.arguments);
            break;

        case CommandValue::topic:
            topics_command_(command.arguments);
            break;

        case CommandValue::print:
            print_command_(command.arguments);
            break;

        case CommandValue::version:
            version_command_(command.arguments);
            break;

        case CommandValue::help:
            help_command_(command.arguments);
            break;

        case CommandValue::error_input:
            error_command_(command.arguments);
            break;

        case CommandValue::filter:
            filter_command_(command.arguments);
            break;

        default:
            break;
    }
}

fastdds::dds::DynamicData::_ref_type Controller::get_dynamic_data_(
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept
{
    // TODO fast this should not be done, but dyn types API is like it is.
    auto& data_no_const = const_cast<ddspipe::core::types::RtpsPayloadData&>(data);

    // Create PubSub Type
    fastdds::dds::DynamicPubSubType pubsub_type(dyn_type);
    fastdds::dds::DynamicData::_ref_type dyn_data(fastdds::dds::DynamicDataFactory::get_instance()->create_data(
                dyn_type));

    pubsub_type.deserialize(data_no_const.payload, &dyn_data);

    return dyn_data;
}

void Controller::data_stream_callback_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
    // Block entrance so prints does not collapse
    std::lock_guard<std::mutex> _(view_mutex_);

    // Get deserializad data
    auto dyn_data = get_dynamic_data_(dyn_type, data);

    // TODO fast this does not make much sense as dynamictypes::print does not allow to choose target
    // change in dyn types to be able to print it in view
    view_.show("---");
    std::stringstream ss;
    if (fastdds::dds::RETCODE_OK !=
            fastdds::dds::json_serialize(dyn_data, fastdds::dds::DynamicDataJsonFormat::EPROSIMA, ss))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_CONTROLLER,
                "Not able to serialize data of topic " << topic.topic_name() << " into JSON format.");
        return;
    }

    // Reformat: arrays single-line
    const std::string pretty = format_json_arrays_inline(ss.str(), /*indent_step*/ 4);
    std::cout << pretty << std::endl;
    view_.show("---\n");
}

void Controller::data_stream_callback_verbose_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
    // Block entrance so prints does not collapse
    std::lock_guard<std::mutex> _(view_mutex_);

    // get the source guid
    std::ostringstream guid_ss;
    std::string partitions = "";
    guid_ss << data.source_guid;
    const auto partition_it = topic.partition_name.find(guid_ss.str());
    if (partition_it != topic.partition_name.end())
    {
        // add the partition set
        partitions = partition_it->second;
    }

    // Prepare info data
    participants::DdsDataData data_info{
        {topic.m_topic_name, topic.type_name},
        data.source_guid,
        partitions,
        data.source_timestamp
    };

    // Write yaml with info
    Yaml yml;
    ddspipe::yaml::set(yml, data_info);
    view_.show(yml);

    // Get deserializad data
    auto dyn_data = get_dynamic_data_(dyn_type, data);

    // Print data
    view_.show("data:\n---");
    std::stringstream ss;
    if (fastdds::dds::RETCODE_OK !=
            fastdds::dds::json_serialize(dyn_data, fastdds::dds::DynamicDataJsonFormat::EPROSIMA, ss))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_CONTROLLER,
                "Not able to serialize data of topic " << topic.topic_name() << " into JSON format.");
        return;
    }

    // Reformat: arrays single-line
    const std::string pretty = format_json_arrays_inline(ss.str(), /*indent_step*/ 4);
    std::cout << pretty << std::endl;
    view_.show("---\n");
}

bool Controller::verbose_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "verbose")
        || (argument == "v")
        || (argument == "-v")
        || (argument == "--v")
        || (argument == "V"));
}

bool Controller::verbose_verbose_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "verbose2")
        || (argument == "vv")
        || (argument == "--vv")
        || (argument == "VV"));
}

bool Controller::idl_argument_(
        const std::string& argument) const noexcept
{
    return (argument == "idl");
}

bool Controller::keys_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "keys")
        || (argument == "key")
        || (argument == "k")
        || (argument == "K"));
}

bool Controller::all_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "all")
        || (argument == "a")
        || (argument == "-a")
        || (argument == "--a")
        || (argument == "A"));
}

void Controller::participants_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::participants(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::participants_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::participants(model, guid);
        },
        "participant"
        );
}

void Controller::writers_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::writers(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::writers_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::writers(model, guid);
        },
        "writer"
        );
}

void Controller::readers_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::readers(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::readers_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::readers(model, guid);
        },
        "reader"
        );
}

void Controller::topics_command_(
        const std::vector<std::string>& arguments) noexcept
{
    Yaml yml;

    // Handle 'topics' command without arguments
    if (arguments.size() == 1)
    {
        // All participants simple
        ddspipe::yaml::set(yml, participants::ModelParser::topics(
                    *model_, ddspipe::core::types::WildcardDdsFilterTopic()), true);
    }
    else if (arguments.size() == 2)
    {
        const std::string& arg_1 = arguments[1];

        if (verbose_argument_(arg_1))
        {
            // Handle 'topics verbose'
            ddspipe::yaml::set(yml, participants::ModelParser::topics(
                        *model_, ddspipe::core::types::WildcardDdsFilterTopic()), false);
        }
        else if (verbose_verbose_argument_(arg_1))
        {
            // Handle 'topics verbose2'
            ddspipe::yaml::set(yml, participants::ModelParser::topics_verbose(
                        *model_, ddspipe::core::types::WildcardDdsFilterTopic()));
        }
        else
        {
            // Handle 'topics <wildard topic name>'
            ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
            filter_topic.topic_name = arg_1;

            auto data = participants::ModelParser::topics(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network.");
                return;
            }

            ddspipe::yaml::set(yml, data, true);
        }
    }
    else if (arguments.size() == 3)
    {
        const std::string& arg_1 = arguments[1];

        ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
        filter_topic.topic_name = arg_1;

        const std::string& arg_2 = arguments[2];

        if (verbose_argument_(arg_2))
        {
            // Handle 'topics <name> verbose'
            auto data = participants::ModelParser::topics(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network.");
                return;
            }

            ddspipe::yaml::set(yml, data, false);
        }
        else if (verbose_verbose_argument_(arg_2))
        {
            // Handle 'topics <name> verbose2'
            auto data = participants::ModelParser::topics_verbose(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network.");
                return;
            }

            ddspipe::yaml::set_collection(yml, data);
        }
        else if (idl_argument_(arg_2))
        {
            // Handle 'topics <name> idl'
            auto data = participants::ModelParser::topics_type_idl(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network.");
                return;
            }

            std::cout << '\n' << data << std::endl;
        }
        else if (keys_argument_(arg_2))
        {
            // Handle 'topics <name> keys'
            auto data = participants:: ModelParser::topics_keys(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network or no type information available.");
                return;
            }

            ddspipe::yaml::set(yml, data, true);
        }

        else
        {
            view_.show_error(STR_ENTRY
                    << "<"
                    << arguments[2]
                    << "> is not a valid topic option. "
                    << "Valid options are \"v \", \"vv\" (verbosity modes), \"idl\" or \"keys\".");
            return;
        }
    }
    else if (arguments.size() == 4)
    {
        const std::string& arg_1 = arguments[1];

        ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
        filter_topic.topic_name = arg_1;

        const std::string& arg_2 = arguments[2];

        if (keys_argument_(arg_2))
        {
            // Handle 'topics <name> keys v'
            auto data = participants:: ModelParser::topics_keys(*model_, filter_topic);

            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> does not match any topic in the DDS network or no type information available.");
                return;
            }

            const std::string& arg_3 = arguments[3];
            if (verbose_argument_(arg_3))
            {
                ddspipe::yaml::set(yml, data, false);
            }
            else
            {
                view_.show_error(STR_ENTRY
                        << "Last argument <" << arg_3 << "> is not valid. "
                        << "Only \"v\" (verbosity mdode) is allowed after \"keys\".");
            }
        }
    }

    view_.show(yml);
}

void Controller::print_command_(
        const std::vector<std::string>& arguments) noexcept
{
    // Check the number of arguments is correct
    if (arguments.size() < 2)
    {
        view_.show_error(STR_ENTRY
                << "Command <"
                << arguments[0]
                << "> requires at least one argument.");
        return;
    }

    // Print all data
    if (all_argument_(arguments[1]))
    {
        bool activated = model_->activate_all(
            std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_verbose_(topic, dyn_type, data);
                }));

        if (!activated)
        {
            view_.show_error(STR_ENTRY
                    << "Error printing all topics.");
            return;
        }

    }

    // Print topic
    else
    {
        ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
        filter_topic.topic_name = arguments[1];

        std::set<eprosima::ddspipe::core::types::DdsTopic> topics =
                participants::ModelParser::get_topics(*model_, filter_topic);
        if (topics.empty())
        {
            view_.show_error(STR_ENTRY
                    << "<"
                    << filter_topic.topic_name.get_value()
                    << "> does not match any topic discovered.");
            return;
        }
        if (!(model_->is_any_topic_type_discovered(topics)))
        {
            view_.show_error(STR_ENTRY
                    << "No type information available and thus cannot print data.");
            return;
        }

        // Check if verbose is set
        bool verbose = false;
        if (arguments.size() >= 3)
        {
            verbose = verbose_argument_(arguments[2]);
        }

        std::shared_ptr<participants::DataStreamer::CallbackType> callback;

        if (verbose)
        {
            callback = std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_verbose_(topic, dyn_type, data);
                });
        }
        else
        {
            callback = std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_(topic, dyn_type, data);
                });
        }

        // Must activate data streamer with the required callback
        bool activated = model_->activate(
            filter_topic,
            callback);
    }

    // Wait for other command to stop printing topics
    input_.stdin_handler().set_ignore_input(true);
    input_.wait_something();
    input_.stdin_handler().set_ignore_input(false);
    model_->deactivate();

    // Small delay to allow stdout to flush and avoid prompt overlap
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Controller::version_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show(STR_ENTRY
            << "Fast DDS Spy "
            << FASTDDSSPY_PARTICIPANTS_VERSION_STRING
            << "\ncommit hash: "
            << FASTDDSSPY_PARTICIPANTS_COMMIT_HASH);
}

void Controller::help_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show(STR_ENTRY
            << "Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.\n"
            << "Each command shows data related with the network in Yaml format.\n"
            << "Commands available and the information they show:\n"
            << "\thelp                                      : this help.\n"
            << "\tversion                                   : tool version.\n"
            << "\tquit                                      : exit interactive CLI and close program.\n"
            << "\tparticipants                              : DomainParticipants discovered in the network.\n"
            <<
            "\tparticipants verbose                      : verbose information about DomainParticipants discovered in the network.\n"
            <<
                    "\tparticipants <Guid>                       : verbose information related with a specific DomainParticipant.\n"
            << "\twriters                                   : DataWriters discovered in the network.\n"
            <<
                    "\twriters verbose                           : verbose information about DataWriters discovered in the network.\n"
            << "\twriters <Guid>                            : verbose information related with a specific DataWriter.\n"
            << "\treader                                    : DataReaders discovered in the network.\n"
            <<
                    "\treader verbose                            : verbose information about DataReaders discovered in the network.\n"
            << "\treader <Guid>                             : verbose information related with a specific DataReader.\n"
            << "\ttopics                                    : Topics discovered in the network in compact format.\n"
            << "\ttopics v                                  : Topics discovered in the network.\n"
            <<
                    "\ttopics vv                                 : verbose information about Topics discovered in the network.\n"
            <<
            "\ttopics <name>                             : Topics discovered in the network filtered by name (wildcard allowed (*)).\n"
            <<
            "\ttopics <name> idl                         : Display the IDL type definition for topics matching <name> (wildcards allowed).\n"
            <<
            "\ttopics <name> keys                        : Display the keys for topics matching <name> (wildcards allowed).\n"
            <<
            "\ttopics <name> keys v                      : verbose information about keys discovered in the network.\n"
            << "\tfilters                                   : Display the active filters.\n"
            << "\tfilters clear                             : Clear all the filter lists.\n"
            << "\tfilters remove                            : Remove all the filter lists.\n"
            << "\tfilter clear <category>                   : Clear <category> filter list.\n"
            << "\tfilter remove <category>                  : Remove <category> filter list.\n"
            <<
                    "\tfilter set <category> <filter_str>        : Set <category> filter list with <filter_str> as first value.\n"
            << "\tfilter add <category> <filter_str>        : Add <filter_str> in <category> filter list.\n"
            << "\tfilter remove <category> <filter_str>     : Remove <filter_str> in <category> filter list.\n"
            <<
                    "\techo <name>                               : data of a specific Topic (Data Type must be discovered).\n"
            <<
            "\techo <wildcard_name>                      : data of Topics matching the wildcard name (and whose Data Type is discovered).\n"
            << "\techo <name> verbose                       : data with additional source info of a specific Topic.\n"
            <<
            "\techo <wildcard_name> verbose              : data with additional source info of Topics matching the topic name (wildcard allowed (*)).\n"
            <<
            "\techo all                                  : verbose data of all topics (only those whose Data Type is discovered).\n"
            << "\n"
            << "Notes and comments:\n"
            << "\tTo exit from data printing, press enter.\n"
            << "\tEach command is accessible by using its first letter (h/v/q/p/w/r/t/s/f).\n"
            << "\n"
            << "For more information about these commands and formats, please refer to the documentation:\n"
            << "https://fast-dds-spy.readthedocs.io/en/latest/\n"
            );
}

void Controller::error_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show_error(STR_ENTRY
            << "<" << arguments[0] << "> is not a known command. "
            << "Use <help> command to see valid commands and arguments.");
}

void Controller::filter_command_(
        const std::vector<std::string>& arguments) noexcept
{
    const auto& check_filter_contains_value = [&](std::string category, std::string value, bool& ret)
            {
                if(category == "partitions" &&
                    partition_filter_set_.find(value) != partition_filter_set_.end())
                {
                    ret = true;
                }
                else if(category == "topic" &&
                    topic_filter_dict_.find(value) != topic_filter_dict_.end())
                {
                    ret = true;
                }
                else
                {
                    ret = false;
                }
            };

    bool pass;
    std::string operation;
    std::string category;
    std::string filter_str;
    std::string topic_str;

    std::set<std::string> supported_filters = { "partitions", /*"keys",*/ "topic"};

    if (arguments.size() == 1) // print filters
    {
        if (arguments[0] == "filter")
        {
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> requires 3 or 4 arguments.");
            return;
        }

        // print the filters list
        //std::cout << "Filter lists (" << filter_dict.size() << ")\n";
        std::cout << "--------\n";
        std::cout << "Filters:\n";
        std::cout << "--------\n\n";
        std::cout << "  Topic:\n";
        for (const auto& topic_pair: topic_filter_dict_)
        {
            std::cout << "    " << topic_pair.first << ": \"" << topic_pair.second << "\"\n";
        }

        std::cout << "\n  Partitions:\n";

        for (const auto& partition: partition_filter_set_)
        {
            std::cout << "    - " << (partition == "" ? "\"\"" : partition) << "\n";
        }
    }
    else if (arguments.size() == 2) // clear filters
    {
        if (arguments[0] == "filter")
        {
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> requires 3 or 4 arguments.");
            return;
        }

        std::set<std::string> allowed_args = {"clear"};
        if (allowed_args.find(arguments[1]) == allowed_args.end())
        {
            view_.show_error(STR_ENTRY
                    << "To clear filters list do: \"filters clear\".");
            return;
        }

        // clear ther filters list
        partition_filter_set_.clear();
        topic_filter_dict_.clear();

        update_partitions();
        update_topics();
    }
    else if (arguments.size() == 3) // filter clear <category>
    {
        if (arguments[0] == "filters")
        {
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> requires 1 or 2 arguments.");
            return;
        }

        operation = arguments[1];
        category = arguments[2];
        // std::set<std::string> allowed_args = {"clear"};//, "remove"};
        // if (allowed_args.find(operation) == allowed_args.end())
        if (operation != "clear" || supported_filters.count(category) == 0)
        {
            // incorrect operation ("clear")
            // or incorrect category ("partitions", "topic")
            view_.show_error(STR_ENTRY
                    << "To clear a filter category do: \"filters clear <category>\".");
            return;
        }

        if(category == "partitions")
        {
            partition_filter_set_.clear();
            update_partitions();
        }
        else
        {
            topic_filter_dict_.clear();
            update_topics();
        }
    }
    else if (arguments.size() == 4)
    {
        if (arguments[0] == "filters")
        {
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> requires 1 or 2 arguments.");
            return;
        }

        operation = arguments[1]; // <add/remove>
        category = arguments[2]; // partitions
        filter_str = arguments[3]; // filter string


        std::set<std::string> allowed_args = {"add", "remove"};
        if (allowed_args.find(operation) == allowed_args.end() ||
            category != "partitions")
        {
            // Operation not allowed
            // or category not supported
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> with 4 arguments have the following format: "
                    << arguments[0] << "<add/remove> partitions <filter_str>.");
            return;
        }

        if (operation == "add")
        {
            // Add "filter_str" for partitions filter

            bool contains;
            check_filter_contains_value(category, filter_str, contains);
            if (contains)
            {
                view_.show_error(STR_ENTRY
                        << "Partitions filter already contains filter_str: " << filter_str
                        << ".");
                return;
            }

            partition_filter_set_.insert(filter_str);
        }
        else
        {
            bool contains;
            check_filter_contains_value(category, filter_str, contains);
            if (!contains)
            {
                view_.show_error(STR_ENTRY
                        << "Partitions filter do not contains filter_str: " << filter_str
                        << ".");
                return;
            }

            partition_filter_set_.erase(filter_str);
        }

        update_partitions();
    }
    else if (arguments.size() == 5)
    {
        if (arguments[0] == "filters")
        {
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> requires 1 or 2 arguments.");
            return;
        }

        operation = arguments[1];  // <set/remove>
        category = arguments[2];   // topic
        topic_str = arguments[3];  // topic name
        filter_str = arguments[4]; // filter string


        std::set<std::string> allowed_args = {"set", "remove"};
        if (allowed_args.find(operation) == allowed_args.end() ||
            category != "topic")
        {
            // Operation not allowed
            // or category not supported
            view_.show_error(STR_ENTRY
                    << "Command <"
                    << arguments[0]
                    << "> with 5 arguments have the following format: "
                    << arguments[0] << "<add/remove> topic <topic_name> <filter_str>.");
            return;
        }

        if (operation == "set")
        {
            // Set "filter_str" for topic filter

            topic_filter_dict_[topic_str] = filter_str;
        }
        else
        {
            bool contains;
            check_filter_contains_value(category, filter_str, contains);
            if (!contains)
            {
                view_.show_error(STR_ENTRY
                        << "Topic filter do not contains filter_str: " << filter_str
                        << ".");
                return;
            }

            topic_filter_dict_.erase(topic_str);
        }

        update_content_topicfilter(topic_str);
    }
    else
    {
        view_.show_error(STR_ENTRY
                << "Command <"
                << arguments[0]
                << "> requires less than 5 argument.");
        return;
    }
}

void Controller::update_content_topicfilter(const std::string& topic_name)
{
    auto topic_it = topic_filter_dict_.find(topic_name);

    // Get the filter from the dictionary
    // if is not in the data structure
    // apply an empty expression in the content_topicfilter
    std::string filter_str = "";
    if (topic_it != topic_filter_dict_.end())
    {
        filter_str = topic_it->second;
    }

    backend_.update_readers_track_content_filter(topic_name, filter_str);
}

void Controller::update_topics()
{
    for (endpoint_pair endpoint: model_->endpoint_database_)
    {
        update_content_topicfilter(endpoint.second.info.topic.topic_name());
    }
}

void Controller::update_partitions()
{
    backend_.update_readers_track_partitions(partition_filter_set_);
}


// TODO. danip
bool Controller::check_filter_partitions(
        const endpoint_pair endpoint)
{
    if (partition_filter_set_.empty())
    {
        return true;
    }

    int i, n;
    std::string curr_partition;

    // get the partition set of the current endpoint
    for (const auto& guid_partition_pair: endpoint.second.info.specific_partitions)
    {
        i = 0;
        n = guid_partition_pair.second.size();
        curr_partition = "";

        // iterate in the partition set
        while (i < n)
        {
            if (guid_partition_pair.second[i] == '|')
            {
                for (std::string filter_p: partition_filter_set_)
                {
                    if (utils::match_pattern(filter_p, curr_partition) ||
                            utils::match_pattern(curr_partition, filter_p))
                    {
                        // The current partition matches with a partition
                        //  from the filter, the endpoint is active
                        return true;
                    }
                }

                curr_partition = "";
            }
            else
            {
                curr_partition += guid_partition_pair.second[i];
            }

            i++;
        }

        // Empty or last partition
        for (std::string filter_p: partition_filter_set_)
        {
            if (utils::match_pattern(filter_p, curr_partition) ||
                    utils::match_pattern(curr_partition, filter_p))
            {
                // The current partition matches with a partition
                //  from the filter, the endpoint is active
                return true;
            }
        }
    }

    return false;
}

/*bool Controller::check_filter_keys(
        const endpoint_pair endpoint,
        bool keys_exists)
{

    const auto& split_once = [&](const std::string& s, char delim = ':')
            -> std::pair<std::string, std::string>
            {
                // Splits "name:value" → {"name","value"}

                const std::size_t pos = s.find(delim);
                if (pos == std::string::npos)
                {
                    return std::make_pair(s, std::string());
                }

                return std::make_pair(s.substr(0, pos), s.substr(pos + 1));
            };

    const auto& extract_instance_value = [&](const std::string& instance)
            -> std::string
            {
                // Extracts value from: {\"flight_number\":111} → "111"

                const std::size_t colon = instance.find(':');
                if (colon == std::string::npos)
                {
                    return std::string();
                }

                std::size_t start = colon + 1;
                std::size_t end = instance.find('}', start);

                if (end == std::string::npos)
                {
                    end = instance.size();
                }

                return instance.substr(start, end - start);
            };


    if (keys_exists)
    {
        // Get the topic keys data from the current endpoint
        auto topic_keys = participants::ModelParser::topics_keys_by_ddstopic(*model_, {endpoint.second.info.topic});
        std::string key_name, key_val;

        // Iterate through the list of topic key data
        for (const auto& topic_key_data : topic_keys)
        {
            // Iterate through the key filters
            for (const auto& key_filter_str : filter_dict["keys"].second)
            {
                // Split the key filter string into "name" and "value"
                const auto key_pair = split_once(key_filter_str, ':');
                const std::string& key_name = key_pair.first;
                const std::string& key_val  = key_pair.second;

                // Check if the current topic key data name matches the key filter
                bool has_key = false;
                for (const auto& topic_key : topic_key_data.key_fields)
                {
                    if (topic_key == key_name)
                    {
                        has_key = true;
                        break;
                    }
                }

                if (!has_key)
                {
                    // Do not match, continue with the next key filter
                    continue;
                }

                // Iterate through the instances to find a matching key value
                for (const auto& inst : topic_key_data.instances)
                {
                    if (extract_instance_value(inst) == key_val)
                    {
                        // Match found, the current endpoint passes the key filter
                        return true;
                    }
                }
            }
        }
    }

    // Return true if no partition filter exists
    //        false if exists but no match found
    return !keys_exists;
}*/

} /* namespace spy */
} /* namespace eprosima */
