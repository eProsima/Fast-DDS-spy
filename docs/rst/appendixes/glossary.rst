.. include:: ../exports/alias.include

.. _glossary:

########
Glossary
########

.. glossary::

    CLI
        **Command Line Interface**
        This is a channel of communication between an application and a user using the Command Line, so-called Terminal.

    DDS
        **Data Distribution Service** protocol.
        Specification: `<https://www.omg.org/spec/DDS/>`_.

    YAML
        **YAML Ain Markup Language** human-friendly data serialization language.
        Specification: `<https://yaml.org/>`_.


DDS nomenclature
================

.. glossary::

    DataReader
        DDS element that subscribes to a specific Topic.
        It belong to one and only one Participant, and it is uniquely identified by a Guid.

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1/fastdds/dds_layer/subscriber/subscriber.html>`__
        for further information.

    DataWriter
        DDS entity that publish data in a specific Topic.
        It belong to one and only one Participant, and it is uniquely identified by a Guid.

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1/fastdds/dds_layer/publisher/dataWriter/dataWriter.html>`__
        for further information.

    Discovery Server
        Discovery Server Discovery Protocol is a Fast DDS feature that enables a new Discovery mechanism based on a
        Server that filters and distribute the discovery information.
        This is highly recommended in networks where multicast is not available (e.g. WAN).

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1/fastdds/discovery/discovery_server.html>`__
        for further information.

    Domain Id
        The Domain Id is a virtual partition for DDS networks.
        Only DomainParticipants with the same Domain Id would be able to communicate to each other.
        DomainParticipants  in different Domains will not even discover each other.

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1//fastdds/dds_layer/domain/domain.html>`__
        for further information.

    DomainParticipant
        A DomainParticipant is the entry point of the application to a DDS Domain.
        Every DomainParticipant is linked to a single domain from its creation, and cannot change such domain.
        It also acts as a factory for Publisher, Subscriber and Topic.

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1/fastdds/dds_layer/domain/domainParticipant/domainParticipant.html>`__
        for further information.

    Endpoint
        DDS element that publish or subscribes in a specific Topic. Endpoint kinds are *DataWriter* or *DataReader*.

    Entity Id
        DDS Identifier created by 4 bytes, represented by 4 hexadecimal values separated by ``.``.
        This identifier differentiates each of the entities inside a :term:`DomainParticipant`.
        e.g. ``00.00.01.c1``.

    Guid Prefix
        DDS Identifier created by 12 bytes, represented by 12 hexadecimal values separated by ``.``.
        Every entity that belongs to the same :term:`DomainParticipant` has the same Guid Prefix.
        The EntityId uniquely identifies sub-entities inside a Participant.
        e.g. ``01.0f.22.ba.3b.47.ab.3c.00.00.00.00``.

    Guid
        Global Unique Identifier.
        It contains a :term:`Guid Prefix` and an :term:`Entity Id`.
        Identifies uniquely a DDS entity (:term:`DomainParticipant`, :term:`DataWriter` or :term:`DataReader`).
        e.g. ``01.0f.22.ba.3b.47.ab.3c.00.00.00.00|00.00.01.c1``.

    Topic
        DDS isolation abstraction to encapsulate subscriptions and publications.
        Each Topic is uniquely identified by a topic name and a topic type name (name of the data type it transmits).

        See `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/v2.4.1/fastdds/dds_layer/topic/topic.html>`__
        for further information.
