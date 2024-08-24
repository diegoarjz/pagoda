#!/bin/sh

WORKSPACE=${1}
OS=${2}
COMPILER=${3}

PY_VENV=${WORKSPACE}/buildenv
PAGODA_ROOT=${WORKSPACE}/pagoda

PAGODA_ROOT=${WORKSPACE}/pagoda
CONAN_PROFILE_ROOT=${PAGODA_ROOT}/tools/profiles/
CONAN_PROFILE=${CONAN_PROFILE_ROOT}/${OS}_${COMPILER}_Release_gh.txt

echo "########################################"
echo "Running build.sh"
echo "    workspace: ${WORKSPACE}"
echo "           os: ${OS}"
echo "     compiler: ${COMPILER}"
echo "  pagoda root: ${PAGODA_ROOT}"
echo " profile root: ${CONAN_PROFILE_ROOT}"
echo "      profile: ${CONAN_PROFILE}"
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
pushd ${WORKSPACE}/pagoda
git submodule update --init --recursive
conan install \
  --profile:build=${WORKSPACE}/pagoda/${CONAN_PROFILE} \
  --profile:host=${WORKSPACE}/pagoda/${CONAN_PROFILE} \
  -of ${WORKSPACE}/build \
  ${WORKSPACE}/pagoda \
  --build=missing

#----------------------------------------
# Build Pagoda
#----------------------------------------
cmake --preset conan-release
cmake --build ${WORKSPACE}/build -j 24
popd
