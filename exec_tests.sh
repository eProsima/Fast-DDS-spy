#!/bin/bash

#./exec_tests.sh > ORIGINAL_test.txt 2>&1





# Directory containing the files
#PROJECT_NAME="FastDDS-Spy"
PROJECT_NAME="FastDDS-Spy-ORIGINAL"

DIR="/home/danny/eProsima/$PROJECT_NAME/src/fastddsspy/fastddsspy_tool/test/application/test_cases"

echo "FastDDS-Spy test: $PROJECT_NAME"

# Iterate over all files in the directory
for file in "$DIR"/*; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        test_name="${filename::-3}"

        echo
        echo "-- Running test with $test_name ------------------------------------------------------------------------------"

        python3.12 /home/danny/eProsima/$PROJECT_NAME/src/fastddsspy/fastddsspy_tool/test/application/test.py \
          -e /home/danny/eProsima/$PROJECT_NAME/install/fastddsspy_tool/bin/fastddsspy \
          -p /home/danny/eProsima/ShapesDemo/install/ShapesDemo/bin/ShapesDemo \
          -t "$test_name"
    fi
done


# colcon build --packages-select fastddsspy_tool --cmake-args -DBUILD_DOCS_TESTS=ON




