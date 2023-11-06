.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _user_manual_configuration:

#############
Configuration
#############

A |spy| instance can be configured by a :term:`YAML` configuration file.
In order to retrieve a configuration file to a |spy|, use :ref:`user_manual_user_interface_configuration_file_argument`.

Version
=======

The YAML Configuration support a ``version`` value to identify the configuration version to parse the file.
In future releases could be common to change the YAML format (some key words, fields, etc.).
This value allow to keep using the same YAML file using an old configuration format, maintaining compatibility.

.. list-table::
    :header-rows: 1

    *   - Configuration Versions
        - String in ``version`` tag
        - |spy| activation release

    *   - version 1.0
        - ``v1.0``
        - *v0.1.0*

.. _user_manual_configuration_dds:

DDS Configurations
==================

The YAML Configuration supports a ``dds`` **optional** tag that contains certain :term:`DDS` configurations.
The values available to configure are:

DDS Domain Id
-------------

In order to execute a |spy| instance in a :term:`Domain Id` different than the default (``0``) use tag ``domain``.

.. _user_manual_configuration_dds_ignore_participant_flags:

Ignore Participant Flags
------------------------

A set of discovery traffic filters can be defined in order to add an extra level of isolation.
This configuration option can be set through the ``ignore-participant-flags`` tag:

.. code-block:: yaml

    ignore-participant-flags: no_filter                          # No filter (default)
    # or
    ignore-participant-flags: filter_different_host              # Discovery traffic from another host is discarded
    # or
    ignore-participant-flags: filter_different_process           # Discovery traffic from another process on same host is discarded
    # or
    ignore-participant-flags: filter_same_process                # Discovery traffic from own process is discarded
    # or
    ignore-participant-flags: filter_different_and_same_process  # Discovery traffic from own host is discarded

See `Ignore Participant Flags <https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/general_disc_settings.html?highlight=ignore%20flags#ignore-participant-flags>`_ for more information.


.. _user_manual_configuration_dds_custom_transport_descriptors:

Custom Transport Descriptors
----------------------------

By default, |spy| internal participants are created with enabled `UDP <https://fast-dds.docs.eprosima.com/en/latest/fastdds/transport/udp/udp.html>`_ and `Shared Memory <https://fast-dds.docs.eprosima.com/en/latest/fastdds/transport/shared_memory/shared_memory.html>`_ transport descriptors.
The use of one or the other for communication will depend on the specific scenario, and whenever both are viable candidates, the most efficient one (Shared Memory Transport) is automatically selected.
However, a user may desire to force the use of one of the two, which can be accomplished via the ``transport`` configuration tag.

.. code-block:: yaml

    transport: builtin    # UDP & SHM (default)
    # or
    transport: udp        # UDP only
    # or
    transport: shm        # SHM only

.. warning::

    When configured with ``transport: shm``, |spy| will only communicate with applications using Shared Memory Transport exclusively (with disabled UDP transport).


.. _user_manual_configuration_dds__interface_whitelist:

Interface Whitelist
-------------------

Optional tag ``whitelist-interfaces`` allows to limit the network interfaces used by UDP and TCP transport.
This may be useful to only allow communication within the host (note: same can be done with :ref:`user_manual_configuration_dds_ignore_participant_flags`).
Example:

.. code-block:: yaml

    whitelist-interfaces:
      - "127.0.0.1"    # Localhost only

See `Interface Whitelist <https://fast-dds.docs.eprosima.com/en/latest/fastdds/transport/whitelist.html>`_ for more information.

.. warning::

    When providing an interface whitelist, external participants with which communication is desired must also be configured with interface whitelisting.

Topic Filtering
---------------

|spy| includes a mechanism to automatically detect which topics are being used in a DDS network.
By automatically detecting these topics, a |spy| creates internal :term:`Readers<DataReader>` for each topic and for each participant in order to read data published on each discovered topic.

.. note::

    |spy| entities are created with the QoS of the first subscriber found in this topic.

