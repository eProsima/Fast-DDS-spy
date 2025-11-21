.. include:: ../../../exports/alias.include
.. include:: ../../../exports/roles.include

:orphan:

.. _linux_cmake_installation:

CMake installation
==================

This section explains how to compile |espy| with CMake_, either :ref:`locally <local_installation_sl>` or :ref:`globally <global_installation_sl>`.

.. _local_installation_sl:

Local installation
------------------

#.  Create a :code:`Fast-DDS-Spy` directory where to download and build |spy| and its dependencies:

    .. code-block:: bash

        mkdir -p ~/fastdds-spy/src
        mkdir -p ~/fastdds-spy/build
        cd ~/fastdds-spy
        wget https://raw.githubusercontent.com/eProsima/Fast-DDS-Spy/v1.4.0/fastddsspy.repos
        vcs import src < fastddsspy.repos

#.  Compile all dependencies using CMake_.

    * `Foonathan memory <https://github.com/foonathan/memory>`_

        .. code-block:: bash

            cd ~/fastdds-spy
            mkdir build/foonathan_memory_vendor
            cd build/foonathan_memory_vendor
            cmake ~/fastdds-spy/src/foonathan_memory_vendor -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DBUILD_SHARED_LIBS=ON
            cmake --build . --target install

    * `Fast CDR <https://github.com/eProsima/Fast-CDR>`_

        .. code-block:: bash

            cd ~/fastdds-spy
            mkdir build/fastcdr
            cd build/fastcdr
            cmake ~/fastdds-spy/src/fastcdr -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install
            cmake --build . --target install

    * `Fast DDS <https://github.com/eProsima/Fast-DDS>`_

        .. code-block:: bash

            cd ~/fastdds-spy
            mkdir build/fastdds
            cd build/fastdds
            cmake ~/fastdds-spy/src/fastdds -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

    * `Dev Utils <https://github.com/eProsima/dev-utils>`_

        .. code-block:: bash

            # CMake Utils
            cd ~/fastdds-spy
            mkdir build/cmake_utils
            cd build/cmake_utils
            cmake ~/fastdds-spy/src/dev-utils/cmake_utils -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

            # C++ Utils
            cd ~/fastdds-spy
            mkdir build/cpp_utils
            cd build/cpp_utils
            cmake ~/fastdds-spy/src/dev-utils/cpp_utils -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

    * `DDS Pipe <https://github.com/eProsima/DDS-Pipe>`_

        .. code-block:: bash

            # ddspipe_core
            cd ~/fastdds-spy
            mkdir build/ddspipe_core
            cd build/ddspipe_core
            cmake ~/fastdds-spy/ddspipe_core -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

            # ddspipe_yaml
            cd ~/fastdds-spy
            mkdir build/ddspipe_yaml
            cd build/ddspipe_yaml
            cmake ~/fastdds-spy/ddspipe_yaml -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

            # ddspipe_participants
            cd ~/fastdds-spy
            mkdir build/ddspipe_participants
            cd build/ddspipe_participants
            cmake ~/fastdds-spy/ddspipe_participants -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
            cmake --build . --target install

#.  Once all dependencies are installed, install |espy|:

    .. code-block:: bash

        # fastddsspy_participants
        cd ~/fastdds-spy
        mkdir build/fastddsspy_participants
        cd build/fastddsspy_participants
        cmake ~/fastdds-spy/src/fastddsspy_participants -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
        cmake --build . --target install

        # fastddsspy_yaml
        cd ~/fastdds-spy
        mkdir build/fastddsspy_yaml
        cd build/fastddsspy_yaml
        cmake ~/fastdds-spy/src/fastddsspy_yaml -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
        cmake --build . --target install

        # fastddsspy_tool
        cd ~/fastdds-spy
        mkdir build/fastddsspy_tool
        cd build/fastddsspy_tool
        cmake ~/fastdds-spy/src/fastddsspy_tool -DCMAKE_INSTALL_PREFIX=~/fastdds-spy/install -DCMAKE_PREFIX_PATH=~/fastdds-spy/install
        cmake --build . --target install

.. note::

    By default, |espy| does not compile tests.
    However, they can be activated by downloading and installing `Gtest <https://github.com/google/googletest>`_
    and building with CMake option ``-DBUILD_TESTS=ON``.


.. _global_installation_sl:

Global installation
-------------------

To install |espy| system-wide instead of locally, remove all the flags that appear in the configuration steps of :code:`Fast-CDR`, :code:`Fast-DDS`, :code:`Dev-Utils`, :code:`DDS-Pipe`, and :code:`fastdds-spy`, and change the first in the configuration step of :code:`foonathan_memory_vendor` to the following:

.. code-block:: bash

    -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_SHARED_LIBS=ON


.. External links

.. _CMake: https://cmake.org
