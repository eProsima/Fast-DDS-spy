# eProsima Fast DDS Spy docs

This package generates the Fast DDS Spy documentation.
The online documentation is available [here](https://fast-dds-spy.readthedocs.io/en/latest/), hosted on
[readthedocs](https://readthedocs.org/).
This package is powered by [sphinx](https://www.sphinx-doc.org/en/master/).

---

## Documentation generation

### Dependencies

Install the following dependencies before building the documentation:

```bash
sudo apt update
sudo apt install -y \
    doxygen \
    python3 \
    python3-pip \
    python3-venv \
    python3-sphinxcontrib.spelling \
    imagemagick
pip3 install -U -r src/fastddsspy/docs/requirements.txt
```

### Build documentation

To install this package independently, use the following command:

```bash
colcon build --packages-select fastddsspy_docs
```

To compile and run the package tests, enable the CMake option `BUILD_DOCS_TESTS`.

```bash
colcon build --packages-select fastddsspy_docs --cmake-args -DBUILD_DOCS_TESTS=ON
colcon test --packages-select fastddsspy_docs --event-handler console_direct+
```

---

## Library documentation

This documentation is the user manual for installing and working with Fast DDS Spy.
For the repository structure, design decisions, development guidelines, etc.,
see each package's own documentation and the source code, which is commented in Doxygen format.
The `.dev` directory (if it exists) contains information for developers.
