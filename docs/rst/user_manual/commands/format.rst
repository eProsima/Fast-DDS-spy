.. include:: ../../exports/alias.include
.. include:: ../../exports/roles.include

.. _user_manual_commands_input:

############
Input format
############

TODO

.. _user_manual_commands_input_verbose:

Verbose
=======

:code:`verbose` is a common argument for most of the :ref:`Commands <user_manual_commands>` available.
This argument generate more complete and complex output data for the query executed by the command.

Key-words
---------

These are the key-words recognize as this argument:
``verbose`` ``v`` ``-v`` ``--v`` ``V``.

.. _user_manual_commands_input_guid:

Guid
====

:term:`Guid` identifies a DDS entity by a unique Id.
This argument is available with :ref:`user_manual_command_participant` and TODO and it retrieves verbose information regarding a unique entity.

Format
------

The format for the Guid is a string of 12 hexadecimal numbers separated by ``.``, the :term:`Guid Prefix`,
followed by ``|`` and then 4 more hexadecimal values representing the :term:`Entity Id`.
e.g. ``01.0f.22.ba.3b.47.ab.3c.00.00.00.00|00.00.01.c1``.
