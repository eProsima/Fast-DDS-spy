.. _notes:

.. include:: forthcoming_version.rst

##############
Version v1.5.4
##############

This release includes the following **features**:

* Support topic-name endpoint profile lookup: when creating a :term:`DataReader` for a topic, a loaded XML
  ``data_reader`` profile whose name matches the topic name is automatically applied, giving users control over
  QoS fields such as history, memory policy and transport.
  Durability, reliability, ownership and history depth explicitly set in the YAML configuration take precedence
  over the profile
  (see :ref:`Endpoint profiles <user_manual_configuration_dds__xml_endpoint_profiles>`)
* Add the ``endpoint-profile-name`` tag to select a specific profile for a topic
* Reject the configuration tags that the *Fast DDS Spy* does not apply: ``qos`` and ``filter`` within
  ``allowlist`` and ``blocklist``, and ``participants`` within ``topics``

This release includes the following **bugfixes**:

* Match the partition filters against the default empty partition, so that endpoints announcing no partition are filtered correctly
* Report the correct default value of ``--log-verbosity`` in the application's help text
* Solve flaky tests and stabilize the application tests on Windows

This release includes the following **improvements**:

* Refactor the partitions logic to read the partitions of an endpoint from its announced QoS, instead of from a separate snapshot
* Run the ``Uncrustify`` and ``Python Linters`` workflows only in pull requests
* Remove the nightly CI jobs targeting the End-of-Life ``0.x`` branch

This release includes the following **documentation updates**:

* Warn that the configuration file is validated against a schema and that unknown tags are rejected
* Add a tag tree to the *DDS* and *Specs* configuration sections
* Document the ``dds: partitions``, ``topics: filter``, ``qos: endpoint-profile-name`` and
  ``specs: rtps`` configuration tags
* Document the accepted values and required tags of each configuration tag
* Document that the *Fast DDS Spy* always blocks the ROS 2 service topics ``rq/*`` and ``rr/*``
* Correct the default value of ``specs: discovery-time`` and the tag named in the *ROS 2 Easy Mode*
  warning
* Complete the command summary table with the missing keywords and arguments
* Update the Docker image installation instructions to use eProsima's *Fast DDS Suite*
* Merge the Developer Manual into the Installation Manual and remove the Developer Manual

This release includes the following **Dependencies Update**:

.. list-table::
    :header-rows: 1

    *   -
        - Repository
        - Old Version
        - New Version
    *   - Foonathan Memory Vendor
        - `eProsima/foonathan_memory_vendor <https://github.com/eProsima/foonathan_memory_vendor>`__
        - `v1.4.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.4.1>`__
        - `v1.4.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.4.1>`__
    *   - Fast CDR
        - `eProsima/Fast-CDR <https://github.com/eProsima/Fast-CDR>`__
        - `v2.3.6 <https://github.com/eProsima/Fast-CDR/releases/tag/v2.3.6>`__
        - `v2.3.6 <https://github.com/eProsima/Fast-CDR/releases/tag/v2.3.6>`__
    *   - Fast DDS
        - `eProsima/Fast-DDS <https://github.com/eProsima/Fast-DDS>`__
        - `v3.6.2 <https://github.com/eProsima/Fast-DDS/releases/tag/v3.6.2>`__
        - `v3.6.2 <https://github.com/eProsima/Fast-DDS/releases/tag/v3.6.2>`__
    *   - Dev Utils
        - `eProsima/dev-utils <https://github.com/eProsima/dev-utils>`__
        - `v1.5.3 <https://github.com/eProsima/dev-utils/releases/tag/v1.5.3>`__
        - `v1.5.4 <https://github.com/eProsima/dev-utils/releases/tag/v1.5.4>`__
    *   - DDS Pipe
        - `eProsima/DDS-Pipe <https://github.com/eProsima/DDS-Pipe>`__
        - `v1.5.3 <https://github.com/eProsima/DDS-Pipe/releases/tag/v1.5.3>`__
        - `v1.5.4 <https://github.com/eProsima/DDS-Pipe/releases/tag/v1.5.4>`__

#################
Previous Versions
#################

.. include:: ./previous_versions/v1.5.3.rst
.. include:: ./previous_versions/v1.5.2.rst
.. include:: previous_versions/v1.5.1.rst
.. include:: ./previous_versions/v1.5.0.rst
.. include:: ./previous_versions/v1.4.0.rst
.. include:: ./previous_versions/v1.3.0.rst
.. include:: ./previous_versions/v1.2.0.rst
.. include:: ./previous_versions/v1.1.0.rst
.. include:: ./previous_versions/v1.0.0.rst
.. include:: ./previous_versions/v0.4.0.rst
.. include:: ./previous_versions/v0.3.0.rst
.. include:: ./previous_versions/v0.2.0.rst
.. include:: ./previous_versions/v0.1.0.rst
