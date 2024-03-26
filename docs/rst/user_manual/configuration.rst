.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _user_manual_configuration:

#############
Configuration
#############

A |spy| instance can be configured by a :term:`YAML` configuration file.
In order to retrieve a configuration file to a |spy|, use :ref:`user_manual_user_interface_configuration_file_argument`.

.. _user_manual_configuration_dds:

DDS Configurations
==================

The YAML Configuration supports a ``dds`` **optional** tag that contains certain :term:`DDS` configurations.
The values available to configure are:

.. _user_manual_configuration_dds__topic_filtering:

Topic Filtering
---------------

The |spy| automatically detects the topics that are being used in a DDS Network.
The |spy| then creates internal DDS :term:`Readers<DataReader>` for each topic to process the data published.
The |spy| allows filtering DDS :term:`Topics<Topic>`, that is, it allows users to configure the DDS :term:`Topics<Topic>` to process.
These data filtering rules can be configured under the ``allowlist`` and ``blocklist`` tags.
If the ``allowlist`` and ``blocklist`` are not configured, the |spy| will process all the data published on the topics it discovers.
If both the ``allowlist`` and ``blocklist`` are configured and a topic appears in both of them, the ``blocklist`` has priority and the topic will be blocked.

Topics are determined by the tags ``name`` (required) and ``type``, both of which accept wildcard characters.

.. note::

    Placing quotation marks around values in a YAML file is generally optional, but values containing wildcard characters do require single or double quotation marks.

Consider the following example:

.. code-block:: yaml

    allowlist:
      - name: AllowedTopic1
        type: Allowed

      - name: AllowedTopic2
        type: "*"

      - name: HelloWorldTopic
        type: HelloWorld

    blocklist:
      - name: "*"
        type: HelloWorld

In this example, the data in the topic ``AllowedTopic1`` with type ``Allowed`` and the data in the topic ``AllowedTopic2`` with any type will be processed by the |spy|.
The data in the topic ``HelloWorldTopic`` with type ``HelloWorld`` will be blocked, since the ``blocklist`` is blocking all topics with any name and with type ``HelloWorld``.

.. _user_manual_configuration_dds__topic_qos:

Topic QoS
---------

The following is the set of QoS that are configurable for a topic.
For more information on topics, please read the `Fast DDS Topic <https://fast-dds.docs.eprosima.com/en/latest/fastdds/dds_layer/topic/topic.html>`_ section.

.. list-table::
    :header-rows: 1

    *   - Quality of Service
        - Yaml tag
        - Data type
        - Default value
        - QoS set

    *   - Reliability
        - ``reliability``
        - *bool*
        - ``false``
        - ``RELIABLE`` / ``BEST_EFFORT``

    *   - Durability
        - ``durability``
        - *bool*
        - ``false``
        - ``TRANSIENT_LOCAL`` / ``VOLATILE``

    *   - Ownership
        - ``ownership``
        - *bool*
        - ``false``
        - ``EXCLUSIVE_OWNERSHIP_QOS`` / ``SHARED_OWNERSHIP_QOS``

    *   - Partitions
        - ``partitions``
        - *bool*
        - ``false``
        - Topic with / without partitions

    *   - Key
        - ``keyed``
        - *bool*
        - ``false``
        - Topic with / without `key <https://fast-dds.docs.eprosima.com/en/latest/fastdds/dds_layer/topic/typeSupport/typeSupport.html#data-types-with-a-key>`_

    *   - History Depth
        - ``history-depth``
        - *unsigned integer*
        - ``5000``
        - :ref:`user_manual_configuration_dds__history_depth`

    *   - Max Reception Rate
        - ``max-rx-rate``
        - *float*
        - ``0`` (unlimited)
        - :ref:`user_manual_configuration_dds__max_rx_rate`

    *   - Downsampling
        - ``downsampling``
        - *unsigned integer*
        - ``1``
        - :ref:`user_manual_configuration_dds__downsampling`

.. warning::

    Manually configuring ``TRANSIENT_LOCAL`` durability may lead to incompatibility issues when the discovered reliability is ``BEST_EFFORT``.
    Please ensure to always configure the ``reliability`` when configuring the ``durability`` to avoid the issue.

.. _user_manual_configuration_dds__history_depth:

History Depth
^^^^^^^^^^^^^

The ``history-depth`` tag configures the history depth of the Fast DDS internal entities.
By default, the depth of every RTPS History instance is :code:`5000`.
Its value should be decreased when the sample size and/or number of created endpoints (increasing with the number of topics) are big enough to cause memory exhaustion issues.

.. _user_manual_configuration_dds__max_rx_rate:

Max Reception Rate
^^^^^^^^^^^^^^^^^^

The ``max-rx-rate`` tag limits the frequency [Hz] at which samples are processed by discarding messages received before :code:`1/max-rx-rate` seconds have passed since the last processed message.
It only accepts non-negative numbers.
By default it is set to ``0``; it processes samples at an unlimited reception rate.

.. _user_manual_configuration_dds__downsampling:

Downsampling
^^^^^^^^^^^^

The ``downsampling`` tag reduces the sampling rate of the received data by only keeping *1* out of every *n* samples received (per topic), where *n* is the value specified under the ``downsampling`` tag.
When the ``max-rx-rate`` tag is also set, downsampling only applies to messages that have passed the ``max-rx-rate`` filter.
It only accepts positive integers.
By default it is set to ``1``; it accepts every message.

.. _user_manual_configuration_dds__manual_topics:

Manual Topics
-------------

