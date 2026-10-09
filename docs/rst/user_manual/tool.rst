.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _user_manual_user_interface:

##############
User Interface
##############

|espy| is a :term:`CLI` user application executed from the command line and configured through a :term:`YAML` configuration file.

Run application
===============

Run the |espy| application with the :code:`fastddsspy` command.

Source Dependency Libraries
---------------------------

|espy| depends on some eProsima projects such as |fastdds| or |ddspipe|.
To run the application correctly, make sure that these dependencies are sourced.

.. code-block:: bash

    source <path-to-fastdds-installation>/install/setup.bash

.. note::

    If Fast DDS has been installed in the system, these libraries are sourced by default.


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

    *   - :ref:`user_manual_user_interface_reload_time_argument`
        - ``-r``
        - ``--reload-time``
        - Period (in seconds) to reload |br|
          configuration file
        - 0

    *   - :ref:`user_manual_user_interface_domain_argument`
        -
        - ``--domain``
        - Domain to spy on
        - 0

    *   - :ref:`user_manual_user_interface_debug_argument`
        - ``-d``
        - ``--debug``
        - Debug mode
        - ``false``

    *   - :ref:`user_manual_user_interface_log_filter_argument`
        -
        - ``--log-filter``
        - Filter the logs displayed
        - ``FASTDDSSPY``

    *   - :ref:`user_manual_user_interface_log_verbosity_argument`
        -
        - ``--log-verbosity``
        - Maximum category of the |br|
          logs displayed
        - ``error``

.. _user_manual_user_interface_help_argument:

Help Argument
-------------

This argument shows the usage information of the application.

.. code-block:: console

    Usage: Fast DDS Spy
    Start an interactive CLI to introspect a DDS network.
    General options:

    Application help and information.
    -h --help           Print this help message.
    -v --version        Print version, branch and commit hash.

    Application parameters
    -c --config-path    Path to the Configuration File (yaml format) [Default: ./FASTDDSSPY_CONFIGURATION.yaml].
    -r --reload-time    Time period in seconds to reload configuration file. This is needed when FileWatcher functionality is not available (e.g. config file is a symbolic link). Value 0 does not reload file. [Default: 0].
       --domain         Set the domain (0-232) to spy on. [Default = 0].

    Debug parameters
    -d --debug          Set log verbosity to Info (Using this option with --log-filter and/or --log-verbosity will head to undefined behaviour).
       --log-filter     Set a Regex Filter to filter by category the info and warning log entries. [Default = "FASTDDSSPY"].
       --log-verbosity  Set a Log Verbosity Level higher or equal the one given. (Values accepted: "info","warning","error" no Case Sensitive) [Default = "error"].

.. _user_manual_user_interface_version_argument:

Version Argument
----------------

This argument shows the current version of the |spy| and the hash of the last commit of the compiled code.

.. _user_manual_user_interface_configuration_file_argument:

Configuration File Argument
---------------------------

|spy| supports a *YAML* configuration file.
Please refer to :ref:`user_manual_configuration` for more information on how to build this configuration file.

This *YAML* configuration can be passed as an argument to |spy| when it is executed.
If no configuration file is provided as argument, |spy| tries to load a file named
``FASTDDSSPY_CONFIGURATION.yaml`` from the directory where the application is executed.
If no configuration file is found, |spy| uses the :ref:`default configuration <user_manual_configuration_default>`.

Reload Topics
^^^^^^^^^^^^^

This configuration file can be used to allow and block DDS :term:`Topics <Topic>`.
Changes made to this file are applied to the running application.

.. _user_manual_user_interface_reload_time_argument:

Reload Time Argument
--------------------

This argument sets the time period in seconds to reload the configuration file.

.. _user_manual_user_interface_domain_argument:

Domain Argument
---------------

This argument sets the domain id of the |spy|.

.. warning::

    If set, it overrides the domain id set in the configuration file.

.. _user_manual_user_interface_debug_argument:

Debug Argument
--------------

This argument sets the log verbosity to Info.

.. warning::

    Using this option with :ref:`log filter <user_manual_user_interface_log_filter_argument>` and/or :ref:`log verbosity <user_manual_user_interface_log_verbosity_argument>` will lead to undefined behavior.

.. _user_manual_user_interface_log_filter_argument:

Log Filter Argument
-------------------

Configure the |spy| to print the logs that match the given filter.
By default the filter is set to ``FASTDDSSPY`` for ``warning`` and ``info`` logs.

.. _user_manual_user_interface_log_verbosity_argument:

Log Verbosity Argument
----------------------

Configure the |spy| to print the logs up to a certain verbosity level.
The verbosity levels are (from most to least restrictive): ``error``, ``warning``, and ``info``.

.. _user_manual_user_interface_interactive_app:

Interactive application
=======================

The standard way to use this application is the *interactive CLI*.
This user interface repeatedly asks the user for a command and its arguments on ``stdin``, and retrieves the data by querying the internal database.

.. code-block:: console

    Insert a command for Fast DDS Spy:
    >>

Check the :ref:`following section <user_manual_commands>` to see the available commands and their arguments,
or use the :ref:`help <user_manual_commands_extra_help>` command to print this information to ``stdout``.

Close Application
-----------------

Write ``exit``, ``quit``, or ``q`` to exit the application.

.. _user_manual_user_interface_one_shot:

One-shot application
====================

|espy| can also be executed as a *one-shot* application.
In this mode it connects to a DDS network, runs a single command against the internal database built from that network, and prints the result to ``stdout``.
It does not ask the user for commands, nor wait before closing.

To execute |spy| in *one-shot* mode, add the command and its arguments right after the last :ref:`tool argument <user_manual_user_interface_application_arguments>`.

Whether this mode discovers every entity before it shows the information and closes depends on the size and speed of the network.
To configure how long it waits before querying the requested information, use :ref:`user_manual_configuration_discovery_time`.

.. note::

    Running a |spy| instance creates DDS entities that discover and connect to everything in the same network.
    Running *one-shot* |spy| applications very frequently could therefore affect network performance, since an instance is created and destroyed every time.

Example
-------

The following example retrieves the information of every :term:`DomainParticipant` running in the network.

With the :ref:`user_manual_user_interface_interactive_app`, the process and the output are:

.. code-block:: console

    $ fastddsspy

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
    $

However, with the :ref:`user_manual_user_interface_one_shot`, the expected result is:

.. code-block:: console

    $ fastddsspy participants
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.c1
    $
