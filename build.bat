@echo off

set WORKSPACE=%1%
set OS=%2%
set COMPILER=%3%

set PY_VENV=%WORKSPACE%\buildenv
set PAGODA_ROOT=%WORKSPACE%\pagoda
set BUILD_ROOT=%WORKSPACE%\build

set CONAN_PROFILE_ROOT=%PAGODA_ROOT%\tools\profiles
set CONAN_PROFILE=%CONAN_PROFILE_ROOT%\%OS%_%COMPILER%_Release_gh.txt

echo "########################################"
echo "Running build.sh"
echo "    workspace: %WORKSPACE%"
echo "           os: %OS%"
echo "     compiler: %COMPILER%"
echo "         venv: %PY_VENV%"
echo "  pagoda root: %PAGODA_ROOT%"
echo " profile root: %CONAN_PROFILE_ROOT%"
echo "      profile: %CONAN_PROFILE%"
echo "   build root: %BUILD_ROOT%"
echo "########################################"

REM ----------------------------------------
REM  install dependencies
REM ----------------------------------------
pip3 install wheel setuptools numpy
pip3 install -r %WORKSPACE%/pagoda/requirements.txt

REM ----------------------------------------
REM  install conan dependencies
REM ----------------------------------------
git -C %PAGODA_ROOT% submodule update --init --recursive
conan install                                     ^
  --profile:build=%CONAN_PROFILE%                 ^
  --profile:host=%CONAN_PROFILE%                  ^
  -of %BUILD_ROOT%                                ^
  %PAGODA_ROOT%                                   ^
  --build=missing

REM ----------------------------------------
REM  Build Pagoda
REM ----------------------------------------
cmake -S %PAGODA_ROOT% --preset conan-default
cmake --build %BUILD_ROOT%  --config Release -j 24
