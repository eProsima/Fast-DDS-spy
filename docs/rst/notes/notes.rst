.. include:: ../exports/alias.include

.. _notes:

.. .. include:: forthcoming_version.rst

##############
Version v0.5.0
##############

.. warning::

    This is the last release of the ``0.x`` series.
    This branch has reached its End-of-Life (EOL) and will receive no further releases, bugfixes or security updates.
    Users are encouraged to migrate to the latest stable version of |espy|.

This release includes the following **Configuration Features**:

* New :ref:`domain argument <user_manual_user_interface_domain_argument>` to configure the ``domain`` through the command-line.
* Change the default log configuration verbosity to ``error``.

This release includes the following **Bugfixes**:

* Accept IPv6 interfaces and interface names in the ``whitelist-interfaces`` configuration.
* Update the |spy| listeners to align with the changes in |ddspipe|.
* Add the missing command to the Docker installation instructions.

This release includes the following **CI improvements**:

* Upgrade to Ubuntu Noble (24.04).
* Remove Ubuntu Focal (20.04) from the CI.
* Support multiple versions of Fast DDS in the CI.
* Empty the XTSAN tests list.

This release includes the following **Dependencies Update**:

.. list-table::
    :header-rows: 1

    *   -
        - Repository
        - Old Version
        - New Version
    *   - Foonathan Memory Vendor
        - `eProsima/foonathan_memory_vendor <https://github.com/eProsima/foonathan_memory_vendor>`__
        - `v1.3.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.1>`__
        - `v1.3.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.1>`__
    *   - Fast CDR
        - `eProsima/Fast-CDR <https://github.com/eProsima/Fast-CDR>`__
        - `v2.2.0 <https://github.com/eProsima/Fast-CDR/releases/tag/v2.2.0>`__
        - `v2.2.8 <https://github.com/eProsima/Fast-CDR/releases/tag/v2.2.8>`__
    *   - Fast DDS
        - `eProsima/Fast-DDS <https://github.com/eProsima/Fast-DDS>`__
        - `v2.14.0 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.14.0>`__
        - `v2.14.7 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.14.7>`__
    *   - Dev Utils
        - `eProsima/dev-utils <https://github.com/eProsima/dev-utils>`__
        - `v0.6.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.6.0>`__
        - `v0.7.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.7.0>`__
    *   - DDS Pipe
        - `eProsima/DDS-Pipe <https://github.com/eProsima/DDS-Pipe.git>`__
        - `v0.4.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.4.0>`__
        - `v0.5.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.5.0>`__


#################
Previous Versions
#################

.. include:: ./previous_versions/v0.4.0.rst
.. include:: ./previous_versions/v0.3.0.rst
.. include:: ./previous_versions/v0.2.0.rst
.. include:: ./previous_versions/v0.1.0.rst
