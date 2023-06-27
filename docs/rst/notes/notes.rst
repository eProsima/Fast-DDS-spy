.. include:: ../exports/alias.include

.. _notes:

.. TODO uncomment when there are forthcoming notes
.. .. include:: forthcoming_version.rst

##############
Version v0.2.0
##############

This release includes the following **Configuration Features**:

* Support :ref:`Interface Whitelisting <user_manual_configuration_dds__interface_whitelist>`.
* Support :ref:`Custom Transport Descriptors <user_manual_configuration_dds_custom_transport_descriptors>` (UDP or Shared Memory only).
* Support :ref:`Ignore Participant Flags <user_manual_configuration_dds_ignore_participant_flags>`.

This release includes the following **Internal Changes** and **Bugfixes**:

* Fix C++ *namespace* ambiguities in yaml configuration.
* Change default log-filter to only show warning coming from DDS Spy source code.
* Filter out topics associated to services (RPC).
* Parameterize simulated endpoints.

This release includes the following **Documentation Updates**:

* Update introduction section in README file.

This release includes the following change in the **Continuous Integration** process:

* Update CI to use `eProsima CI <https://github.com/eProsima/eProsima-CI>`__.


#################
Previous Versions
#################

.. include:: ./previous_versions/v0.1.0.rst
