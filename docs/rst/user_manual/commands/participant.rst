.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_participant:

####################
Participants Command
####################

**Participants** is a command that retrieves information of the :term:`DomainParticipants <DomainParticipant>` currently active in the network.

Arguments
=========

**Participants** command support 0 or 1 argument.

*No argument*
-------------

When no arguments are given to this command, the information shown is a **list** with every DomainParticipant currently active in the network, giving their Guid and names.
The output format is as follows: :ref:`user_manual_command_participant_output_simple`.

Verbose
-------

This argument queries for more complete information about each of the DomainParticipants in the network.
It adds the information about endpoints and the topics they communicate in of each of the Participants.
The output got is a **list** of data with :ref:`verbose information <user_manual_command_participant_output_verbose>`.
Check the :ref:`verbose <user_manual_commands_input_verbose>` section in order to know how to add this argument to the command.

Guid
----

This argument requires a string with :ref:`Guid format <user_manual_commands_input_guid>`.
This command queries the database for a **single DomainParticipant** and retrieves its :ref:`verbose information <user_manual_command_participant_output_verbose>`.
This Guid must exist inside the DDS network.

Output Format
=============

The participant information is retrieved in 2 formats depending on the verbose option.

.. _user_manual_command_participant_output_simple:

Simple Participant info
-----------------------

.. code-block:: yaml

    name: <participant name>
    guid: <guid>

.. _user_manual_command_participant_output_verbose:

Verbose Participant info
------------------------

.. code-block:: yaml

    name: <participant name>
    guid: <guid>
    datawriters:
        - <topic name> [<topic data type name>] (<number of datawriters>)
        - ...
    datareaders:
        - <topic name> [<topic data type name>] (<number of datareaders>)
        - ...

Example
=======

Let's assume we have a DDS network where 2 ShapesDemo applications are running.

This would be the expected output for the command ``participants``:

.. code-block::

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.79.47.8d.f0.00.00.00.00|0.0.1.c1

This would be the expected output for the command ``participants verbose``:

.. code-block::

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
      datawriters:
        - Square [ShapeType] (2)
        - Circle [ShapeType] (1)
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.79.47.8d.f0.00.00.00.00|0.0.1.c1
      datawriters:
        - Triangle [ShapeType] (1)
      datareaders:
        - Square [ShapeType] (1)

This would be the expected output for the command ``participants 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1``:

.. code-block::

    name: Fast DDS ShapesDemo Participant
    guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
    datawriters:
    - Square [ShapeType] (2)
    - Circle [ShapeType] (1)
