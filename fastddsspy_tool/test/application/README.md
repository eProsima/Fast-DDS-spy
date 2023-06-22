# Fast-DDS-spy tool tests

This module builds a test suite for the Fast DDS Spy.

## Executable [test.py](test.py)

The executable is responsible for running all the tests and verify that the system behaves correctly under the different conditions.

## Executable [test_class.py](test_class.py)

Is the base class that provides the foundation for creating test cases.
Encapsulates various methods and functionalities that enable the definition and execution of individual test cases.
By inheriting from `test_class.TestCase`, you can create custom test case classes that inherit the features and capabilities of the base class.
This inheritance allows you to leverage the provided methods and utilities within your test cases.
However, you also have the flexibility to reimplement or override those methods to tailor them to your specific test cases.

## Add a new test case file

To add a new test case file and define specific conditions to test, follow these steps:

1. Create a new python file inside [test_cases](test_cases/) directory.
2. In the newly created file, create a child class that inherits from `test_class.TestCase`.
3. Customize the class by setting the desired parameters to define the conditions you want to test.

For example, if you want to test:

    ```bash
    fastddsspy --config-path fastddsspy_tool/test/application/configuration/configuration_discovery_time.yaml participants verbose
    ```

With a DDS Publisher running:

    ```bash
    AdvancedConfigurationExample publisher
    ```

You need a class like [this](test_cases/one_shot_participants_verbose_dds.py):

    ```yaml
    name='TopicsVerboseDDSCommand',
    one_shot=True,
    command=[],
    dds=True,
    config='fastddsspy_tool/test/application/configuration/configuration_discovery_time.yaml',
    arguments_dds=[],
    arguments_spy=['--config-path', 'configuration', 'topics', 'verbose'],
    commands_spy=[],
    output="""- name: HelloWorldTopic\n\
                type: HelloWorld\n\
                datawriters:\n\
                    - %%guid%%\n\
                rate: %%rate%%\n\
                dynamic_type_discovered: false\n"""
    ```

If you need to override a specific method for a particular test case, you can do so by providing an implementation within that specific test case class like [here](test_cases/one_shot__help.py) with `valid_output()`.

## TODO

Add a test scenario with a DataReader.
