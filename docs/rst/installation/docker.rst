.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _docker:

############
Docker Image
############

eProsima no longer distributes a standalone Docker image of |spy|.
Instead, |spy| is included in the *Fast DDS Suite* Docker image, which bundles eProsima's tools and libraries on
an Ubuntu base and can be downloaded from `eProsima's Downloads Page <https://www.eprosima.com/index.php/downloads-all>`__.
Inside the container, |spy| is configured with a *YAML* configuration file provided by the user and shared with
the container.
The steps to run |spy| in a Docker container are explained below.

#.  Download the compressed Docker image in ``.tar`` format from the
    `eProsima's Downloads Page <https://www.eprosima.com/index.php/downloads-all>`__ and load it into your local Docker by running the
    following command in a terminal:

    .. code-block:: bash

        docker load -i "ubuntu-fastdds-suite_<fastdds-version>.tar"

    where ``fastdds-version`` is the downloaded version of |efastdds|.

    |br|

#.  Create a |spy| configuration YAML file on the local machine.
    This is the configuration file that |spy| uses inside the Docker container.
    Open your preferred text editor and copy the
    :ref:`General Example <user_manual_configuration_default>` into the
    ``/<fastddsspy_ws>/FASTDDSSPY_CONFIGURATION.yaml`` file, where ``fastddsspy_ws`` is the path of the
    configuration file.
    To make this file accessible from the Docker container, the next step creates a shared volume that contains
    only this file.

    |br|

#.  Run the Docker container by executing the following command:

    .. code-block:: bash

        docker run -it \
            --net=host \
            --ipc=host \
            --privileged \
            -v /<fastddsspy_ws>/FASTDDSSPY_CONFIGURATION.yaml:/root/FASTDDSSPY_CONFIGURATION.yaml \
            ubuntu-fastdds-suite:<fastdds-version> fastddsspy

    Both the path to the configuration file on the local machine and the one created in the Docker container
    must be absolute paths, so that a single file is shared as a volume.

    After executing the previous command, you should see the initialization traces of |spy|
    running in the Docker container.
    To terminate the application gracefully, press ``Ctrl+C`` to stop the execution of |spy|.
