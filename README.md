# Fast-DDS-spy

<a href="http://www.eprosima.com"><img src="https://encrypted-tbn3.gstatic.com/images?q=tbn:ANd9GcSd0PDlVz1U_7MgdTe0FRIWD0Jc9_YH-gGi0ZpLkr-qgCI6ZEoJZ5GBqQ" align="left" hspace="8" vspace="2" width="100" height="100" ></a>

[![License](https://img.shields.io/github/license/eProsima/Fast-DDS-spy.svg)](https://opensource.org/licenses/Apache-2.0)
[![Issues](https://img.shields.io/github/issues/eProsima/Fast-DDS-spy.svg)](https://github.com/eProsima/Fast-DDS-spy/issues)
[![Forks](https://img.shields.io/github/forks/eProsima/Fast-DDS-spy.svg)](https://github.com/eProsima/Fast-DDS-spy/network/members)
[![Stars](https://img.shields.io/github/stars/eProsima/Fast-DDS-spy.svg)](https://github.com/eProsima/Fast-DDS-spy/stargazers)
[![test](https://github.com/eProsima/Fast-DDS-spy/actions/workflows/test.yml/badge.svg)](https://github.com/eProsima/Fast-DDS-spy/actions/workflows/test.yml)

*eProsima Fast DDS Spy*...


## Documentation

You can access the documentation online, which is hosted on [Read the Docs](https://fast-dds-spy.readthedocs.io).

* [Introduction](https://fast-dds-spy.readthedocs.io/en/latest/rst/formalia/titlepage.html)
* [Getting Started](https://fast-dds-spy.readthedocs.io/en/latest/rst/getting_started/project_overview.html)
* [User Manual](https://fast-dds-spy.readthedocs.io/en/latest/rst/user_manual/user_interface.html)
* [Examples](https://fast-dds-spy.readthedocs.io/en/latest/rst/examples/echo_example.html)
* [Use Cases](https://fast-dds-spy.readthedocs.io/en/latest/rst/use_cases/ros_cloud.html)
* [Developer Manual](https://fast-dds-spy.readthedocs.io/en/latest/rst/developer_manual/installation/sources/linux.html)
* [Release Notes](https://fast-dds-spy.readthedocs.io/en/latest/rst/notes/notes.html)


## Installation Guide

The instructions for installing the *Fast DDS Spy* application from sources and its required dependencies on a Linux
environment are provided below. These installation instructions are a summarized version of the complete
[installation guide](https://fast-dds-spy.readthedocs.io/en/latest/rst/developer_manual/installation/sources/linux.html) available online. Instructions for installing *Fast DDS Spy* on a **Windows** platform can be found
[here](https://fast-dds-spy.readthedocs.io/en/latest/rst/developer_manual/installation/sources/windows.html).

### Requirements

*eProsima Fast DDS Spy* requires the following tools to be installed in the system:
* [CMake](https://cmake.org/), [g++](https://gcc.gnu.org/), [pip](https://pypi.org/project/pip/), [wget](https://www.gnu.org/software/wget/) and [git](https://git-scm.com/)
* [Colcon](https://colcon.readthedocs.io/en/released/) [optional, not required for CMake-only installation]

```bash
# apt packages
sudo apt install -y \
    cmake \
    g++ \
    pip \
    wget \
    git \
    libasio-dev \
    libtinyxml2-dev \
    libssl-dev \
    libyaml-cpp-dev

# python packages
pip3 install -U \
    colcon-common-extensions \
    vcstool
```

### Colcon installation

1. Create a `Fast-DDS-spy` directory and download the `.repos` file that will be used to install *Fast DDS Spy* and its dependencies:

    ```bash
    mkdir -p ~/Fast-DDS-spy/src
    cd ~/Fast-DDS-spy
    wget https://raw.githubusercontent.com/eProsima/Fast-DDS-spy/main/fastddsspy.repos
    vcs import src < fastddsspy.repos
    ```

2. Build the packages:

    ```bash
    colcon build --packages-up-to fastddsspy_tool
    ```

### Run an application

To run the *Fast DDS Spy* application, source the installation environment and execute the executable file that has been
installed in `<install-path>/fastddsspy_tool/bin/fastddsspy`:

```bash
# Source installation
source <install-path>/setup.bash

# Execute Fast DDS Spy
fastddsspy_tool
# Use <fastddsspy_tool --help> to see the tool manual
```

## Getting Help

If you need support you can reach us by mail at `support@eProsima.com` or by phone at `+34 91 804 34 48`.
