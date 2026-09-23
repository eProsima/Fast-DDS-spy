
.. add orphan tag when new info added to this file

:orphan:

###################
Forthcoming Version
###################

Next release will include the following **bugfixes**:

* Match the partition filters against the default empty partition, so that endpoints announcing no
  partition are filtered correctly
* Report the correct default value of ``--log-verbosity`` in the application's help text
* Solve flaky tests
* Run the ``Uncrustify`` and ``Python Linters`` workflows only in pull requests

Next release will include the following **improvements**:

* Reject the configuration tags that the *Fast DDS Spy* does not apply: ``qos`` and ``filter`` within
  ``allowlist`` and ``blocklist``, and ``participants`` within ``topics``
* Read the partitions of an endpoint from its announced QoS, instead of from a separate snapshot

Next release will include the following **documentation updates**:

* Warn that the configuration file is validated against a schema and that unknown tags are rejected
* Add a tag tree to the *DDS* and *Specs* configuration sections
* Document the ``dds: partitions``, ``topics: filter``, ``qos: endpoint-profile-name`` and
  ``specs: rtps`` configuration tags
* Document the accepted values and required tags of each configuration tag
* Document that the *Fast DDS Spy* always blocks the ROS 2 service topics ``rq/*`` and ``rr/*``
* Correct the default value of ``specs: discovery-time`` and the tag named in the *ROS 2 Easy Mode*
  warning
* Complete the command summary table with the missing key-words and arguments
* Update the Docker image installation instructions to use eProsima's *Fast DDS Suite*
* The Installation Manual has been merged with the Developer Manual, and the latter is removed
