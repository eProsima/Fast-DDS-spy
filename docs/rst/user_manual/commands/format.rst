.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_commands_input:

.. _user_manual_commands_input_compact:

Compact
=======

The compact argument gives a shorter output format for the information that the command receives.
It shows less metadata and focuses on the data values of the topics.

Key-words
---------

These are the key-words recognized as this argument:
``compact`` ``c`` ``-c`` ``--c`` ``C``.

.. _user_manual_commands_input_verbose:

Verbose
=======

:code:`verbose` is an argument shared by most of the available :ref:`Commands <user_manual_commands>`.
This argument makes the command return more complete and detailed output for its query.
To see verbose information, add one of the key-words right after the command and its required arguments.

Key-words
---------

These are the key-words recognized as this argument:
``verbose`` ``v`` ``-v`` ``--v`` ``V``.

.. _user_manual_commands_input_all:

All
===

Some commands support an ``all`` argument.

Key-words
---------

These are the key-words recognized as this argument:
``all`` ``a`` ``-a`` ``--a`` ``A``.

.. _user_manual_commands_input_guid:

Guid
====

A :term:`Guid` is a unique Id that identifies a DDS entity.

Format
------

A Guid is a string of 12 hexadecimal numbers separated by ``.``, the :term:`Guid Prefix`,
followed by ``|`` and then 4 more hexadecimal values, the :term:`Entity Id`.
For example, ``01.0f.22.ba.3b.47.ab.3c.00.00.00.00|00.00.01.c1``.
