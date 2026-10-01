.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _cmake_options:

#############
CMake options
#############

|espy| provides several CMake options to change its behavior and configuration.
With these options, the developer can enable or disable certain |espy| settings by setting them to ``ON``/``OFF`` when running CMake, or set the required path to certain dependencies.

.. warning::
    These options are only for developers who installed |espy| following the compilation steps described in :ref:`installation_sources_linux`.

.. list-table::
    :header-rows: 1

    *   - Option
        - Description
        - Possible values
        - Default
    *   - :class:`CMAKE_BUILD_TYPE`
        - CMake optimization build type.
        - ``Release`` |br|
          ``Debug``
        - ``Release``
    *   - :class:`BUILD_DOCS`
        - Build the |espy| documentation. |br|
        - ``OFF`` |br|
          ``ON``
        - ``OFF``
    *   - :class:`BUILD_TESTS`
        - Build the |espy| tools and documentation |br|
          tests.
        - ``OFF`` |br|
          ``ON``
        - ``OFF``
    *   - :class:`LOG_INFO`
        - Activate |espy| logs. It is |br|
          set to ``ON`` if :class:`CMAKE_BUILD_TYPE` is set |br|
          to ``Debug``.
        - ``OFF`` |br|
          ``ON``
        - ``ON`` if ``Debug`` |br|
          ``OFF`` otherwise
    *   - :class:`ASAN_BUILD`
        - Activate address sanitizer build.
        - ``OFF`` |br|
          ``ON``
        - ``OFF``
    *   - :class:`TSAN_BUILD`
        - Activate thread sanitizer build.
        - ``OFF`` |br|
          ``ON``
        - ``OFF``
