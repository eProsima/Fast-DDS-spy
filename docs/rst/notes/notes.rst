.. include:: ../exports/alias.include

.. _notes:

.. include:: forthcoming_version.rst

##############
Version v0.2.0
##############

This release includes the following **Configuration Features**:

* Support :ref:`Interface Whitelisting <user_manual_configuration_dds__interface_whitelist>`.
* Support :ref:`Custom Transport Descriptors <user_manual_configuration_dds_custom_transport_descriptors>` (UDP or Shared Memory only).
* Support :ref:`Ignore Participant Flags <user_manual_configuration_dds_ignore_participant_flags>`.

This release includes the following **Internal Changes** and **Bugfixes**:

* Print only active participants when using verbose mode in ``participants`` command.
* Fix C++ *namespace* ambiguities in yaml configuration.
* Change default log-filter to only show warning coming from DDS Spy source code.
* Filter out topics associated to services (RPC).
* Parameterize simulated endpoints.

This release includes the following **Documentation Updates**:

* Update introduction section in README file.

This release includes the following change in the **Continuous Integration** process:

* Update CI to use `eProsima CI <https://github.com/eProsima/eProsima-CI>`__.

This release includes the following **Dependencies Update**:

.. list-table::
    :header-rows: 1

    *   -
        - Repository
        - Old Version
        - New Version
    *   - Foonathan Memory Vendor
        - `eProsima/foonathan_memory_vendor <https://github.com/eProsima/foonathan_memory_vendor>`_
        - `v1.3.0 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.0>`_
        - `v1.3.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.1>`_
    *   - Fast CDR
        - `eProsima/Fast-CDR <https://github.com/eProsima/Fast-CDR>`_
        - `v1.0.27 <https://github.com/eProsima/Fast-CDR/releases/tag/v1.0.27>`_
        - `v1.1.0 <https://github.com/eProsima/Fast-CDR/releases/tag/v1.1.0>`_
    *   - Fast DDS
        - `eProsima/Fast-DDS <https://github.com/eProsima/Fast-DDS>`_
        - `v2.10.1 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.10.1>`_
        - `v2.11.0 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.11.0>`_
    *   - Dev Utils
        - `eProsima/dev-utils <https://github.com/eProsima/dev-utils>`_
        - `v0.3.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.3.0>`_
        - `v0.4.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.4.0>`_
    *   - DDS Pipe
        - `eProsima/DDS-Pipe <https://github.com/eProsima/DDS-Pipe.git>`_
        - `v0.1.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.1.0>`_
        - `v0.2.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.2.0>`_


#################
Previous Versions
#################

.. include:: ./previous_versions/v0.1.0.rst
