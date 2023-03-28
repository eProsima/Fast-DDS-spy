.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_writer:

#######
Writers
#######

**Writers** is a command that retrieves information of the :term:`DataWriters <DataWriter>` currently active in the network.

Key-words
=========

These are the key-words recognize as this command:
``writer`` ``writers`` ``datawriter`` ``datawriters`` ``publication`` ``publications`` ``w`` ``W``.

Arguments
=========

**Writers** command support 0 or 1 argument:

*No argument*
-------------

When no arguments are given to this command, the information shown is a **list** with every DataWriter currently active in the network, giving their :term:<Guid>,
the name of their respective :term:`DomainParticipants <DomainParticipant>`, and the topic name and topic data type name.
The output format is as follows: :ref:`user_manual_command_writer_output_simple`.

Verbose
-------

This argument queries for more complete information about each of the DataWriters in the network.
It adds information about the QoS.
The output got is a **list** of DataWriters with :ref:`verbose information <user_manual_command_writer_output_verbose>`.
Check the :ref:`verbose <user_manual_commands_input_verbose>` section in order to know which key-words are available for this argument.

Guid
----

This argument requires a string with :ref:`Guid format <user_manual_commands_input_guid>`.
This command queries the database for a **single DataWriter** and retrieves its :ref:`verbose information <user_manual_command_writer_output_verbose>`.
This Guid must exist inside the DDS network.

Output Format
=============

The writer information is retrieved in 2 formats depending on the verbose option.

.. _user_manual_command_writer_output_simple:

Simple Writer info
------------------

.. code-block:: yaml

    guid: <guid>
    name: <participant name>
    topic: <topic name> [<topic data type name>]

.. _user_manual_command_writer_output_verbose:

Verbose Writer info
-------------------

.. code-block:: yaml

    name: <writer name>
    guid: <guid>
    topic:
        - <topic name>
        - <topic data type name>
    qos:
        - durability: <volatile | transient-local>
        - reliability: <reliable | best-effort>

Example
=======

Let's assume we have a DDS network where 2 ShapesDemo applications are running.

This would be the expected output for the command ``writers``:

.. code-block::

    - guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.2
      participant: Fast DDS ShapesDemo Participant
      topic: Triangle [ShapeType]
    - guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
      participant: Fast DDS ShapesDemo Participant
      topic: Circle [ShapeType]
    - ...

This would be the expected output for the command ``writers verbose``:

.. code-block::

    - guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.2
      participant: Fast DDS ShapesDemo Participant
      topic:
        name: Triangle
        type: ShapeType
      qos:
        durability: volatile
        reliability: best-effort
    - guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
      participant: Fast DDS ShapesDemo Participant
      topic:
        name: Circle
        type: ShapeType
      qos:
        durability: transient-local
        reliability: reliable
    - ...

This would be the expected output for the command ``writers 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2``:

.. code-block::

    guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    participant: Fast DDS ShapesDemo Participant
    topic:
      name: Circle
      type: ShapeType
    qos:
      durability: transient-local
      reliability: reliable
