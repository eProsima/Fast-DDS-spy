# Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

Param(
    [Parameter(Position=0, Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf -IsValid })]
    [String]
    # python3 binary
    $python_path,

    [Parameter(Position=1, Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf -IsValid })]
    [String]
    # python script that keeps the  testing
    $test_script,

    [Parameter(Position=2, Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf -IsValid })]
    [String]
    # fastddsspy creation binary full qualified path
    $tool_path,

    [Parameter(Position=3, Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf -IsValid })]
    [String]
    # fastdds publisher creation binary full qualified path
    $pub_path,

    [Parameter(Position=4, Mandatory=$true)]
    [ValidateScript({Test-Path $_ -PathType Leaf -IsValid })]
    [String]
    # fastddsspy test
    $test_path
)

$test = Start-Process -Passthru -Wait `
    -FilePath $python_path `
    -ArgumentList (
        $test_script,
        "--exe", $tool_path,
        "--pub", $pub_path,
        "--test", $test_path) `
    -WindowStyle Hidden

if( $test.ExitCode -ne 0 )
{
    $error_message = "Test: $test_path failed with exit code $($test.ExitCode)."
    throw $error_message
}
