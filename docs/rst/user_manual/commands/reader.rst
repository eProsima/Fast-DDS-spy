.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_reader:

#######
Readers
#######

**Readers** is a command that retrieves information of the :term:`DataReaders <DataReader>` currently active in the network.

Key-words
=========

These are the key-words recognized as this command:
``reader`` ``readers`` ``datareader`` ``datareaders`` ``subscription`` ``subscriptions`` ``r`` ``R``.

Arguments
=========

The **Readers** command supports 0 or 1 argument:

*No argument*
-------------

When no arguments are given, this command shows a list of every DataReader currently active in the network, with their :term:`Guid`,
the name of their :term:`DomainParticipants <DomainParticipant>`, and the topic name and topic data type name.
The output format is described in :ref:`user_manual_command_reader_output_simple`.

Verbose
-------

This argument queries for more complete information about each of the DataReaders in the network.
It adds information about the QoS.
The output is a list with the :ref:`verbose information <user_manual_command_reader_output_verbose>` of each DataReader.
Check the :ref:`verbose <user_manual_commands_input_verbose>` section to see which key-words are available for this argument.

Guid
----

This argument requires a string with :ref:`Guid format <user_manual_commands_input_guid>`.
The command queries the database for a single DataReader and retrieves its :ref:`verbose information <user_manual_command_reader_output_verbose>`.
The Guid must exist in the DDS network.

.. note::

    If you are using |espy| as a one-shot application, you need to put the GUID in quotes.

Output Format
=============

The reader information is shown in 2 formats depending on the verbose option.

.. _user_manual_command_reader_output_simple:

Simple Reader info
------------------

.. code-block:: yaml

    guid: <guid>
    name: <participant name>
    topic: <topic name> [<topic data type name>]

.. _user_manual_command_reader_output_verbose:

Verbose Reader info
-------------------

.. code-block:: yaml

    name: <reader name>
    guid: <guid>
    topic:
        - <topic name>
        - <topic data type name>
    qos:
        - durability: <volatile | transient-local>
        - reliability: <reliable | best-effort>

Example
=======

Consider a DDS network where 2 ShapesDemo applications are running.

This is the expected output of the command ``readers``:

.. code-block::

    - guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.2
      participant: Fast DDS ShapesDemo Participant
      topic: Triangle [ShapeType]
    - guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
      participant: Fast DDS ShapesDemo Participant
      topic: Circle [ShapeType]
    - ...

This is the expected output of the command ``readers verbose``:

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

This is the expected output of the command ``readers 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2``:

.. code-block::

    guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    participant: Fast DDS ShapesDemo Participant
    topic:
      name: Circle
      type: ShapeType
    qos:
      durability: transient-local
      reliability: reliable
