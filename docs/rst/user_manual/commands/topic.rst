.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_topic:

#####
Topic
#####

**Topic** is a command that retrieves information of the :term:`Topics <Topic>` with at least one endpoint currently active in the network.

Key-words
=========

These are the key-words recognize as this command:
``topic`` ``topics`` ``t`` ``T``.

Arguments
=========

**Topic** command supports from 0 to 2 arguments:

*No argument*
-------------

When no arguments are given to this command, the information shown is a **list** with every topic with at least one endpoint currently active in the network.
The information shown is the topic name, data type name, number of writers and readers and the subscription rate measured in samples per second.
The output format is as follows: :ref:`user_manual_command_topic_output_simple`.

Verbose
-------

This argument (``v`` or ``vv``) queries for more complete information about each of the topics in the network.
The output is a **list** of data with :ref:`verbose information <user_manual_command_topic_output_verbose>`.
Check the :ref:`verbose <user_manual_commands_input_verbose>` section in order to know which key-words are available for this argument.

Topic name
----------

This argument requires a string with the topic name.
This command queries the database for a **single Topic** and retrieves its :ref:`verbose information <user_manual_command_topic_output_verbose>`.
This Guid must exist inside the DDS network.

.. note::

    If there are 2 topics with the same name and different Topic Data Type, only one of them could be visible.
    These is a circumstance that :term:`DDS` allows, but it is strongly suggested not to do.

Topic name with wildcards
-------------------------

When a topic name contains wildcards (*), this command retrieves :ref:`verbose information <user_manual_command_topic_output_verbose>` for all topics that match the given filter.
This allows users to query multiple topics in a single command, making it easier to gather related data quickly.

Output Format
=============

The topic information is retrieved in multiple formats depending on the verbosity option.

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

.. code-block:: yaml

    name: <topic name>
    type: <data type name>
    datawriters:
        - <Guid>
        - ...
    datareaders:
        - <Guid>
        - ...
    rate: <samples per second> Hz
    dynamic_type_discovered: <bool>

Example
=======

Let's assume we have a DDS network where 2 ShapesDemo applications are running.

This would be the expected output for the command ``topics``:

.. code-block::

    - name: Circle
      type: ShapeType
      datawriters: 2
      datareaders: 2
      rate: 13.0298 Hz
    - name: Square
      type: ShapeType
      datawriters: 3
      datareaders: 2
      rate: 26.6975 Hz

This would be the expected output for the command ``topics verbose``:

.. code-block::

    - name: Circle
      type: ShapeType
      datawriters:
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.3.2
      datareaders:
        - 01.0f.44.59.c9.65.78.e5.00.00.00.00|0.0.2.7
      rate: 13.0286 Hz
      dynamic_type_discovered: true
    - name: Square
      type: ShapeType
      datawriters:
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.1.2
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.2.2
      datareaders:
        - 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.2.7
        - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.4.7
      rate: 26.685 Hz
      dynamic_type_discovered: true


This would be the expected output for the command ``topics Square``:

.. code-block::

    name: Circle
    type: ShapeType
    datawriters:
      - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    datareaders:
      - 01.0f.44.59.c9.65.78.e5.00.00.00.00|0.0.2.7
    rate: 13.0418 Hz
    dynamic_type_discovered: true
