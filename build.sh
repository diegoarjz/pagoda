#!/bin/sh

WORKSPACE=${1}
OS=${2}
COMPILER=${3}

PY_VENV=${WORKSPACE}/buildenv
PAGODA_ROOT=${WORKSPACE}/pagoda
BUILD_ROOT=${WORKSPACE}/build

CONAN_PROFILE_ROOT=${PAGODA_ROOT}/tools/profiles
CONAN_PROFILE=${CONAN_PROFILE_ROOT}/${OS}_${COMPILER}_Release_gh.txt

echo "########################################"
echo "Running build.sh"
echo "    workspace: ${WORKSPACE}"
echo "           os: ${OS}"
echo "     compiler: ${COMPILER}"
echo "         venv: ${PY_VENV}"
echo "  pagoda root: ${PAGODA_ROOT}"
echo " profile root: ${CONAN_PROFILE_ROOT}"
echo "      profile: ${CONAN_PROFILE}"
echo "   build root: ${BUILD_ROOT}"
echo "########################################"

#----------------------------------------
# install dependencies
#----------------------------------------
python3 -m venv ${PY_VENV}
source ${PY_VENV}/bin/activate
pip3 install wheel setuptools numpy
pip3 install -r ${WORKSPACE}/pagoda/requirements.txt

#----------------------------------------
# install conan dependencies
#----------------------------------------
git -C ${PAGODA_ROOT} submodule update --init --recursive
conan install                                     \
  --profile:build=${CONAN_PROFILE} \
  --profile:host=${CONAN_PROFILE}  \
  -of ${BUILD_ROOT}                               \
  ${PAGODA_ROOT}                                  \
  --build=missing

#----------------------------------------
# Build Pagoda
#----------------------------------------
cmake -S${PAGODA_ROOT} --preset conan-release
cmake --build ${BUILD_ROOT} -j 24
