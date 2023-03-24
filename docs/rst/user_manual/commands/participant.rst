.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _user_manual_format:

####################
Participants Command
####################

**Participants** is a command that retrieves information of the :term:`DomainParticipants <DomainParticipant>` currently active in the network.

Arguments
=========


Output Format
=============

The participant information is retrieved in 2 formats depending on the verbose option.

.. tabs::

    .. tab:: simple

        .. code-block:: yaml



    .. tab:: verbose

        .. code-block:: python

            # Create a new Computing Node
            node = ComputingNode("My_Computing_Node")

            def process_solution():
                JobSolutionDataType solution;
                # Do some code that calculates the solution
                return solution

            # Wait for 1 task from any client and answer it with process_solution callback
            node.process_job(callback=process_solution)


This is the information retrieved by the command :ref:`user_manual_command_participants`.

.. code-block:: yaml

Example
=======

Let's assume we have a DDS network where 2 ShapesDemo applications are running.

This would be the expected output for the command `participants`:

.. code-block::

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.79.47.8d.f0.00.00.00.00|0.0.1.c1

This would be the expected output for the command `participants verbose`:

.. code-block::

    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
      datawriters:
        - Square [ShapeType] (2)
        - Circle [ShapeType] (1)
    - name: Fast DDS ShapesDemo Participant
      guid: 01.0f.22.ba.79.47.8d.f0.00.00.00.00|0.0.1.c1
      datawriters:
        - Triangle [ShapeType] (1)
      datareaders:
        - Square [ShapeType] (1)

This would be the expected output for the command `participants 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1`:

.. code-block::

    name: Fast DDS ShapesDemo Participant
    guid: 01.0f.22.ba.3b.47.ab.3c.00.00.00.00|0.0.1.c1
    datawriters:
    - Square [ShapeType] (2)
    - Circle [ShapeType] (1)
