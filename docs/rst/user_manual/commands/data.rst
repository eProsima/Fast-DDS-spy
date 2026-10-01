.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_echo:

####
Echo
####

This command prints all the user data received, in a human-readable way.
The data is shown in real time, as the |spy| receives it.
To stop the command, press enter, and the CLI stops showing the received data.

.. note::

   This is a real-time command that does not stop until enter is pressed.

Key-words
=========

These are the key-words recognized as this command:
``echo`` ``show`` ``print`` ``s`` ``S``.

Data Type discovered
====================

For a :term:`Topic` to be printable by the application, the |spy| needs to know the data type of that topic.
Use the :ref:`user_manual_command_topic` command to check whether the application has already discovered the data type of a topic.

Arguments
=========

The **Echo** command supports different combinations of arguments:

Topic name
----------

When a topic name is given, the command shows the data received in real time on that topic.
The output format is described in :ref:`user_manual_command_echo_output_simple`.

Topic name with wildcards
-------------------------
When a topic name is provided with wildcards (*), the command displays real-time information for all topics whose names match the given filter.

For example, ``sensor_*`` prints data from all topics starting with ``sensor_``, such as ``sensor_temperature``, ``sensor_humidity``, etc.

Topic name + Verbose
--------------------

When a topic name and the :ref:`verbose argument <user_manual_commands_input_verbose>` are given, the output is the data received in real time with additional meta-information such as the topic name, the source timestamp, and the source :term:`DataWriter` :term:`Guid`.
Data is printed using :ref:`user_manual_command_echo_output_verbose`.

Topic name wildcard
-------------------

When a topic name is provided with wildcards (*) and the :ref:`verbose argument <user_manual_commands_input_verbose>`, the command displays detailed real-time information and meta-information for all topics that match the given filter.

All
---

This argument prints all topics whose Data Type has been discovered.
Data is printed using :ref:`user_manual_command_echo_output_verbose`.

Output Format
=============

.. note::

    The format of the data printed is not YAML.
    The correct YAML format will come in future releases.

The data is shown in 2 formats depending on the verbose option.

.. _user_manual_command_echo_output_simple:

Simple Data format
------------------

Only shows the data:

.. code-block:: yaml

    ---
    <field name>: <value>
    ...
    ---

.. _user_manual_command_echo_output_verbose:

Verbose Data format
-------------------

.. code-block:: yaml

    topic: <topic name> [<topic data type name>]
    data: <Guid>
    timestamp: <YYYY/MM/DD hh:mm:ss>
    data:
    ---
    <field name>: <value>
    ...
    ---

Example
=======

Consider a DDS network where 2 ShapesDemo applications are running.

This is the expected output of the command ``show Circle``:

.. code-block::

    ---
    color: GREEN
    x: 168
    y: 125
    shapesize: 30
    ---

    ---
    color: GREEN
    x: 173
    y: 121
    shapesize: 30
    ---

    ...


This is the expected output of the command ``show Circle verbose``:

.. code-block::

    topic: Circle [ShapeType]
    data: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    timestamp: 2023/03/27 09:35:23
    data:
    ---
    color: GREEN
    x: 72
    y: 125
    shapesize: 30
    ---

    topic: Circle [ShapeType]
    data: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    timestamp: 2023/03/27 09:35:24
    data:
    ---
    color: GREEN
    x: 66
    y: 120
    shapesize: 30
    ---

    ...


This is the expected output of the command ``datas 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1``:

.. code-block::

    topic: Square [ShapeType]
    data: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.2.2
    timestamp: 2023/03/27 09:35:25
    data:
    ---
    color: BLUE
    x: 158
    y: 70
    shapesize: 20
    ---

    topic: Triangle [ShapeType]
    data: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.2
    timestamp: 2023/03/27 09:35:26
    data:
    ---
    color: YELLOE
    x: 93
    y: 75
    shapesize: 25
    ---

    ...
