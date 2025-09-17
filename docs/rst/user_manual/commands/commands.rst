.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_commands:

########
Commands
########

These are the commands supported so far by |spy|.
Every command explained in this section is available from the :ref:`user_manual_user_interface_interactive_app` and from the :ref:`user_manual_user_interface_one_shot`.
Most of the commands and arguments available have shortcuts and other names related.
Thus, multiple words execute the same commands (e.g. `participant` = `participants` = `p`).

Entity Commands
===============

The information retrieved by these commands follows :term:`YAML` format and queries the application database about the network current status.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/participant.rst
   /rst/user_manual/commands/writer.rst
   /rst/user_manual/commands/reader.rst
   /rst/user_manual/commands/topic.rst

Data commands
=============

This commands show user data being received by the application in real time.

.. toctree::
   :maxdepth: 2

   /rst/user_manual/commands/data.rst

Filter commands
===============

This command enables the user to filter the information observed by the application.

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

Input format for the application command arguments.

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
          ``<topic name>``
        - ``topic`` ``topics`` |br|
          ``t`` ``T``

    *   - :ref:`user_manual_command_filter`
        - Filter related commands
        - ``set`` |br|
          ``add`` |br|
          ``remove`` |br|
          ``clear`` |br|
          ``<category>`` |br|
          ``<filter_str>`` |br|
        - ``filter`` ``filters`` |br|
          ``f`` ``F``

    *   - :ref:`user_manual_command_echo`
        - Show real-time receiving user data.
        - ``<topic name>`` |br|
          ``<topic name> verbose`` |br|
          ``all``
        - ``show`` ``print`` |br|
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
        - Stop and close application.
        -
        - ``quit`` ``exit`` |br|
          ``quit()`` ``exit()`` |br|
          ``q`` ``Q`` ``x``
