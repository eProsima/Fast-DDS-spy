.. include:: ../../../exports/alias.include
.. include:: ../../../exports/roles.include

:orphan:

.. _windows_cmake_installation:

CMake installation
==================

This section explains how to compile |espy| with CMake_, either :ref:`locally <windows_sources_local_installation>` or :ref:`globally <windows_sources_global_installation>`.

.. _windows_sources_local_installation:

Local installation
------------------

#.  Open a command prompt, and create a :code:`fastdds-spy` directory where to download and build |espy| and its dependencies:

    .. code-block:: bash

        mkdir <path\to\user\workspace>\fastdds-spy
        mkdir <path\to\user\workspace>\fastdds-spy\src
        mkdir <path\to\user\workspace>\fastdds-spy\build
        cd <path\to\user\workspace>\fastdds-spy
        wget https://raw.githubusercontent.com/eProsima/Fast-DDS-Spy/v1.3.0/fastddsspy.repos
        vcs import src < fastddsspy.repos

#.  Compile all dependencies using CMake_.

    *  `Foonathan memory <https://github.com/foonathan/memory>`_

        .. code-block:: bash

            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\foonathan_memory_vendor
            cd build\foonathan_memory_vendor
            cmake <path\to\user\workspace>\fastdds-spy\src\foonathan_memory_vendor -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
                -DBUILD_SHARED_LIBS=ON
            cmake --build . --config Release --target install

    *  `Fast CDR <https://github.com/eProsima/Fast-CDR>`_

        .. code-block:: bash

            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\fastcdr
            cd build\fastcdr
            cmake <path\to\user\workspace>\fastdds-spy\src\fastcdr -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --config Release --target install

    *  `Fast DDS <https://github.com/eProsima/Fast-DDS>`_

        .. code-block:: bash

            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\fastdds
            cd build\fastdds
            cmake <path\to\user\workspace>\fastdds-spy\src\fastdds -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
                -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --config Release --target install

    * `Dev Utils <https://github.com/eProsima/dev-utils>`_

        .. code-block:: bash

            # CMake Utils
            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\cmake_utils
            cd build\cmake_utils
            cmake <path\to\user\workspace>\fastdds-spy\src\dev-utils\cmake_utils -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
                -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --config Release --target install

            # C++ Utils
            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\cpp_utils
            cd build\cpp_utils
            cmake <path\to\user\workspace>\fastdds-spy\src\dev-utils\cpp_utils -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
                -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --config Release --target install

    * `DDS Pipe <https://github.com/eProsima/DDS-Pipe>`_

        .. code-block:: bash

            # ddspipe_core
            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\ddspipe_core
            cd build\ddspipe_core
            cmake cd <path\to\user\workspace>\fastdds-spy\src\ddspipe\ddspipe_core -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --target install

            # ddspipe_yaml
            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\ddspipe_yaml
            cd build\ddspipe_yaml
            cmake <path\to\user\workspace>\fastdds-spy\src\ddspipe\ddspipe_yaml -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --target install

            # ddspipe_participants
            cd <path\to\user\workspace>\fastdds-spy
            mkdir build\ddspipe_participants
            cd build\ddspipe_participants
            cmake <path\to\user\workspace>\fastdds-spy\src\ddspipe\ddspipe_participants -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
            cmake --build . --target install

#.  Once all dependencies are installed, install |espy|:

    .. code-block:: bash

        # fastddsspy_participants
        cd <path\to\user\workspace>\fastdds-spy
        mkdir build\fastddsspy_participants
        cd build\fastddsspy_participants
        cmake <path\to\user\workspace>\fastdds-spy\src\fastddsspy\fastddsspy_participants ^
            -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
        cmake --build . --config Release --target install

        # fastddsspy_yaml
        cd <path\to\user\workspace>\fastdds-spy
        mkdir build\fastddsspy_yaml
        cd build\fastddsspy_yaml
        cmake <path\to\user\workspace>\fastdds-spy\src\fastddsspy\fastddsspy_yaml -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
            -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
        cmake --build . --config Release --target install

        # fastddsspy_tool
        cd <path\to\user\workspace>\fastdds-spy
        mkdir build\fastddsspy
        cd build\fastddsspy
        cmake <path\to\user\workspace>\fastdds-spy\src\fastddsspy\fastddsspy_tool -DCMAKE_INSTALL_PREFIX=<path\to\user\workspace>\fastdds-spy\install ^
            -DCMAKE_PREFIX_PATH=<path\to\user\workspace>\fastdds-spy\install
        cmake --build . --config Release --target install


.. note::

    By default, |espy| does not compile tests.
    However, they can be activated by downloading and installing `Gtest <https://github.com/google/googletest>`_
    and building with CMake option ``-DBUILD_TESTS=ON``.


.. _windows_sources_global_installation:

Global installation
-------------------

To install |espy| system-wide instead of locally, remove all the flags that appear in the configuration steps of :code:`Fast-CDR`, :code:`Fast-DDS`, :code:`Dev-Utils`, :code:`DDS-Pipe`, and :code:`fastdds-spy`


.. External links

.. _CMake: https://cmake.org
