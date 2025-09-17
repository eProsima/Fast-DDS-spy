.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_command_filter:

######
Filter
######

**Filter** is a command that adds filters

retrieves information of the :term:`Topics <Topic>` with at least one endpoint currently active in the network.

Key-words
=========

These are the key-words recognize as this command:
``filter`` ``filters`` ``f`` ``F``.

Arguments
=========

**Filter** command supports from 0 to 3 arguments:

*No argument*
-------------

When no arguments are given to this command, the information shown is a **list** with all the list of filters
added during runtime.

The information shown is divided into lists, one for each category added at runtime.
For each list, the added filters are displayed.

The output format is as follows: :ref:`user_manual_command_filter_output`.

*1 argument:* `<clear/remove>`
------------------------------

- clear: This argument clear all the list of categories added to the filters.
- remove: This argument delete all the list of categories added to the filters.

*2 argument:* `<clear/remove> <category>`
-----------------------------------------

- clear: This argument clear the list "category" added to the filters.
- remove: This argument delete the list "category" from the filters.

*3 argument:* `<clear/remove> <category> <filter_str>`
------------------------------------------------------

- set: This argument create the `category` filter list with `filter_str` as first element.
- add: This argument add `filter_str` to `category` filter list.
- remove: This argument delete `filter_str` from `category` filter list.

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
the following 2 DataWriters:
- Circle (partition A)
- Square (partitions B and C).

This would be the expected output for the following commands:

- ``filter set partitions A``:

Nothing, the category "partitions" is created with filter "A" as first element.

- ``filters``:

.. code-block::

    Filter lists (1)

    partitions (1):
      - A

- ``topics vv``:

.. code-block::

    - name: Circle
    type: ShapeType
    datawriters:
      - 01.0f.72.e4.86.f3.9b.a0.00.00.00.00|0.0.6.2 [A]
    rate: 12.5391 Hz
    dynamic_type_discovered: true


- ``filters add partitions B``:

Nothing, the filter "B" is added to the category "partitions".

- ``filters``:

.. code-block::

    Filter lists (1)

    partitions (2):
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

- ``filters remove partitions B``:

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

    Filter lists (0)