|spy| allows filtering of DDS :term:`Topics<Topic>`, that is, it allows to define which DDS Topics are going to be
relayed by the application.
This way, it is possible to define a set of rules in |spy| to filter those data samples the user does not wish to
forward.

It is not mandatory to define such set of rules in the configuration file. In this case, a |spy| will read all
the data published under the topics that it automatically discovers within the DDS network to which it connects.

To define these data filtering rules based on the topics to which they belong, two lists are available:

* Allowed topics list (``allowlist``)
* Block topics list (``blocklist``)

These two lists of topics listed above are defined by a tag in the *YAML* configuration file, which defines a
*YAML* vector (``[]``).
This vector contains the list of topics for each filtering rule.
Each topic is determined by its entries ``name`` and ``type``, strings referring the topic name and topic data type name.

.. note::

    Placing quotation marks around values in a YAML file is generally optional. However, values containing wildcard
    characters must be enclosed in single or double quotes.

Allow topic list (``allowlist``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This is the list of topics that |spy| will forward, i.e. the data published under the topics matching the
expressions in the ``allowlist`` will be relayed by |spy|.

.. note::

    If no ``allowlist`` is provided, data will be read for all topics (unless filtered out in ``blocklist``).


Block topic list (``blocklist``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This is the list of topics that the |spy| will block, that is, all data published under the topics matching the
filters specified in the ``blocklist`` will be discarded by the |spy| and therefore will not be relayed.

This list takes precedence over the ``allowlist``.
If a topic matches an expression both in the ``allowlist`` and in the ``blocklist``, the ``blocklist`` takes precedence,
causing the data under this topic to be discarded.

Topic type format
-----------------

The optional ``ros2-types`` tag enables specification of the format for displaying schemas.
When set to ``true``, schemas are displayed in ROS 2 message format (.msg).
If set to ``false``, schemas are displayed in OMG IDL format (.idl).

.. _user_manual_configuration_specs:

Specs Configurations
====================

The YAML Configuration supports a ``specs`` **optional** tag that contains certain options related with the overall configuration of the application.
The values available to configure are:

Number of Threads
-----------------

``specs`` supports a ``threads`` **optional** value that allows the user to set a maximum number of threads for the internal :code:`ThreadPool`.
This ThreadPool allows to limit the number of threads spawned by the application.
This improves the performance of the data transmission between participants.

This value should be set by each user depending on each system characteristics.
By default, this value is ``12``.

.. _history_depth_configuration:

Maximum History Depth
---------------------

``specs`` supports a ``max-depth`` **optional** value that configures the history size of the Fast DDS internal entities.
By default, the depth of every RTPS History instance is :code:`5000`.
This value should be decreased when the sample size and/or number of created endpoints (increasing with the number of
topics) are as big as to cause memory exhaustion issues.

.. _user_manual_configuration_discovery_time:

Discovery Time
--------------

``specs`` supports a ``discovery-time`` **optional** value that allows the user to set the time (in milliseconds) before a :ref:`user_manual_user_interface_one_shot` retrieves the output and closes.
This parameter is useful for very big networks, as |spy| may not discover the whole network fast enough to return a complete information.
By default, this value is ``1000`` (1 second).

.. _user_manual_configuration_default:

Default Configuration
=====================

This is a YAML file that uses all supported configurations and set them as default:

.. warning::

    This example can be used as a quick reference, but it may not be correct due to incompatibility or exclusive properties. **Do not take it as a working example**.

.. code-block:: yaml

    version: 1.0

    dds:
      domain: 0

      allowlist:
        - name: "topic_name"
          type: "topic_type"

      blocklist:
        - name: "topic_name"
          type: "topic_type"

      builtin-topics:
        - name: "HelloWorldTopic"
          type: "HelloWorld"
          qos:
            reliability: true
            durability: true
            keyed: false
            partitions: true
            ownership: false
            downsampling: 4
            max-reception-rate: 10

      ignore-participant-flags: no_filter
      transport: builtin
      whitelist-interfaces:
        - "127.0.0.1"

      ros2-types: false

    specs:
      threads: 12
      max-history-depth: 5000
      discovery-time: 1000
