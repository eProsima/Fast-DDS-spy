.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_topic:

#####
Topic
#####

**Topic** is a command that retrieves information of the :term:`Topics <Topic>` with at least one endpoint currently active in the network.

Key-words
=========

These are the key-words recognized as this command:
``topic`` ``topics`` ``t`` ``T``.

Arguments
=========

The **Topic** command supports from 0 to 3 arguments:

*No argument*
-------------

When no arguments are given, this command shows a list of every topic with at least one endpoint currently active in the network.
For each topic, it shows the topic name, the data type name, the number of writers and readers, and the subscription rate in samples per second.
The output format is described in :ref:`user_manual_command_topic_output_simple`.

Verbose
-------

This argument (``v`` or ``vv``) queries for more complete information about each of the topics in the network.
The output is a list with the :ref:`verbose information <user_manual_command_topic_output_verbose>` of each topic.
Check the :ref:`verbose <user_manual_commands_input_verbose>` section to see which key-words are available for this argument.

Topic name
----------

This argument requires a string with the topic name.
The command queries the database for a single topic and retrieves its :ref:`verbose information <user_manual_command_topic_output_verbose>`.
The topic must exist in the DDS network.

.. note::

    If 2 topics have the same name and different Topic Data Types, only one of them may be visible.
    :term:`DDS` allows this, but it is strongly discouraged.

Topic name with wildcards
-------------------------

When a topic name contains wildcards (*), this command retrieves the :ref:`verbose information <user_manual_command_topic_output_verbose>` of all topics that match the given filter,
so several topics can be queried with a single command.

Topic type IDL definition
-------------------------

When the argument ``idl`` is appended after a topic name, this command displays the IDL type definition of that topic.

Topic keys
----------

When the argument ``keys`` is appended after a topic name, this command displays the key fields of the data type of that topic, along with the number of discovered instances.
An optional ``v`` argument gives verbose output, which also includes the key values of each discovered instance.

Output Format
=============

The topic information is shown in a different format depending on the verbosity option.

.. _user_manual_command_topic_output_simple:

Topics info
-----------

- topic: <topic name> (<topic type name>) (<n writers>|<n readers>) [<rate> Hz]

.. _user_manual_command_topic_output_verbose:

Topics info in verbose mode
---------------------------

.. code-block:: yaml

    name: <topic name>
    type: <data type name>
    datawriters: <number of datawriters currently active>
    datareaders: <number of datareaders currently active>
    rate: <samples per second> Hz

Topics info in high verbosity mode
----------------------------------

This mode shows more complete information about each of the topics in the network.
It adds the Guid of each endpoint on the topic and whether the type has been discovered.

.. code-block:: yaml

    name: <topic name>
    type: <data type name>
    datawriters:
        - <Guid> [<partitions>]
        - ...
    datareaders:
        - <Guid> [<partitions>]
        - ...
    rate: <samples per second> Hz
    dynamic_type_discovered: <bool>

Example
=======

Consider a DDS network where 2 ShapesDemo applications are running.

This is the expected output of the command ``topics``:

.. code-block::

    - topic: Circle (ShapeType) (1|1) [13.029800 Hz]
    - topic: Square (ShapeType) (2|2) [26.697500 Hz]

This is the expected output of the command ``topics v``:

.. code-block::

    - name: Circle
      type: ShapeType
      datawriters: 1
      datareaders: 1
      rate: 13.029800 Hz
    - name: Square
      type: ShapeType
      datawriters: 2
      datareaders: 2
      rate: 26.697500 Hz

This is the expected output of the command ``topics vv``:

.. code-block::

    - name: Circle
      type: ShapeType
      datawriters:
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.3.2 ["A"]
      datareaders:
        - 01.0f.44.59.c9.65.78.e5.00.00.00.00|0.0.2.7 ["A"]
      rate: 13.028600 Hz
      dynamic_type_discovered: true
    - name: Square
      type: ShapeType
      datawriters:
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.1.2 ["A"]
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.2.2 ["A|B"]
      datareaders:
        - 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.2.7 ["A"]
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.4.7 ["B"]
      rate: 26.685000 Hz
      dynamic_type_discovered: true


This is the expected output of the command ``topics Square``:

.. code-block::

    name: Square
    type: ShapeType
    datawriters:
      - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.1.2
      - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.2.2
    datareaders:
      - 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.2.7
      - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.4.7
    rate: 26.685000 Hz
    dynamic_type_discovered: true

This is the expected output of the command ``topics Square idl``:

.. code-block::

    @extensibility(APPENDABLE)
    struct ShapeType
    {
        @key string color;
        long x;
        long y;
        long shapesize;
    };

This is the expected output of the command ``topics Square keys``:

.. code-block::

    - topic: Square
      keys:
        - color
      instance_count: 2

This is the expected output of the command ``topics Square keys v``:

.. code-block::

    - topic: Square
      keys:
        - color
      instances:
        - color: RED
        - color: BLUE
      instance_count: 2
