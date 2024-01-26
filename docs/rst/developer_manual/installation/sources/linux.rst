.. include:: ../../../exports/alias.include
.. include:: ../../../exports/roles.include

.. _developer_manual_installation_sources_linux:

###############################
Linux installation from sources
###############################

The instructions for installing the |espy| from sources and its required dependencies are provided in this page.
It is organized as follows:

.. contents::
    :local:
    :backlinks: none
    :depth: 2


Dependencies installation
=========================

|spy| depends on *eProsima Fast DDS* library and certain Debian packages.
This section describes the instructions for installing |spy| dependencies and requirements in a Linux environment from sources.
The following packages will be installed:

- ``foonathan_memory_vendor``, an STL compatible C++ memory allocation library.
- ``fastcdr``, a C++ library that serializes according to the standard CDR serialization mechanism.
- ``fastrtps``, the core library of eProsima Fast DDS library.
- ``cmake_utils``, an eProsima utils library for CMake.
- ``cpp_utils``, an eProsima utils library for C++.
- ``ddspipe``, an eProsima internal library that enables the communication of DDS interfaces.

First of all, the :ref:`Requirements <requirements>` and :ref:`Dependencies <dependencies>` detailed below need to be met.
Afterwards, the user can choose whether to follow either the :ref:`colcon <colcon_installation>` or the
:ref:`CMake <linux_cmake_installation>` installation instructions.

.. _requirements:

Requirements
------------

The installation of |espy| in a Linux environment from sources requires the following tools to be installed in the system:

* :ref:`cmake_gcc_pip_wget_git_sl`
* :ref:`colcon_install` [optional]
* :ref:`gtest_sl` [for test only]


.. _cmake_gcc_pip_wget_git_sl:

CMake, g++, pip, wget and git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These packages provide the tools required to install |espy| and its dependencies from command line.
Install CMake_, `g++ <https://gcc.gnu.org/>`_, pip_, wget_ and git_ using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

    sudo apt install cmake g++ pip wget git


.. _colcon_install:

Colcon
^^^^^^

colcon_ is a command line tool based on CMake_ aimed at building sets of software packages.
Install the ROS 2 development tools (colcon_ and vcstool_) by executing the following command:

.. code-block:: bash

    pip3 install -U colcon-common-extensions vcstool

.. note::

    If this fails due to an Environment Error, add the :code:`--user` flag to the :code:`pip3` installation command.


.. _gtest_sl:

Gtest
^^^^^

Gtest_ is a unit testing library for C++.
By default, |espy| does not compile tests.
It is possible to activate them with the opportune `CMake options <https://colcon.readthedocs.io/en/released/reference/verb/build.html#cmake-options>`_ when calling colcon_ or CMake_.
For more details, please refer to the :ref:`cmake_options` section.
For a detailed description of the Gtest_ installation process, please refer to the `Gtest Installation Guide <https://github.com/google/googletest>`_.

It is also possible to clone the Gtest_ Github repository into the |espy| workspace and compile it with colcon_ as a dependency package.
Use the following command to download the code:

.. code-block:: bash

    git clone --branch release-1.11.0 https://github.com/google/googletest src/googletest-distribution


.. _dependencies:

Dependencies
------------

|espy| has the following dependencies, when installed from sources in a Linux environment:

* :ref:`asiotinyxml2_sl`
* :ref:`openssl_sl`
* :ref:`yaml_cpp`
* :ref:`eprosima_dependencies`

.. _asiotinyxml2_sl:

Asio and TinyXML2 libraries
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Asio is a cross-platform C++ library for network and low-level I/O programming, which provides a consistent asynchronous model.
TinyXML2 is a simple, small and efficient C++ XML parser.
Install these libraries using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

    sudo apt install libasio-dev libtinyxml2-dev

.. _openssl_sl:

OpenSSL
^^^^^^^

OpenSSL is a robust toolkit for the TLS and SSL protocols and a general-purpose cryptography library.
Install OpenSSL_ using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

   sudo apt install libssl-dev

.. _yaml_cpp:

yaml-cpp
^^^^^^^^

yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec, and is used by *Fast DDS Spy* application to parse the provided configuration files.
Install yaml-cpp using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

   sudo apt install libyaml-cpp-dev

.. _eprosima_dependencies:

eProsima dependencies
^^^^^^^^^^^^^^^^^^^^^

If it already exists in the system an installation of *Fast DDS* and *DDS Pipe* libraries, just source this libraries when building |espy| by running the following commands.
In other case, just skip this step.

.. code-block:: bash

    source <fastdds-installation-path>/install/setup.bash
    source <ddspipe-installation-path>/install/setup.bash


.. _colcon_installation:

Colcon installation
===================

#.  Create a :code:`fastdds-spy` directory and download the :code:`.repos` file that will be used to install |espy| and its dependencies:

    .. code-block:: bash

        mkdir -p ~/fastdds-spy/src
        cd ~/fastdds-spy
        wget https://raw.githubusercontent.com/eProsima/Fast-DDS-Spy/v0.3.0/fastddsspy.repos
        vcs import src < fastddsspy.repos

    .. note::

        In case there is already a *Fast DDS* installation in the system it is not required to download and build every dependency in the :code:`.repos` file.
        It is just needed to download and build the |espy| project having sourced its dependencies.
        Refer to section :ref:`eprosima_dependencies` in order to check how to source *Fast DDS* library.

#.  Build the packages:

    .. code-block:: bash

        colcon build --packages-up-to-regex fastddsspy

.. note::

    Being based on CMake_, it is possible to pass the CMake configuration options to the :code:`colcon build` command.
    For more information on the specific syntax, please refer to the `CMake specific arguments <https://colcon.readthedocs.io/en/released/reference/verb/build.html#cmake-specific-arguments>`_ page of the colcon_ manual.


CMake installation
==================

There exist the possibility to install |spy| by CMake, and could be see in following :ref:`section <linux_cmake_installation>`.
However :ref:`colcon_installation` is recommended.


.. _run_app_colcon_sl:

Run an application
==================

To run the |spy| tool, source the installation path and execute the executable file that has been installed in :code:`<install-path>/fastddsspy_tool/bin/fastddsspy`:

.. code-block:: bash

    # If built has been done using colcon, all projects could be sourced as follows
    source install/setup.bash
    fastddsspy

Be sure that this executable has execution permissions.


Run tests
=========




.. External links

.. _colcon: https://colcon.readthedocs.io/en/released/
.. _CMake: https://cmake.org
.. _pip: https://pypi.org/project/pip/
.. _wget: https://www.gnu.org/software/wget/
.. _git: https://git-scm.com/
.. _OpenSSL: https://www.openssl.org/
.. _Gtest: https://github.com/google/googletest
.. _vcstool: https://pypi.org/project/vcstool/
