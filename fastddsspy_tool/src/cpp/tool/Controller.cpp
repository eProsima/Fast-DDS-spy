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
        refresh_database();
        run_command_(command);
    }
}

void Controller::one_shot_run(
        const std::vector<std::string>& args)
{
    utils::sleep_for (configuration_.one_shot_wait_time_ms);
    run_command_(input_.parse_as_command(args));
}

void Controller::refresh_database()
{
    // vector of inactive endpoints to disable it after
    // the filter is applied in all active endpoints
    std::vector<ddspipe::core::types::Guid> v_guid_disable;

    // check if there is a filter partition
    if (filter_dict.find("partitions") != filter_dict.end())
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
            while(i < n)
            {
                if (endpoint_partitions[i] == '|')
                {
                    // check if the current partition is in the filter
                    for (std::string filter_p: filter_dict["partitions"])
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
            for (std::string filter_p: filter_dict["partitions"])
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

        else
        {
            view_.show_error(STR_ENTRY
                    << "<"
                    << arguments[2]
                    << "> is not a valid topic option. "
                    << "Valid options are \"v \", \"vv\" (verbosity modes) or or \"idl\".");
            return;
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
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
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
            << "\tparticipants <Guid>                       : verbose information related with a specific DomainParticipant.\n"
            << "\twriters                                   : DataWriters discovered in the network.\n"
            << "\twriters verbose                           : verbose information about DataWriters discovered in the network.\n"
            << "\twriters <Guid>                            : verbose information related with a specific DataWriter.\n"
            << "\treader                                    : DataReaders discovered in the network.\n"
            << "\treader verbose                            : verbose information about DataReaders discovered in the network.\n"
            << "\treader <Guid>                             : verbose information related with a specific DataReader.\n"
            << "\ttopics                                    : Topics discovered in the network in compact format.\n"
            << "\ttopics v                                  : Topics discovered in the network.\n"
            << "\ttopics vv                                 : verbose information about Topics discovered in the network.\n"
            <<
            "\ttopics <name>                             : Topics discovered in the network filtered by name (wildcard allowed (*)).\n"
            <<
            "\ttopics <name> idl                         : Display the IDL type definition for topics matching <name> (wildcards allowed).\n"
            << "\tfilters                                   : Display the active filters.\n"
            << "\tfilters clear                             : Clear all the filter lists.\n"
            << "\tfilters remove                            : Remove all the filter lists.\n"
            << "\tfilter clear <category>                   : Clear <category> filter list.\n"
            << "\tfilter remove <category>                  : Remove <category> filter list.\n"
            << "\tfilter set <category> <filter_str>        : Set <category> filter list with <filter_str> as first value.\n"
            << "\tfilter add <category> <filter_str>        : Add <filter_str> in <category> filter list.\n"
            << "\tfilter remove <category> <filter_str>     : Remove <filter_str> in <category> filter list.\n"
            << "\techo <name>                               : data of a specific Topic (Data Type must be discovered).\n"
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
    const auto& check_filter_dict_contains_category = [&](std::string category, bool& ret)
    {
        if (filter_dict.find(category) == filter_dict.end())
        {
            view_.show_error(STR_ENTRY
                    << "Filter list do not contains category: "
                    << category
                    << ".");
            ret = false;
        }
        else
        {
            ret = true;
        }
    };

    bool pass;
    std::string operation;
    std::string category;
    std::string filter_str;

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
        std::cout << "Filter lists (" << filter_dict.size() << ")\n";
        for (const auto& category: filter_dict)
        {
            std::cout << "\n  " << category.first << " (" << category.second.size() << "):\n";
            for (std::string filter: category.second)
            {
                std::cout << "    - " << (filter == "" ? "\"\"" : filter) << "\n";
            }
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
        if (arguments[1] != "clear")
        {
            view_.show_error(STR_ENTRY
                << "To clear filters list do: \"filters clear\".");
            return;
        }

        // clear ther filters list
        filter_dict.clear();

        update_filter_partitions();
    }
    else if (arguments.size() == 3) // filter <clear/removes> <category>
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

        bool pass;
        check_filter_dict_contains_category(category, pass);
        if (!pass)
        {
            // filter_dict do not contains the category
            return;
        }

        if (operation == "clear")
        {
            filter_dict[category].clear();
        }
        else if (operation == "remove")
        {
            filter_dict.erase(category);
        }
        else
        {
            view_.show_error(STR_ENTRY
                << "To clear or remove a filter category do: \"filters <clear/remove> <category>\".");
            return;
        }

        if (category == "partitions")
        {
           update_filter_partitions();
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

        operation = arguments[1];
        category = arguments[2];
        filter_str = arguments[3];

        if (operation == "set")
        {
            // set filter category
            if (filter_dict.find(category) != filter_dict.end())
            {
                view_.show_error(STR_ENTRY
                        << "Filter list already contains category: "
                        << category
                        << ".");
                return;
            }

            filter_dict[category] = std::set<std::string>{filter_str};

            if (category == "partitions")
            {
                update_filter_partitions();
            }
        }
        else if (operation == "add")
        {
            // add filter category

            bool pass;
            check_filter_dict_contains_category(category, pass);
            if (!pass)
            {
                // filter_dict do not contains the category
                return;
            }

            // check if filter_str is not in the filter list of the category
            if (filter_dict[category].find(category) != filter_dict[category].end())
            {
                view_.show_error(STR_ENTRY
                        << "Filter list already contains filter_str: " << filter_str
                        << " in category: " << category
                        << ".");
                return;
            }

            filter_dict[category].insert(filter_str);

            if (category == "partitions")
            {
                update_filter_partitions();
            }
        }
        else if (operation == "remove")
        {
            // remove filter category

            bool pass;
            check_filter_dict_contains_category(category, pass);
            if (!pass)
            {
                // filter_dict do not contains the category
                return;
            }

            // check if filter_str is in the filter list of the category
            if (filter_dict[category].find(filter_str) == filter_dict[category].end())
            {
                view_.show_error(STR_ENTRY
                        << "Filter list do not contains filter_str: " << filter_str
                        << " in category: " << category
                        << ".");
                return;
            }

            filter_dict[category].erase(filter_str);

            if (category == "partitions")
            {
                update_filter_partitions();
            }
        }
        else
        {
            view_.show_error(STR_ENTRY
                << "Command <"
                << arguments[0]
                << "> with 4 arguments have the following format: "
                << arguments[0] << "<set/add/remove> <category> <filter_str>.");
            return;
        }
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

void Controller::update_filter_partitions()
{
    // new filter partition list
    // update the endpoints active variable

    std::string topic_name;
    std::set<std::string> topic_set;
    std::vector<ddspipe::core::types::Guid> v_guid_active, v_guid_disable;

    int i, n;
    std::string curr_partition;
    bool endpoint_active;

    bool partitions_exists = filter_dict.find("partitions") != filter_dict.end();

    for (const auto& endpoint: model_->endpoint_database_)
    {
        topic_name = endpoint.second.info.topic.m_topic_name;

        // get the partition set of the current endpoint
        for (const auto& guid_partition_pair: endpoint.second.info.specific_partitions)
        {
            i = 0;
            n = guid_partition_pair.second.size();
            curr_partition = "";
            endpoint_active = filter_dict["partitions"].empty();
            // iterate in the partition set
            while(i < n)
            {
                if (guid_partition_pair.second[i] == '|')
                {
                    for (std::string filter_p: filter_dict["partitions"])
                    {
                        if (utils::match_pattern(filter_p, curr_partition) ||
                            utils::match_pattern(curr_partition, filter_p))
                        {
                            // the current partition matches with a partition
                            // from the filter, the endpoint is active
                            endpoint_active = true;
                            break;
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

            // empty or last partition
            for (std::string filter_p: filter_dict["partitions"])
            {
                if (utils::match_pattern(filter_p, curr_partition) ||
                    utils::match_pattern(curr_partition, filter_p))
                {
                    // the current partition matches with a partition
                    // from the filter, the endpoint is active
                    endpoint_active = true;
                    break;
                }
            }
        }

        // store the information of the active/disable endpoins,
        // later used for changing the active variable of all endpoints.
        if (!endpoint_active)
        {
            v_guid_disable.push_back(endpoint.first);
        }
        else
        {
            v_guid_active.push_back(endpoint.first);
        }

        // update tracker if it has not being updated before
        if (topic_set.find(topic_name) == topic_set.end())
        {
            backend_.update_readers_track(topic_name, filter_dict["partitions"]);
            topic_set.insert(topic_name);
        }
    }

    // update the filter of partitions in the pipeline
    backend_.update_pipeline_filter(filter_dict["partitions"]);

    if (!partitions_exists)
    {
        // remove the partition list if it was not created before.
        filter_dict.erase("partitions");
    }

    // change the active variable of the dabase
    for (const auto& curr_guid: v_guid_disable)
    {
        auto endpoint_tmp = model_->endpoint_database_.find(curr_guid)->second;
        endpoint_tmp.info.active = false;
        model_->endpoint_database_.add_or_modify(curr_guid, endpoint_tmp);
    }
    for (const auto& curr_guid: v_guid_active)
    {
        auto endpoint_tmp = model_->endpoint_database_.find(curr_guid)->second;
        endpoint_tmp.info.active = true;
        model_->endpoint_database_.add_or_modify(curr_guid, endpoint_tmp);
    }
}

} /* namespace spy */
} /* namespace eprosima */
