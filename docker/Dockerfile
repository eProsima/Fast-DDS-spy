#########################################################################################
# Fast DDS Spy Demos Dockerfile
#########################################################################################

FROM ubuntu:jammy

# Avoids using interactions during building
ENV DEBIAN_FRONTEND=noninteractive

# Use a bash shell so it is possigle to run things like `source` (required for colcon builds)
SHELL ["/bin/bash", "-c"]

# Avoid interactuation with installation of some package that needs the locale.
ENV TZ=Europe/Madrid
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Download Fast DDS Spy dependencies
RUN apt update && \
    apt install -y \
        cmake \
        g++ \
        pip \
        wget \
        git \
        libasio-dev \
        libtinyxml2-dev \
        libssl-dev \
        libyaml-cpp-dev \
    && \
    pip3 install -U colcon-common-extensions vcstool

# Download eProsima dependencies
WORKDIR /fastddsspy
RUN wget https://raw.githubusercontent.com/eProsima/Fast-DDS-Spy/v1.4.0/fastddsspy.repos && \
    mkdir src && \
    vcs import src < fastddsspy.repos

# Build Fast DDS Spy
RUN colcon build

# Add Fast DDS Spy configuration files
COPY resources resources

# Source built workspace
RUN echo "source /fastddsspy/install/setup.bash" >> ~/.bashrc

# Restore working directory to default
WORKDIR /
