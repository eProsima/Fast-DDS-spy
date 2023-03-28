.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _user_manual_user_interface:

##############
User Interface
##############

|espy| is a :term:`CLI` user application executed from command line and configured through a :term:`YAML` configuration file.

.. contents::
    :local:
    :backlinks: none
    :depth: 1

Run application
===============

Run |espy| application by using command :code:`fastddsspy`.

Source Dependency Libraries
---------------------------

|espy| depends on some eProsima projects as |fastdds| or |ddspipe|.
In order to correctly execute the application, make sure that these dependencies are properly sourced.

.. code-block:: bash

    source <path-to-fastdds-installation>/install/setup.bash

.. note::

    If Fast DDS has been installed in the system, these libraries would be sourced by default.


.. _user_manual_user_interface_application_arguments:

Application Arguments
=====================

The |espy| application supports several input arguments:

.. list-table::
    :header-rows: 1

    *   - Command
        - Option
        - Long option
        - Value
        - Default Value

    *   - :ref:`user_manual_user_interface_help_argument`
        - ``-h``
        - ``--help``
        -
        -

    *   - :ref:`user_manual_user_interface_version_argument`
        - ``-v``
        - ``--version``
        -
        -

    *   - :ref:`user_manual_user_interface_configuration_file_argument`
        - ``-c``
        - ``--config-path``
        - Readable File Path
        - ``./FASTDDSSPY_CONFIGURATION.yaml``

.. _user_manual_user_interface_help_argument:

Help Argument
-------------

It shows the usage information of the application.

.. code-block:: console

    Usage: Fast DDS Spy
    Start an interactive CLI to introspect a DDS network.
    General options:

    Application help and information.
    -h --help           Print this help message.
    -v --version        Print version, branch and commit hash.

    Application parameters
    -c --config-path    Path to the Configuration File (yaml format) [Default: ./FASTDDSSPY_CONFIGURATION.yaml].

.. _user_manual_user_interface_version_argument:

Version Argument
----------------

It shows the current version of the |spy| and the hash of the last commit of the compiled code.

.. _user_manual_user_interface_configuration_file_argument:

Configuration File Argument
---------------------------

A |spy| supports *YAML* configuration file.
Please refer to :ref:`user_manual_configuration` for more information on how to build this configuration file.

This *YAML* configuration can passed as argument to |spy| when executed.
If no configuration file is provided as argument, |spy| will attempt to load a file named
``FASTDDSSPY_CONFIGURATION.yaml`` that must be in the same directory where the application is executed.
If no configuration file is found, |spy| will use :ref:`default configuration <user_manual_configuration_default>`.

Reload Topics
^^^^^^^^^^^^^

This configuration file allows to allow and block DDS :term:`Topics <Topic>`.
A modification in this file will modify the running application.

.. _user_manual_user_interface_interactive_app:

Interactive application
=======================

The standard way to use this application is by running the *interactive CLI*.
This is a user interface that repeatedly ask the user for a command, expecting a ``stdin`` command and arguments in order to retrieve the data querying the internal database.

.. code-block:: console

    Insert a command for Fast DDS Spy:
    >>

Check the :ref:`following section <user_manual_commands>` to see the available commands and their arguments,
or use :ref:`help <user_manual_commands_extra_help>` command to get this information in ``stdout``.

Close Application
-----------------

Use :ref:`user_manual_commands_extra_quit` to stop and close the application.

.. _user_manual_user_interface_one_shot:

One-shot application
====================

|espy| can be executed as a *one-shot* application.
This is, an application that connects to a DDS network, queries a specific command to the internal database generated from such DDS network, and retrieves the information by ``stdout``.
Thus it will not ask the user for commands, neither wait before closing.

In order to execute |spy| in *one-shot* mode add the command and arguments right after the last :ref:`tool argument <user_manual_user_interface_application_arguments>`.

This kind of application depends on the network size and speed in order to discover every entity before showing the information and close.
In order to configure the amount of time elapsed before querying for the information requested, use :ref:`user_manual_configuration_discovery_time`.

.. note::

    Running a |spy| instance will create DDS entities that will discover and connect everything in the same network.
    Thus, running *one-shot* |spy| applications very frequently could affect to network performance, as each instance will be created and destroyed in the process.

Example
-------

Let's assume we want to retrieve the information of every :term:`DomainParticipant` running in a current network.

Using the :ref:`user_manual_user_interface_interactive_app`, this will be the process and the output:

.. code-block:: console

    <path>$ fastddsspy

     ____|             |        __ \   __ \    ___|        ___|
    |     _` |   __|  __|      |   |  |   | \___ \      \___ \   __ \   |   |
    __|  (   | \__ \  |        |   |  |   |       |           |  |   |  |   |
    _|   \__,_| ____/ \__|     ____/  ____/  _____/      _____/   .__/  \__, |
                                                                _|     ____/


    Insert a command for Fast DDS Spy:
    >> participants
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.c1

    Insert a command for Fast DDS Spy:
    >> quit
    <path>$

However, using :ref:`user_manual_user_interface_one_shot` this will be expected result:

.. code-block:: console

    <path>$ fastddsspy participants
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.c1
    <path>$
