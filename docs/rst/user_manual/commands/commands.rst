.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_commands:

########
Commands
########

These are the commands that |spy| currently supports.
Every command in this section is available from the :ref:`user_manual_user_interface_interactive_app` and from the :ref:`user_manual_user_interface_one_shot`.
Most commands and arguments have shortcuts and alternative names,
so several words run the same command (e.g. `participant` = `participants` = `p`).

Entity commands
===============

These commands query the application database for the current status of the network and return the information in :term:`YAML` format.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/participant.rst
   /rst/user_manual/commands/writer.rst
   /rst/user_manual/commands/reader.rst
   /rst/user_manual/commands/topic.rst

Data commands
=============

These commands show the user data that the application receives in real time.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/data.rst

Filter commands
===============

This command lets the user filter the information that the application observes.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/filter.rst

Extra commands
==============

These are other commands available in the application.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/extra_commands.rst

Input format
============

Format of the arguments that the application commands accept.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/format.rst

Summary
=======

.. this table is here instead on a different file because include directive from sphinx works weird with aliases.

.. list-table::
    :header-rows: 1

    *   - Command
        - Description
        - Arguments
        - KeyWords

    *   - :ref:`user_manual_command_participant`
        - Show DomainParticipant info
        - ``_`` |br|
          ``verbose`` |br|
          ``<Guid>``
        - ``participant`` ``participants`` |br|
          ``p`` ``P``

    *   - :ref:`user_manual_command_writer`
        - Show DataWriter info
        - ``_`` |br|
          ``verbose`` |br|
          ``<Guid>``
        - ``writer`` ``writers`` |br|
          ``datawriter`` ``datawriters`` |br|
          ``publication`` ``publications`` |br|
          ``w`` ``W``

    *   - :ref:`user_manual_command_reader`
        - Show DataReader info
        - ``_`` |br|
          ``verbose`` |br|
          ``<Guid>``
        - ``reader`` ``readers`` |br|
          ``datareader`` ``datareaders`` |br|
          ``subscription`` ``subscriptions`` |br|
          ``r`` ``R``

    *   - :ref:`user_manual_command_topic`
        - Show Topic info
        - ``_`` |br|
          ``verbose`` |br|
          ``vv`` |br|
          ``<topic name>`` |br|
          ``<topic name> idl`` |br|
          ``<topic name> keys`` |br|
          ``<topic name> keys v``
        - ``topic`` ``topics`` |br|
          ``t`` ``T``

    *   - :ref:`user_manual_command_filter`
        - Filter related commands
        - ``set`` |br|
          ``add`` |br|
          ``remove`` |br|
          ``clear`` |br|
          ``partitions`` |br|
          ``topic`` |br|
          ``<filter_str>`` |br|
          ``<topic_name>`` |br|
        - ``filter`` ``filters`` |br|
          ``partitions`` |br|
          ``f`` ``F``

    *   - :ref:`user_manual_command_echo`
        - Show user data received in real time.
        - ``<topic name>`` |br|
          ``<topic name> verbose`` |br|
          ``all``
        - ``echo`` ``show`` ``print`` |br|
          ``s`` ``S``

    *   - :ref:`user_manual_commands_extra_help`
        - Show help.
        -
        - ``help`` ``man`` |br|
          ``h`` ``H``

    *   - :ref:`user_manual_commands_extra_version`
        - Show version information.
        -
        - ``version`` |br|
          ``v`` ``V``

    *   - :ref:`user_manual_commands_extra_quit`
        - Stop and close the application.
        -
        - ``quit`` ``exit`` |br|
          ``quit()`` ``exit()`` |br|
          ``q`` ``Q`` ``x``
