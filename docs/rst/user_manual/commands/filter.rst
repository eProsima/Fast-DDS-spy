.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_filter:

######
Filter
######

**Filter** is a command that adds filters to restrict the spied information based on user-defined criteria.

Key-words
=========

These are the key-words recognized as this command:
``filter`` ``filters`` ``partitions`` ``f`` ``F``.

Categories
==========

Each category defines a filter used to restrict the spied information.
A category can include one or more filter strings,
and information is printed if it matches any of the strings in the category.

The **filter** command supports the following categories:

- ``partitions``
- ``topic``

Both categories can also be set from the configuration file, so that the filters are already in place
when the |spy| starts: the ``partitions`` category from the
:ref:`partitions <user_manual_configuration_dds__partitions>` tag, and the ``topic`` category from the
:ref:`Content Filter <user_manual_configuration_dds__content_filter>` of the Manual Topics.

Arguments
=========

The **Filter** command supports from 0 to 4 arguments:

*No argument*
-------------

When no arguments are given, this command shows all the filters added during runtime, divided into one list for
each ``category`` added at runtime.

The output format is described in :ref:`user_manual_command_filter_output`.

*1 argument:* `<clear/remove>`
------------------------------

- ``clear``: This argument **clears** all the category lists added to the filters.
- ``remove``: This argument **deletes** all the category lists added to the filters.

*2 arguments:* `<clear/remove> <partitions/topic>`
--------------------------------------------------

- ``clear``: This argument **clears** the ``partitions/topic`` list added to the filters.
- ``remove``: This argument **deletes** the ``partitions/topic`` list from the filters.

*3 arguments:* `<add/remove> <partitions/topic> <filter_str/topic_name>`
------------------------------------------------------------------------

- ``add partitions/topic``: This argument **adds** ``filter_str`` to the partitions filter list.
- ``remove partitions``: This argument **deletes** ``filter_str`` from the partitions filter list.
- ``remove topic``: This argument **deletes** the filter of the topic ``topic_name``.

*4 arguments:* `<set> topic <topic_name> <filter_str>`
------------------------------------------------------

- ``set``: This argument **sets** ``filter_str`` in the filter list of the topic ``topic_name``.

.. _user_manual_command_filter_output:

Output Format
=============

The filter information is retrieved with the following format:

.. code-block:: yaml

  Filter lists (1)

    category_1 (2):
      - filter_str_1
      - filter_str_2

    category_2 (2):
      - filter_str_1

Example
=======

Consider a DDS network where a ShapesDemo application is running with
the following two DataWriters:

- Circle (partition A) [key_topic_value: color = RED]
- Square (partitions B and C) [key_topic_value: color = BLUE]

These are the expected outputs of the following commands:

- ``filter add partitions A``:

No output. The filter "A" is added to the "partitions" category.

- ``filters``:

.. code-block::

    --------
    Filters:
    --------

      Topic:

      Partitions:
        - A

- ``topics vv``:

.. code-block::

    - name: Circle
    type: ShapeType
    datawriters:
      - 01.0f.72.e4.86.f3.9b.a0.00.00.00.00|0.0.6.2 [A]
    rate: 12.5391 Hz
    dynamic_type_discovered: true


- ``filter add partitions B``:

No output. The filter "B" is added to the "partitions" category.

- ``filters``:

.. code-block::

    --------
    Filters:
    --------

      Topic:

      Partitions:
        - A
        - B

- ``topics vv``:

.. code-block::

    - name: Circle
      type: ShapeType
      datawriters:
        - 01.0f.72.e4.86.f3.9b.a0.00.00.00.00|0.0.6.2 [A]
      rate: 12.5391 Hz
      dynamic_type_discovered: true
    - name: Square
      type: ShapeType
      datawriters:
        - 01.0f.72.e4.86.f3.9b.a0.00.00.00.00|0.0.7.2 [B|C]
      rate: 12.5391 Hz
      dynamic_type_discovered: true

- ``f set topic Circle "color = 'BLUE'"``

No output. The filter "color = 'BLUE'" is added to the topic filter of the topic "Circle".

- ``f set topic Square "color = 'BLUE'"``

No output. The filter "color = 'BLUE'" is added to the topic filter of the topic "Square".

- ``filters``

.. code-block::

    --------
    Filters:
    --------

      Topic:
        Circle: "color = 'BLUE'"
        Square: "color = 'BLUE'"

      Partitions:
        - A
        - B

- ``echo all``

Prints only the information of the topic Square
(the topic Circle is filtered out because its key value "color" is "RED"):

.. code-block::

    ---

    topic: Square [ShapeType]
    writer: 01.0f.9f.6d.b8.ca.6e.52.00.00.00.00|0.0.3.2
    partitions: ""
    timestamp: 2026/01/16 12:03:14
    data:
    ---
    {
        "color": "BLUE",
        "shapesize": 30,
        "x": 81,
        "y": 35
    }
    ---

    topic: Square [ShapeType]
    writer: 01.0f.9f.6d.b8.ca.6e.52.00.00.00.00|0.0.3.2
    partitions: ""
    timestamp: 2026/01/16 12:03:14
    data:
    ---
    {
        "color": "BLUE",
        "shapesize": 30,
        "x": 87,
        "y": 37
    }
    ---

- ``filter remove partitions B``:

No output. The filter "B" is removed from the "partitions" category.

- ``topics vv``:

.. code-block::

    - name: Circle
    type: ShapeType
    datawriters:
      - 01.0f.72.e4.86.f3.9b.a0.00.00.00.00|0.0.6.2 [A]
    rate: 12.5391 Hz
    dynamic_type_discovered: true

- ``filters``:

.. code-block::

    Filter lists (1)

    partitions (1):
      - A

- ``filter clear``:

- ``filters``:

.. code-block::

    --------
    Filters:
    --------

      Topic:

      Partitions:
