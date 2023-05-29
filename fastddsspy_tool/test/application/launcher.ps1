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
    # fastddsspy test
    $test_path
)

$test = Start-Process -Passthru -Wait `
    -FilePath $python_path `
    -ArgumentList (
        $test_script,
        "--exe", $tool_path,
        "--test", $test_path) `
    -WindowStyle Hidden

if( $test.ExitCode -ne 0 )
{
    $error_message = "Test: $test_path failed with exit code $($test.ExitCode)."
    throw $error_message
}
