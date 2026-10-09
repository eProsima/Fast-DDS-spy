# Fast-DDS-spy tool tests

This module builds a test suite for the Fast DDS Spy.

## Executable [test.py](test.py)

This executable runs all the tests and verifies that the system behaves correctly under the different conditions.

## Executable [test_class.py](test_class.py)

This is the base class for creating test cases.
It contains the methods needed to define and run individual test cases.
By inheriting from `test_class.TestCase`, you can create custom test case classes that use those methods within your test cases.
You can also reimplement or override them to fit a specific test case.

## Add a new test case file

To add a new test case file and define specific conditions to test, follow these steps:

1. Create a new python file inside the [test_cases](test_cases/) directory.
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

To override a method for a particular test case, implement it within that test case class, as [this one](test_cases/one_shot__help.py) does with `valid_output()`.

## TODO

Add a test scenario with a DataReader.
