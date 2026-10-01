.. include:: ../exports/alias.include

.. _getting_started_usage_example:

################
Example of usage
################

This example is a hands-on tutorial that introduces some of the main concepts and features of
|espy|.

Prerequisites
=============

|espy| must be installed beforehand using one of the following installation methods:

* :ref:`installation_sources_windows`
* :ref:`installation_sources_linux`
* :ref:`docker`

`ShapesDemo <https://www.eprosima.com/index.php/products-all/eprosima-shapes-demo>`_ is also required, to publish and subscribe to shapes of different colors and sizes.
Install it by following any of the methods described in the given links:

* `Windows installation from binaries <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/windows_binaries.html>`_
* `Linux installation from sources <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/linux_sources.html>`_
* `Docker Image <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/docker_image.html>`_

Start ShapesDemo
================

Launch a ShapesDemo instance and start publishing on the ``Square`` topic with default settings.

.. figure:: /rst/figures/example_usage/shapesdemo_publisher.png
    :align: center
    :scale: 75 %

Spy configuration
=================

|espy| runs with default configuration settings.

These default configuration parameters can be changed with a YAML configuration file.

.. note::
    Please refer to :ref:`user_manual_configuration` for more information on how to configure |espy|.

Spy execution
=============

Source the following file to set up the |espy| environment:

.. code-block:: bash

    source install/setup.bash

Launch an |espy| instance by executing the following command:

.. code-block:: bash

    fastddsspy

Try out all the commands DDS Spy has to offer:

* ``participants``

.. code-block:: output

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.c1
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.1.c1
    - ...

* ``datawriters``

.. code-block:: output

    - guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.2
      participant: Fast DDS ShapesDemo Participant
      topic: Triangle [ShapeType]
    - guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
      participant: Fast DDS ShapesDemo Participant
      topic: Circle [ShapeType]
    - ...

* ``topics``

.. code-block:: output

    - topic: Circle (ShapeType) (1|2) [9.000000 Hz]
    - topic: Square (ShapeType) (1|0) [12.412500 Hz]
    - topic: Triangle (ShapeType) (0|1) [0.000000 Hz]
    - ...

* ``topics Circle vv``

.. code-block:: output

    - name: Circle
      type: ShapeType
      datawriters:
        - 01.0f.93.86.fb.9a.7f.92.00.00.02.00|0.0.3.2 ["A|B"]
      datareaders:
        - 01.0f.93.86.fb.9a.7f.92.00.00.02.00|0.0.1.7 ["A"]
        - 01.0f.93.86.fb.9a.7f.92.00.00.02.00|0.0.2.7 ["B"]
      rate: 11.2986 Hz
      dynamic_type_discovered: false


* ``topics Circle idl``

.. code-block:: output

    @extensibility(APPENDABLE)
    struct ShapeType
    {
        @key string color;
        long x;
        long y;
        long shapesize;
    };

* ``topics Circle keys``

.. code-block:: output

    - topic: Circle
      keys:
        - color
      instance_count: 2

* ``topics Circle keys v``

.. code-block:: output

    - topic: Circle
      keys:
        - color
      instances:
        - color: RED
        - color: BLUE
      instance_count: 2

* ``help``

.. code-block:: output

    Insert a command for Fast DDS Spy:
    >> help
    Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.
    Each command shows data related with the network in Yaml format.
    Commands available and the information they show:
        help                                        : this help.
        version                                     : tool version.
        quit                                        : exit interactive CLI and close program.
        participants                                : DomainParticipants discovered in the network.
        participants verbose                        : verbose information about DomainParticipants discovered in the network.
        participants <Guid>                         : verbose information related with a specific DomainParticipant.
        writers                                     : DataWriters discovered in the network.
        writers verbose                             : verbose information about DataWriters discovered in the network.
        writers <Guid>                              : verbose information related with a specific DataWriter.
        readers                                     : DataReaders discovered in the network.
        readers verbose                             : verbose information about DataReaders discovered in the network.
        readers <Guid>                              : verbose information related with a specific DataReader.
        topics                                      : Topics discovered in the network in compact format.
        topics v                                    : Topics discovered in the network.
        topics vv                                   : verbose information about Topics discovered in the network.
        topics <name>                               : Topics discovered in the network filtered by name (wildcard allowed (*)).
        topics <name> idl                           : Display the IDL type definition for topics matching <name> (wildcards allowed).
        topics <name> keys                          : Display the keys for topics matching <name> (wildcards allowed).
        topics <name> keys v                        : verbose information about keys discovered in the network.
        filters                                     : Display the active filters.
        filters clear                               : Clear all the filter lists.
        filters clear <category>                    : Clear <category> filter list.
        filters add partitions <filter_str>         : Add <filter_str> in partitions filter list.
        filters remove partitions <filter_str>      : Remove <filter_str> in partitions filter list.
        filters set topic <topic_name> <filter_str> : Set topic filter list with <filter_str> as first value.
        echo <name>                                 : data of a specific Topic (Data Type must be discovered).
        echo <wildcard_name>                        : data of Topics matching the wildcard name (and whose Data Type is discovered).
        echo <name> verbose                         : data with additional source info of a specific Topic.
        echo <wildcard_name> verbose                : data with additional source info of Topics matching the topic name (wildcard allowed (*)).
        echo all                                    : verbose data of all topics (only those whose Data Type is discovered).

    Notes and comments:
        To exit from data printing, press enter.
        Each command is accessible by using its first letter (h/v/q/p/w/r/t/s/f).

    For more information about these commands and formats, please refer to the documentation:
    https://fast-dds-spy.readthedocs.io/en/latest/

Stop |espy| by typing ``exit``.

Next Steps
==========

As a next step, you can apply a configuration file to adjust Fast DDS Spy to your monitoring and debugging needs.
The configuration can enable or disable the DDS communication transports used by Fast DDS Spy, set the DDS Domain to monitor, or define lists of allowed and blocked topics, among other settings.

Please refer to the :ref:`user_manual_configuration` section for all the settings available in Fast DDS Spy.
