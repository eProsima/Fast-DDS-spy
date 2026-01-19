.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_filter:

######
Filter
######

**Filter** is a command that adds filters to restrict the spied information based on user-defined criteria.

Key-words
=========

These are the key-words recognize as this command:
``filter`` ``filters`` ``f`` ``F``.

Categories
==========

Defines a category filter used to restrict the spied information.
Each category can include one or more filter strings,
and information is printed if it matches any of the strings within the category.

The current categories supported by the **filter** command are:

- ``partitions``

Arguments
=========

**Filter** command supports from 0 to 3 arguments:

*No argument*
-------------

When no arguments are given to this command, the information shown is a **list** with all the list of filters
added during runtime.

The information shown is divided into lists, one for each ``category`` added at runtime.
For each list, the added filters are displayed.

The output format is as follows: :ref:`user_manual_command_filter_output`.

*1 argument:* `<clear/remove>`
------------------------------

- ``clear``: This argument **clear** all the list of categories added to the filters.
- ``remove``: This argument **delete** all the list of categories added to the filters.

*2 argument:* `<clear/remove> <category>`
-----------------------------------------

- ``clear``: This argument **clear** the list ``category`` added to the filters.
- ``remove``: This argument **delete** the list ``category`` from the filters.

*3 argument:* `<add/remove> partitions <filter_str>`
----------------------------------------------------

- ``add``: This argument **add** ``filter_str`` to **partitions** filter list.
- ``remove``: This argument **delete** ``filter_str`` from **partitions** filter list.

*4 argument:* `<set/remove> <category> <category_name> <filter_str>`
--------------------------------------------------------------------

- ``set``: This argument **set** ``filter_str`` to ``category`` ``category_name`` filter list.
- ``remove``: This argument **delete** ``filter_str`` from ``category`` ``category_name`` filter list.

.. _user_manual_command_filter_output:

Output Format
=============

The filters information is retrieved with the following format:

.. code-block:: yaml

  Filter lists (1)

    category_1 (2):
      - filter_str_1
      - filter_str_2

    category_2 (2):
      - filter_str_1

Example
=======

Let's assume we have a DDS network where a ShapesDemo applications is running with
the following two DataWriters:

- Circle (partition A) [key_topic_value: color = RED]
- Square (partitions B and C) [key_topic_value: color = BLUE]

This would be the expected output for the following commands:

- ``filter add partitions A``:

Nothing, the filter "A" is added to the category "partitions".

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

Nothing, the filter "B" is added to the category "partitions".

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

Nothing, the filter "color = 'BLUE'" is added to the category "topic" in the category_name "Circle".

- ``f set topic Square "color = 'BLUE'"``

Nothing, the filter "color = 'BLUE'" is added to the category "topic" in the category_name "Square".

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

Prints only the information of the topic Square,
(the topic Circle is filtered because the key value "color" is "RED")

.. code_block::

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

Nothing, the filter "B" is removed from the category "partitions".

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