A subset of :ref:`Topic QoS <user_manual_configuration_dds__topic_qos>` can be manually configured for a specific topic under the tag ``topics``.
The tag ``topics`` has a required ``name`` tag that accepts wildcard characters.
It also has two optional tags: a ``type`` tag that accepts wildcard characters and a ``qos`` tag with the :ref:`Topic QoS <user_manual_configuration_dds__topic_qos>` that the user wants to manually configure.
If a ``qos`` is not manually configured, it will get its value by discovery.

.. code-block:: yaml

    topics:
      - name: "temperature/*"
        type: "temperature/types/*"
        qos:
          max-tx-rate: 15
          downsampling: 2

.. note::

    The :ref:`Topic QoS <user_manual_configuration_dds__topic_qos>` configured in the Manual Topics take precedence over the :ref:`Specs Topic QoS <user_manual_configuration_specs_topic_qos>`.

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

.. _user_manual_configuration_discovery_time:

Discovery Time
--------------

``specs`` supports a ``discovery-time`` **optional** value that allows the user to set the time (in milliseconds) before a :ref:`user_manual_user_interface_one_shot` retrieves the output and closes.
This parameter is useful for very big networks, as |spy| may not discover the whole network fast enough to return a complete information.
By default, this value is ``1000`` (1 second).

.. _user_manual_configuration_specs_topic_qos:

QoS
---

``specs`` supports a ``qos`` **optional** tag to configure the default values of the :ref:`Topic QoS <user_manual_configuration_dds__topic_qos>`.

.. note::

    The :ref:`Topic QoS <user_manual_configuration_dds__topic_qos>` configured in ``specs`` can be overwritten by the :ref:`Manual Topics <user_manual_configuration_dds__manual_topics>`.

.. _spy_specs_logging:

Logging
-------

``specs`` supports a ``logging`` **optional** tag to configure the |spy| logs.
Under the ``logging`` tag, users can configure the type of logs to display and filter the logs based on their content and category.
When configuring the verbosity to ``info``, all types of logs, including informational messages, warnings, and errors, will be displayed.
Conversely, setting it to ``warning`` will only show warnings and errors, while choosing ``error`` will exclusively display errors.
By default, the filter allows all errors to be displayed, while selectively permitting warning and informational messages from ``FASTDDSSPY`` category.

.. note::

    Configuring the logs via the Command-Line is still active and takes precedence over YAML configuration when both methods are used simultaneously.

.. list-table::
    :header-rows: 1

    *   - Logging
        - Yaml tag
        - Description
        - Data type
        - Default value
        - Possible values

    *   - Verbosity
        - ``verbosity``
        - Show messages of equal |br|
          or higher importance.
        - *enum*
        - ``error``
        - ``info`` / ``warning`` / ``error``

    *   - Filter
        - ``filter``
        - Regex to filter the category  |br|
          or message of the logs.
        - *string*
        - info : ``FASTDDSSPY`` |br|
          warning : ``FASTDDSSPY`` |br|
          error : ``""``
        - Regex string

.. note::

    For the logs to function properly, the ``-DLOG_INFO=ON`` compilation flag is required.


The |spy| prints the logs by default (warnings and errors in the standard error and infos in the standard output).
The |spy|, however, can also publish the logs in a DDS topic.
To publish the logs, under the tag ``publish``, set ``enable: true`` and set a ``domain`` and a ``topic-name``.
The type of the logs published is defined as follows:

**LogEntry.idl**

.. code-block:: idl

    const long UNDEFINED = 0x10000000;
    const long SAMPLE_LOST = 0x10000001;
    const long TOPIC_MISMATCH_TYPE = 0x10000002;
    const long TOPIC_MISMATCH_QOS = 0x10000003;

    enum Kind {
      Info,
      Warning,
      Error
    };

    struct LogEntry {
      @key long event;
      Kind kind;
      string category;
      string message;
      string timestamp;
    };

.. note::

    The type of the logs can be published by setting ``publish-type: true``.

**Example of usage**

.. code-block:: yaml

    logging:
      verbosity: info
      filter:
        error: "DDSPIPE|FASTDDSSPY"
        warning: "DDSPIPE|FASTDDSSPY"
        info: "FASTDDSSPY"
      publish:
        enable: true
        domain: 84
        topic-name: "FastDdsSpyLogs"
        publish-type: false
      stdout: true

.. _user_manual_configuration_default:

Default Configuration
=====================

This is a YAML file that uses all supported configurations and set them as default:

.. warning::

    This example can be used as a quick reference, but it may not be correct due to incompatibility or exclusive properties. **Do not take it as a working example**.

.. code-block:: yaml

    dds:
      domain: 0

      allowlist:
        - name: "topic_name"
          type: "topic_type"

      blocklist:
        - name: "topic_name"
          type: "topic_type"

      topics:
        - name: "temperature/*"
          type: "temperature/types/*"
          qos:
            max-rx-rate: 5
            downsampling: 1

      ignore-participant-flags: no_filter
      transport: builtin
      whitelist-interfaces:
        - "127.0.0.1"

      ros2-types: false

    specs:
      threads: 12
      discovery-time: 1000

      qos:
        history-depth: 5000
        max-rx-rate: 10
        downsampling: 2

      logging:
        verbosity: error
        filter:
          error: ""
          warning: "FASTDDSSPY"
          info: "FASTDDSSPY"
        publish:
          enable: true
          domain: 0
          topic-name: "FastDdsSpyLogs"
          publish-type: false
        stdout: true
