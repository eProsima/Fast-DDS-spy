.. raw:: html

  <h1>
    eProsima Fast DDS Spy Documentation
  </h1>

.. image:: /rst/figures/eprosima_logo.svg
  :height: 100px
  :width: 100px
  :align: left
  :alt: eProsima
  :target: http://www.eprosima.com/

*eProsima Fast DDS Spy* is CLI interactive tool that allows to introspect a DDS network in human readable format.
It is possible to query the network about the DomainParticipants connected, their endpoints (DataWriters and DataReaders) and the topics they communicate in.
It is also possible to see the user data sent through network topics in a schematic format in run time.


########
Overview
########

*eProsima Fast DDS Spy* is a tool that introspect or "sniffing" DDS packages in the network and maintain a local database that is accessible from a interactive CLI.
*Fast DDS Spy* responds to user commands introduced by text and print in ``stdin`` the information requested.
This tool has several commands to interact with, that allows to get information regarding the status of the network.
It supports to list topics existing, list Participants, DataReaders, DataWriters and even read user data in real time in a human readable format.

.. figure:: /rst/figures/overview.png
    :align: center

*eProsima Fast DDS Spy* is easily configurable and installed with a default setup, so that DDS topics, data types and entities are automatically discovered without the need to specify the types of data.
This is because this tool exploits the DynamicTypes functionality of `eProsima Fast DDS <https://fast-dds.docs.eprosima.com>`_, the C++ implementation of the `DDS (Data Distribution Service) Specification <https://www.omg.org/spec/DDS/About-DDS/>`_ defined by the `Object Management Group (OMG) <https://www.omg.org/>`_.


###############################
Contacts and Commercial support
###############################

Find more about us at `eProsima's webpage <https://eprosima.com/>`_.

Support available at:

* Email: support@eprosima.com
* Phone: +34 91 804 34 48

#################################
Contributing to the documentation
#################################

*Fast DDS Spy Documentation* is an open source project, and as such all contributions, both in the form of
feedback and content generation, are most welcomed.
To make such contributions, please refer to the
`Contribution Guidelines <https://github.com/eProsima/all-docs/blob/master/CONTRIBUTING.md>`_ hosted in our GitHub
repository.

##############################
Structure of the documentation
##############################

This documentation is organized into the sections below.

* :ref:`Installation Manual <installation_manual_linux>`
* :ref:`Developer Manual <developer_manual_installation_sources_linux>`
* :ref:`Release Notes <notes>`
