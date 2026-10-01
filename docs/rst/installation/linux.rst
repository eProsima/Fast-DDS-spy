.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _installation_sources_linux:

###############################
Linux installation from sources
###############################

This page explains how to install |espy| and its required dependencies from sources.

Dependencies installation
=========================

|spy| depends on the *eProsima Fast DDS* library and some Debian packages.
This section describes how to install the |spy| dependencies and requirements in a Linux environment from sources.
The following packages will be installed:

- ``foonathan_memory_vendor``, an STL compatible C++ memory allocation library.
- ``fastcdr``, a C++ library that serializes according to the standard CDR serialization mechanism.
- ``fastdds``, the core library of eProsima Fast DDS.
- ``cmake_utils``, an eProsima utils library for CMake.
- ``cpp_utils``, an eProsima utils library for C++.
- ``ddspipe``, an eProsima internal library that enables the communication of DDS interfaces.

First, meet the :ref:`Requirements <requirements>` and :ref:`Dependencies <dependencies>` detailed below.
Then follow either the :ref:`colcon <colcon_installation>` or the
:ref:`CMake <linux_cmake_installation>` installation instructions.

.. _requirements:

Requirements
------------

Installing |espy| from sources in a Linux environment requires the following tools to be installed in the system:

* :ref:`cmake_gcc_pip_wget_git_sl`
* :ref:`colcon_install` [optional]
* :ref:`gtest_sl` [for test only]


.. _cmake_gcc_pip_wget_git_sl:

CMake, g++, pip, wget and git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These packages provide the tools required to install |espy| and its dependencies from the command line.
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
It is possible to activate them with the appropriate `CMake options <https://colcon.readthedocs.io/en/released/reference/verb/build.html#cmake-options>`_ when calling colcon_ or CMake_.
For more details, please refer to the :ref:`cmake_options` section.
For a detailed description of the Gtest_ installation process, please refer to the `Gtest Installation Guide <https://github.com/google/googletest>`_.

It is also possible to clone the Gtest_ GitHub repository into the |espy| workspace and compile it with colcon_ as a dependency package.
Use the following command to download the code:

.. code-block:: bash

    git clone --branch release-1.12.0 https://github.com/google/googletest src/googletest-distribution


.. _dependencies:

Dependencies
------------

When installed from sources in a Linux environment, |espy| has the following dependencies:

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

OpenSSL is a toolkit for the TLS and SSL protocols and a general-purpose cryptography library.
Install OpenSSL_ using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

   sudo apt install libssl-dev

.. _yaml_cpp:

yaml-cpp
^^^^^^^^

yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec, and the *Fast DDS Spy* application uses it to parse the provided configuration files.
Install yaml-cpp using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

   sudo apt install libyaml-cpp-dev

.. _eprosima_dependencies:

eProsima dependencies
^^^^^^^^^^^^^^^^^^^^^

If the *Fast DDS* and *DDS Pipe* libraries are already installed in the system, source these libraries when building |espy| by running the following commands.
Otherwise, skip this step.

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
        wget https://raw.githubusercontent.com/eProsima/Fast-DDS-Spy/main/fastddsspy.repos
        vcs import src < fastddsspy.repos

    .. note::

        If there is already a *Fast DDS* installation in the system, it is not required to download and build every dependency in the :code:`.repos` file.
        It is enough to download and build the |espy| project after sourcing its dependencies.
        Refer to section :ref:`eprosima_dependencies` to check how to source the *Fast DDS* library.

#.  Build the packages:

    .. code-block:: bash

        colcon build --packages-up-to-regex fastddsspy

.. note::

    Since colcon is based on CMake_, the CMake configuration options can be passed to the :code:`colcon build` command.
    For more information on the specific syntax, please refer to the `CMake specific arguments <https://colcon.readthedocs.io/en/released/reference/verb/build.html#cmake-specific-arguments>`_ page of the colcon_ manual.


CMake installation
==================

|spy| can also be installed with CMake, as described in the following :ref:`section <linux_cmake_installation>`.
However, the :ref:`colcon_installation` is recommended.


.. _run_app_colcon_sl:

Run an application
==================

To run the |spy| tool, source the installation path and run the executable installed in :code:`<install-path>/fastddsspy_tool/bin/fastddsspy`:

.. code-block:: bash

    # If built has been done using colcon, all projects could be sourced as follows
    source install/setup.bash
    fastddsspy

Make sure that this executable has execution permissions.


Run tests
=========

Tests are not built by default.
To build them, pass the :code:`BUILD_TESTS` CMake option to the :code:`colcon build` command:

.. code-block:: bash

    colcon build --packages-up-to-regex fastddsspy --cmake-args -DBUILD_TESTS=ON

Once built, run the test suite with:

.. code-block:: bash

    colcon test --packages-select-regex fastddsspy --event-handlers console_direct+

.. note::

    Some tests carry the :code:`xfail` label and are expected to fail.
    To exclude them, forward the label filter to :code:`ctest`:
    :code:`colcon test --packages-select-regex fastddsspy --ctest-args --label-exclude "xfail"`.


.. External links

.. _colcon: https://colcon.readthedocs.io/en/released/
.. _CMake: https://cmake.org
.. _pip: https://pypi.org/project/pip/
.. _wget: https://www.gnu.org/software/wget/
.. _git: https://git-scm.com/
.. _OpenSSL: https://www.openssl.org/
.. _Gtest: https://github.com/google/googletest
.. _vcstool: https://pypi.org/project/vcstool/
