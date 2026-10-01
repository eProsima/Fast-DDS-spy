###################################
eProsima Fast DDS Spy Documentation
###################################

.. image:: /rst/figures/eprosima_logo.svg
  :height: 100px
  :width: 100px
  :align: left
  :alt: eProsima
  :target: http://www.eprosima.com/

*eProsima Fast DDS Spy* is an interactive CLI tool for introspecting a DDS network in a human-readable format.
You can query the network for the connected DomainParticipants, their endpoints (DataWriters and DataReaders) and the topics they communicate in.
You can also see the user data sent through network topics in a schematic format at run time.

##################
Commercial support
##################

Looking for commercial support? Write us to info@eprosima.com

Find more about us at `eProsima's webpage <https://eprosima.com/>`_.

########
Overview
########

*eProsima Fast DDS Spy* is a tool that introspects ("sniffs") DDS packages in the network and maintains a local database that is accessible from an interactive CLI.
*Fast DDS Spy* responds to text commands from the user and prints the requested information in ``stdout``.
Its commands give information about the status of the network.
It can list the existing topics, Participants, DataReaders and DataWriters, and even read user data in real time in a human-readable format.

.. figure:: /rst/figures/overview.png
    :align: center

*eProsima Fast DDS Spy* is easy to configure and is installed with a default setup that discovers DDS topics, data types and entities automatically, without the need to specify the data types.
It does this through the DynamicTypes functionality of `eProsima Fast DDS <https://fast-dds.docs.eprosima.com>`_, the C++ implementation of the `DDS (Data Distribution Service) Specification <https://www.omg.org/spec/DDS/About-DDS/>`_ defined by the `Object Management Group (OMG) <https://www.omg.org/>`_.

#################################
Contributing to the documentation
#################################

*Fast DDS Spy Documentation* is an open source project, and all contributions, both in the form of
feedback and content generation, are welcome.
To contribute, please refer to the
`Contribution Guidelines <https://github.com/eProsima/all-docs/blob/master/CONTRIBUTING.md>`_ hosted in our GitHub
repository.

##############################
Structure of the documentation
##############################

This documentation is organized into the sections below.

* :ref:`Installation Manual <installation_sources_linux>`
* :ref:`User Manual <getting_started_usage_example>`
* :ref:`Release Notes <notes>`
