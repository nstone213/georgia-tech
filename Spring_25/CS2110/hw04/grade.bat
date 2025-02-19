@echo off

set imageName="gtcs2110/cs2110docker-c:stable"

docker container ps > nul 2>&1
if "%errorlevel%" neq "0" (
    echo ERROR: Docker not found. Ensure that Docker is installed and is running before running this script. Refer to the CS 2110 Docker Guide.
    exit /b 1
)

echo Attempting to pull down most recent image of %imageName%...
docker pull %imageName%
if "%errorlevel%" neq "0" (
    echo ERROR: Unable to pull down the most recent image of %imageName%
    exit /b 1
)

set curDir=%cd%

del report.html

docker run -e "TEST_ROOT_DIR=/cs2110/host/" --rm -i -v "%curDir%:/cs2110/host/" --entrypoint "python" %imageName% "tests/autograder.py" "%1"

if %ERRORLEVEL% NEQ 125 (
	start "" "report.html"
)