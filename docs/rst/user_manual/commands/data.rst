.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_show:

####
Show
####

This command prints every User Data received in a human readable way.
The information shown with this command is real-time data that is being received by |spy|.
In order to stop the command, press enter and the CLI will finish showing the data received.

.. note::

   This is a real-time command that will not stop until enter is pressed.

Key-words
=========

These are the key-words recognize as this command:
``show`` ``print`` ``s`` ``S``.

Data Type discovered
====================

In order for a :term:`Topic` to be printable by the application, the |spy| requires to know the data type of such topic.
The information whether the topic data type is already discovered be the application can be shown by using :ref:`user_manual_command_topic` command.

Arguments
=========

**Show** command support different combination of arguments:

Topic name
----------

When a topic name is given, the information shown is the data received in real-time in the topic specified.
The output format is as follows: :ref:`user_manual_command_show_output_simple`.

Topic name + Verbose
--------------------

Giving a topic name and the the :ref:`verbose argument <user_manual_commands_input_verbose>` the output is the data received in real-time with additional meta-information as the topic name, the source timestamp, and the source :term:`DataWriter` :term:`Guid`.
Data is printing using :ref:`user_manual_command_show_output_verbose`.

All
---

This argument prints all topics which Data Type has been discovered.
Data is printing using :ref:`user_manual_command_show_output_verbose`.

Output Format
=============

.. note::

    The format of the data printed is not YAML.
    The correct YAML format will come in future releases.

The data information is retrieved in 2 formats depending on the verbose option.

.. _user_manual_command_show_output_simple:

Simple Data format
------------------

Only shows the data, by

.. code-block:: yaml

    ---
    <field name>: <value>
    ...
    ---

.. _user_manual_command_show_output_verbose:

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

Let's assume we have a DDS network where 2 ShapesDemo applications are running.

This would be the expected output for the command ``show Circle``:

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


This would be the expected output for the command ``show Circle verbose``:

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


This would be the expected output for the command ``datas 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1``:

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
