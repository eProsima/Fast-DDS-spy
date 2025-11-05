.. include:: ../exports/alias.include

.. _getting_started_usage_example:

################
Example of usage
################

This example will serve as a hands-on tutorial, aimed at introducing some of the key concepts and features that
|espy| has to offer.

Prerequisites
=============

It is required to have |espy| previously installed using one of the following installation methods:

* :ref:`installation_manual_windows`
* :ref:`installation_manual_linux`
* :ref:`docker`

Additionally, `ShapesDemo <https://www.eprosima.com/index.php/products-all/eprosima-shapes-demo>`_ is required to publish and subscribe shapes of different colors and sizes.
Install it by following any of the methods described in the given links:

* `Windows installation from binaries <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/windows_binaries.html>`_
* `Linux installation from sources <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/linux_sources.html>`_
* `Docker Image <https://eprosima-shapes-demo.readthedocs.io/en/latest/installation/docker_image.html>`_

Start ShapesDemo
================

Let us launch a ShapesDemo instance and start publishing in topics ``Square`` with default settings.

.. figure:: /rst/figures/example_usage/shapesdemo_publisher.png
    :align: center
    :scale: 75 %

Spy configuration
======================

|espy| runs with default configuration settings.

Additionally, it is possible to change the default configuration parameters by means of a YAML configuration file.

.. note::
    Please refer to :ref:`user_manual_configuration` for more information on how to configure a |espy|.

Spy execution
=============

Source the following file to setup the |espy| environment:

.. code-block:: bash

    source install/setup.bash

Launch an |espy| instance as executing the following command:

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

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.21.58.14.d2.00.00.00.00|0.0.1.c1
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.1.c1
    - ...

* ``topics``

.. code-block:: output

    - topic: Circle (ShapeType) (1|2) [9.000000Hz]
    - topic: Square (ShapeType) (1|0) [12.412500Hz]
    - topic: Triangle (ShapeType) (0|1) [0.000000Hz]
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

* ``help``

.. code-block:: output

    Insert a command for Fast DDS Spy:
    >> help
    Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.
    Each command shows data related with the network in Yaml format.
    Commands available and the information they show:
        help                                      : this help.
        version                                   : tool version.
        quit                                      : exit interactive CLI and close program.
        participants                              : DomainParticipants discovered in the network.
        participants verbose                      : verbose information about DomainParticipants discovered in the network.
        participants <Guid>                       : verbose information related with a specific DomainParticipant.
        writers                                   : DataWriters discovered in the network.
        writers verbose                           : verbose information about DataWriters discovered in the network.
        writers <Guid>                            : verbose information related with a specific DataWriter.
        reader                                    : DataReaders discovered in the network.
        reader verbose                            : verbose information about DataReaders discovered in the network.
        reader <Guid>                             : verbose information related with a specific DataReader.
        topics                                    : Topics discovered in the network in compact format.
        topics v                                  : Topics discovered in the network.
        topics vv                                 : verbose information about Topics discovered in the network.
        topics <name>                             : Topics discovered in the network filtered by name (wildcard allowed (*)).
        topics <name> idl                         : Display the IDL type definition for topics matching <name> (wildcards allowed).
        filters                                   : Display the active filters.
        filters clear                             : Clear all the filter lists.
        filters remove                            : Remove all the filter lists.
        filter clear <category>                   : Clear <category> filter list.
        filter remove <category>                  : Remove <category> filter list.
        filter set <category> <filter_str>        : Set <category> filter list with <filter_str> as first value.
        filter add <category> <filter_str>        : Add <filter_str> in <category> filter list.
        filter remove <category> <filter_str>     : Remove <filter_str> in <category> filter list.
        echo <name>                               : data of a specific Topic (Data Type must be discovered).
        echo <wildcard_name>                      : data of Topics matching the wildcard name (and whose Data Type is discovered).
        echo <name> verbose                       : data with additional source info of a specific Topic.
        echo <wildcard_name> verbose              : data with additional source info of Topics matching the topic name (wildcard allowed (*)).
        echo all                                  : verbose data of all topics (only those whose Data Type is discovered).

    Notes and comments:
        To exit from data printing, press enter.
        Each command is accessible by using its first letter (h/v/q/p/w/r/t/s).

    For more information about these commands and formats, please refer to the documentation:
    https://fast-dds-spy.readthedocs.io/en/latest/

Stop |espy| typing ``exit``.

Next Steps
==========

Since the main steps for running Fast DDS Spy have already been explained, you can now continue by applying a configuration file to this tool to adjust it to your monitoring and debugging needs.
These configurations include settings to enable or disable the DDS communication transports used by Fast DDS Spy, set the DDS Domain to monitor, or define lists of allowed and blocked topics, among others.

Please refer to the :ref:`user_manual_configuration` section of this documentation to know more about all settings available for the Fast DDS Spy.
