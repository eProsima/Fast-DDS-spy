.. include:: ../exports/alias.include

.. _notes:

.. TODO uncomment when there are forthcoming notes
.. .. include:: forthcoming_version.rst

##############
Version v0.3.0
##############

This release includes the following **Configuration Features**:

* Display data type information in either ROS 2 format (MSG) or DDS format (IDL).
* :ref:`Topic configuration <user_manual_configuration_dds__manual_topics>`.
* :ref:`Max Reception Rate <user_manual_configuration_dds__max_rx_rate>` per topic.
* :ref:`Downsampling <user_manual_configuration_dds__downsampling>` per topic.
* Remove the support for built-in Topics.

This release includes the following **Internal Changes** and **Bugfixes**:

* Support both fastcdr v1 and v2.
* Add support for Fast DDS versions lower than v2.13.
* Set `app_id` and `app_metadata` attributes in  *eProsima Fast DDS Spy* participants.
* Add participant and endpoint info topics to allowlist.
* Add internal builtin topics as DDS Topics.
* Reload internal topics when reloading the configuration file.
* Process an empty YAML when a configuration file isn't provided.

This release includes the following **Documentation Updates**:

* Update next steps in main example and remove warning in docker installation.
* Fix documentation build for Sphinx versions lower than 5.0.

This release includes the following **Dependencies Update**:

.. list-table::
    :header-rows: 1

    *   -
        - Repository
        - Old Version
        - New Version
    *   - Foonathan Memory Vendor
        - `eProsima/foonathan_memory_vendor <https://github.com/eProsima/foonathan_memory_vendor>`_
        - `v1.3.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.1>`_
        - `v1.3.1 <https://github.com/eProsima/foonathan_memory_vendor/releases/tag/v1.3.1>`_
    *   - Fast CDR
        - `eProsima/Fast-CDR <https://github.com/eProsima/Fast-CDR>`_
        - `v1.1.0 <https://github.com/eProsima/Fast-CDR/releases/tag/v1.1.0>`_
        - `v2.1.3 <https://github.com/eProsima/Fast-CDR/releases/tag/v2.1.3>`_
    *   - Fast DDS
        - `eProsima/Fast-DDS <https://github.com/eProsima/Fast-DDS>`_
        - `v2.11.0 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.11.0>`_
        - `v2.13.1 <https://github.com/eProsima/Fast-DDS/releases/tag/v2.13.1>`_
    *   - Dev Utils
        - `eProsima/dev-utils <https://github.com/eProsima/dev-utils>`_
        - `v0.4.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.4.0>`_
        - `v0.5.0 <https://github.com/eProsima/dev-utils/releases/tag/v0.5.0>`_
    *   - DDS Pipe
        - `eProsima/DDS-Pipe <https://github.com/eProsima/DDS-Pipe.git>`_
        - `v0.2.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.2.0>`_
        - `v0.3.0 <https://github.com/eProsima/DDS-Pipe/releases/tag/v0.3.0>`_


#################
Previous Versions
#################

.. include:: ./previous_versions/v0.2.0.rst
.. include:: ./previous_versions/v0.1.0.rst
