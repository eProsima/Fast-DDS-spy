
.. add orphan tag when new info added to this file

:orphan:

###################
Forthcoming Version
###################

This release includes the following **new features**:

* *Fast DDS Spy* now supports topic-name endpoint profile lookup:
  when creating a :term:`DataReader` for a topic, a loaded XML profile whose name
  matches the topic name is automatically applied, giving users full control over QoS fields such as
  history, memory policy and transport.
  For more details, see :ref:`user_manual_configuration_xml_endpoint_profiles`.

This release includes the following **documentation updates**:

* Document topic-name endpoint profile lookup for the *Fast DDS Spy*,
  including QoS fields always enforced by the *Fast DDS Spy* regardless of the profile.
