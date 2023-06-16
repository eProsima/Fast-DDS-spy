.. include:: ../exports/alias.include

.. _notes:

#####
Notes
#####

.. TODO uncomment when there are forthcoming notes
.. include:: forthcoming_version.rst

Version v0.1.0
==============

This is the first release of |espy|.

This release includes several **features** regarding the introspection of DDS data, configuration and user interaction.

This release includes the following **User Interface features**:

* :ref:`Application executable arguments <user_manual_user_interface_application_arguments>`.
* :ref:`Interactive application <user_manual_user_interface_interactive_app>`.
* :ref:`One-shot application <user_manual_user_interface_one_shot>`.


This release includes the following **Configuration features**:

* Support :term:`YAML` :ref:`configuration file <user_manual_configuration>`.
* Support for allow and block topic filters at execution time and in run-time.
* Support :ref:`DDS configurations <user_manual_configuration_dds>`.
* Support :ref:`Advanced configurations <user_manual_configuration_specs>`.


This release includes the following **Introspection features**:

* Get data of :term:`Domain Participants <DomainParticipant>`.
* Get data of :term:`DataReaders <DataReader>` and :term:`DataWriters <DataWriter>`.
* Get data of :term:`Topics <Topic>`.
* Print real-time DDS user data in human-readable format.


This release includes the following **Documentation features**:

* This same documentation.


This release includes the following **Installation features**:

* *Dockerfile* to build an image of the application ``Fast-DDS-Spy/docker``.
