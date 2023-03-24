# eProsima Fast DDS Spy docker Image

This image is installed with a Fast DDS Spy that is able to run a `fastdds_spy` application
Fast DDS Spy and all dependencies are already installed in the image.
In order to run the image use the following command.

```bash
fastdds_spy
```

---

## Using this Docker image

### Instructions on how to run an already image

```sh
# Run docker image
docker run --rm -it --net=host --ipc=host fastddsspy:v0
```

### Instructions on how to build it

```sh
# Build docker image (from workspace where Dockerfile is)
docker build --rm -t fastddsspy:v0 -f Dockerfile .
```

---

## Fast DDS Spy Tool

This is a CLI tool with several arguments and configured by a `.yaml` file.
This tool introspects a DDS network and then retrieve the relational information from queries asked by commands.
The tool is able to list the participants, writers, readers and topics of the network, along with some information related.
It is also able to subscribe to topics that share their data types and print the user data in run time.

### How to retrieve Data Type to the tool

Fast DDS does not send the Data Type information by default, it must be configured to do so.
First of all, when generating the Types using Fast DDS Gen, the option `-typeobject` must be added in order to generate the needed code to fill the TypeObject data.

For native types (Data Types that does not rely in other Data Types) this is enough, as Fast DDS will send the TypeObject by default.
However, for more complex types, it is required to use `TypeInformation` mechanism.
In the Fast DDS `DomainParticipant` set the following QoS in order to send this information:

```cpp
DomainParticipantQos pqos;
pqos.wire_protocol().builtin.typelookup_config.use_server = true;
```

---

## Run Fast DDS Spy

The Fast DDS Spy is a CLI interactive tool that allows the user to select between several commands in
order to query about the information gathered in the DDS network.
There is also the possibility to just run the application with a specific command and close afterwards.

### Run tool with a specific configuration

There are some configurations already available in the container under directory `/fastddsspy/resources/`.
However the application could be run without a configuration and default configuration is used.

- `simple_configuration.yaml` Configuration with just basic configuration.
- `complete_configuration.yaml` Configuration with all the possible configurations available.


In order to execute the `Fast DDS Spy` use the following command:

```bash
fastddsspy --config-path /fastddsspy/resources/<configuration>.yaml
```

### Show tool manual

In order to know all the possible arguments supported by this tool, use the command:

```bash
fastddsspy --help
# or
fastddsspy -h
```

### Run one-shot application instead of interactive one

To not run the interactive CLI and just run a specific command, execute the tool with such command

```bash
# For example, to execute command <participants> with verbose information
fastddsspy participants verbose
```

---

## Connectivity issues

- `--net=host` allow the Fast DDS Spy to connect with external (different device) participants.
- `--ipc=host` allow the Fast DDS Spy to use Shared Memory Transport with the participants in the same host.

If local Participants (same host) are unable to connect with the Fast DDS Spy inside a Docker, it may be because they try to use Shared Memory, but the docker has no access to the same shared segment.
To avoid this, run the other participants as `root` or change the docker image user name to be the same as the external participants one.
Other option may be to not using Shared Memory by disabling it by Fast DDS configuration (by XML or QoS in code) or by CMake option when compiling `-DSHM_TRANSPORT_DEFAULT=ON`.

---

## Configuration

This first version does support the following configurations:

|                      | Description                                                 | Type           | Default   |
|----------------------|-------------------------------------------------------------|----------------|-----------|
| dds/allowlist        | List of topics that the tool will subscribe to              | List of topics | Empty     |
| dds/blocklist        | List of topics that the tool will **not** subscribe to      | List of topics | Empty     |
| dds/domain           | DDS Domain to discover and subscribe to topics allowed      | integer        | 0         |
| specs/threads        | Number of threads to read DDS data                          | int            | 12        |
| specs/discovery-time | Time before showing info when using one-shot command        | int (ms)       | 1000      |
| specs/max-depth      | Maximum history depth for DataReaders                       | int            | 5000      |

The topics in `allowlist` and `blocklist` are filled with elements with field `name` referring to the Topic name.
Optionally each element can have the element `type` referring to the Topic Data Type name.
Both name and allow wildcards (`*`).

### Use Custom Configurations

There are 2 ways to write a custom configuration:

1. Modify a `.yaml` file within the container instance.
2. Using shared volumes to share the `.yaml` configuration file.

---

## Commands

Commands available and the information they show are:

- `help`: Help regarding commands
- `version`: Tool version.
- `quit`: Exit interactive CLI and close program.
- `participants`: DomainParticipants discovered in the network.
- `participants verbose`: Verbose information about DomainParticipants discovered in the network
- `participants <Guid>`: Verbose information related with a specific DomainParticipant.
- `writers`: DataWriters discovered in the network.
- `writers`: Verbose information about DataWriters discovered in the network.
- `writers <Guid>`: Verbose information related with a specific DataWriter.
- `reader`: DataReaders discovered in the network.
- `reader verbose`: Verbose information about DataReaders discovered in the network.
- `reader <Guid>`: Verbose information related with a specific DataReader.
- `topics`: Topics discovered in the network.
- `topics verbose`: Verbose information about Topics discovered in the network.
- `topics <name>`: Verbose information related with a specific Topic.
- `show <name>`: Data of a specific Topic (Data Type must be discovered).
- `show <name> verbose`: Data with additional source info of a specific Topic.
- `show all`: Verbose data of all topics (only those whose Data Type is discovered)

To exit the `show` command, press `enter`.
Each command is accessible by using its first letter `(h/v/q/p/w/r/t/s)`.
