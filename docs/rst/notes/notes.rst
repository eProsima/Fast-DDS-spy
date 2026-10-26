.. _notes:

.. include:: forthcoming_version.rst

##############
Version v1.5.4
##############

This release includes the following **features**:

* Support XML ``data_reader`` profiles for topic :term:`DataReaders <DataReader>`, applied automatically when the
  profile name matches the topic name or selected with the new ``endpoint-profile-name`` tag, with QoS set in the
  YAML configuration taking precedence
  (see :ref:`Endpoint profiles <user_manual_configuration_dds__xml_endpoint_profiles>`)

This release includes the following **improvements**:

* Refactor the partitions logic
* Run the ``Uncrustify`` and ``Python Linters`` workflows only in pull requests
* Remove the nightly CI jobs targeting the End-of-Life ``0.x`` branch
* Align the validation of the configuration file with what the *Fast DDS Spy* applies: reject tags that were accepted
  but never applied and report configurations that were previously rejected only while parsing

This release includes the following **bugfixes**:

* Match the partition filters against the default empty partition, so that endpoints announcing no partition are filtered correctly
* Report the correct default value of ``--log-verbosity`` in the application's help text
* Solve flaky tests and stabilize the application tests on Windows

This release includes the following **documentation updates**:

* Rework the documentation: the configuration page contains now a reference tag tree and more detailed information of
  the tags, and it warns that unknown tags are rejected by schema validation; the command summary table is extended;
  and the Installation Manual is updated and merged with the Developer Manual

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
.. include:: ./previous_versions/v1.5.1.rst
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
