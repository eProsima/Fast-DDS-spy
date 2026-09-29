
.. add orphan tag when new info added to this file

:orphan:

###################
Forthcoming Version
###################

Next release will include the following **new features**:

* Support topic-name endpoint profile lookup: when creating a :term:`DataReader` for a topic, a loaded XML
  ``data_reader`` profile whose name matches the topic name is automatically applied, giving users control over
  QoS fields such as history, memory policy and transport.
  Durability, reliability, ownership and history depth explicitly set in the YAML configuration take precedence
  over the profile.
  Check :ref:`user_manual_configuration_xml_endpoint_profiles` section.

  - New ``endpoint-profile-name`` tag added to select a specific profile for a topic.
