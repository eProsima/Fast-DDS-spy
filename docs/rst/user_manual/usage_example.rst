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

* ``topics Square``

.. code-block:: output

    name: Circle
    type: ShapeType
    datawriters:
      - 01.0f.44.59.da.57.de.ec.00.00.00.00|0.0.6.2
    datareaders:
      - 01.0f.44.59.c9.65.78.e5.00.00.00.00|0.0.2.7
    rate: 13.0418 Hz
    dynamic_type_discovered: true

* ``help``

.. code-block:: output

    Insert a command for Fast DDS Spy:
    >> help
    Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.
    Each command shows data related with the network in Yaml format.
    Commands available and the information they show:
        help                   : this help.
        version                : tool version.
        quit                   : exit interactive CLI and close program.
        participants           : DomainParticipants discovered in the network.
        participants verbose   : verbose information about DomainParticipants discovered in the network.
        participants <Guid>    : verbose information related with a specific DomainParticipant.
        writers                : DataWriters discovered in the network.
        writers verbose        : verbose information about DataWriters discovered in the network.
        writers <Guid>         : verbose information related with a specific DataWriter.
        reader                 : DataReaders discovered in the network.
        reader verbose         : verbose information about DataReaders discovered in the network.
        reader <Guid>          : verbose information related with a specific DataReader.
        topics                 : Topics discovered in the network.
        topics verbose         : verbose information about Topics discovered in the network.
        topics <name>          : verbose information related with a specific Topic.
        show <name>            : data of a specific Topic (Data Type must be discovered).
        show <name> verbose    : data with additional source info of a specific Topic.
        show all               : verbose data of all topics (only those whose Data Type is discovered).

    Notes and comments:
        To exit from data printing, press enter.
        Each command is accessible by using its first letter (h/v/q/p/w/r/t/s).

    For more information about these commands and formats, please refer to the documentation:
    https://fast-dds-spy.readthedocs.io/en/latest/

Stop |espy| typing ``exit``.

Next Steps
==========

.. TODO add

.. note::
    This page is under maintenance and will be updated soon.
