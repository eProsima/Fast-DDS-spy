.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_commands_input:

.. _user_manual_commands_input_verbose:

Verbose
=======

:code:`verbose` is a common argument for most of the :ref:`Commands <user_manual_commands>` available.
This argument generate more complete and complex output data for the query executed by the command.
In order to see verbose information, add one of the key-words right after the command and its required arguments.

Key-words
---------

These are the key-words recognize as this argument:
``verbose`` ``v`` ``-v`` ``--v`` ``V``.

.. _user_manual_commands_input_all:

All
===

Some commands support an ``all`` argument.

Key-words
---------

These are the key-words recognize as this argument:
``all`` ``a`` ``-a`` ``--a`` ``A``.

.. _user_manual_commands_input_guid:

Guid
====

:term:`Guid` identifies a DDS entity by a unique Id.

Format
------

The format for the Guid is a string of 12 hexadecimal numbers separated by ``.``, the :term:`Guid Prefix`,
followed by ``|`` and then 4 more hexadecimal values representing the :term:`Entity Id`.
e.g. ``01.0f.22.ba.3b.47.ab.3c.00.00.00.00|00.00.01.c1``.
