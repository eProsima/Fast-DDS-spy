.. include:: ../../exports/alias.include

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
    Please refer to :ref:`Configuration <usage_configuration>` for more information on how to configure a |espy|.

Spy execution
=============

Launching a |espy| instance is as easy as executing the following command:

.. code-block:: bash

    fastdds_spy

Try out all the commands DDS Spy has to offer:

.. figure:: /rst/figures/example_usage/spy_participants.png
    :align: center
    :scale: 75 %

.. figure:: /rst/figures/example_usage/spy_datawriter.png
    :align: center
    :scale: 75 %

.. figure:: /rst/figures/example_usage/spy_topics.png
    :align: center
    :scale: 75 %

.. figure:: /rst/figures/example_usage/spy_square.png
    :align: center
    :scale: 75 %

In order to know all the possible arguments supported by this tool, use the command:

.. code-block:: bash

    fastdds_spy --help

Stop |espy| typing ``exit``.

Next Steps
==========

.. note::
    This page is under maintenance and will be updated soon.
