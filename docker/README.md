# eProsima Fast DDS Spy Docker image

eProsima no longer distributes a standalone Fast DDS Spy Docker image.
The only distributed image is the *Fast DDS Suite*, which ships Fast DDS Spy together with the rest of eProsima's tools and libraries.
Download it from [eProsima's Downloads Page](https://www.eprosima.com/index.php/downloads-all), load it, and run `fastddsspy` from it:

```sh
docker load -i "ubuntu-fastdds-suite_<fastdds-version>.tar"

docker run -it \
    --net=host \
    --ipc=host \
    --privileged \
    -v /<fastddsspy_ws>/FASTDDSSPY_CONFIGURATION.yaml:/root/FASTDDSSPY_CONFIGURATION.yaml \
    ubuntu-fastdds-suite:<fastdds-version> fastddsspy
```

See the [Docker Image section](https://fast-dds-spy.readthedocs.io/en/latest/rst/installation/docker.html) of the documentation for the full instructions.

---

## Building a local image

The `Dockerfile` next to this file builds a Fast DDS Spy image locally. This image is not distributed.

```sh
# Build (from the directory where the Dockerfile is)
docker build --rm -t fastddsspy:latest -f Dockerfile .

# Run
docker run --rm -it --net=host --ipc=host fastddsspy:latest
```

Two example configurations are available inside the container under `/fastddsspy/resources/`:

```bash
fastddsspy --config-path /fastddsspy/resources/simple_configuration.yaml
fastddsspy --config-path /fastddsspy/resources/complete_configuration.yaml
```

`--net=host` lets Fast DDS Spy reach participants on other devices, and `--ipc=host` lets it use Shared Memory Transport with participants on the same host.
If local participants cannot connect, run them as `root`, match the container user name to theirs, or set `transport: udp` in the Fast DDS Spy configuration file.

---

For the configuration options and the available commands, see the
[Fast DDS Spy documentation](https://fast-dds-spy.readthedocs.io/en/latest/).
