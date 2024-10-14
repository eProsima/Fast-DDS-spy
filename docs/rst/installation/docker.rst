.. include:: ../exports/alias.include
.. include:: ../exports/roles.include

.. _docker:

##########################
Docker Image (recommended)
##########################

eProsima distributes a Docker image of |spy| with Ubuntu 22.04 as base image.
This image launches an instance of |spy| that is configured using a *YAML* configuration file provided by the user
and shared with the Docker container.
The steps to run |spy| in a Docker container are explained below.

#.  Download the compressed Docker image in ``.tar`` format from the
    `eProsima Downloads website <https://www.eprosima.com/index.php/downloads-all>`_.
    It is strongly recommended to download the image corresponding to the latest version of |spy|.

    |br|

#.  Extract the image by executing the following command:

    .. code-block:: bash

        docker load -i ubuntu-fastddsspy:<version>.tar

    where ``version`` is the downloaded version of |spy|.

    |br|

#.  Build a |spy| configuration YAML file on the local machine.
    This will be the |spy| configuration file that runs inside the Docker container.
    Open your preferred text editor and copy a full configuration example into the ``/<fastddsspy>/FASTDDSSPY_CONFIGURATION.yaml`` file,
    where ``fastddsspy`` is the path where to execute the tool.
    To make this accessible from the Docker container we will create a shared volume containing just
    this file. This is explained in next point.

    .. TODO add reference to an already created conf yaml

    |br|

#.  Run the Docker container executing the following command:

    .. code-block:: bash

        docker run -it \
            --net=host \
            --ipc=host \
            --privileged \
            -v /<fastddsspy>/FASTDDSSPY_CONFIGURATION.yaml:/root/FASTDDSSPY_CONFIGURATION.yaml \
            ubuntu-fastddsspy:v1.1.0

    It is important to mention that both the path to the configuration file hosted in the local machine and the one
    created in the Docker container must be absolute paths in order to share just one single file as a shared volume.

    After executing the previous command you should be able to see the initialization traces from the |spy|
    running in the Docker container.
    If you want to terminate the application gracefully, just press ``Ctrl+C`` to stop the execution of |spy|.
