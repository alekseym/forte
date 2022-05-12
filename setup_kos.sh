#!/bin/bash
echo "----------------------------------------------------------------------------"
echo " Automatically set up development environment for POSIX-platform"
echo "----------------------------------------------------------------------------"
echo ""
echo " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
echo " ASN1-encoding, ..."
echo ""
echo " To include tests set directories for boost-test-framework and "
echo " set FORTE_TESTS-option to 'ON'"
echo ""
echo "----------------------------------------------------------------------------"



export LANG=C
export TARGET="arm-kos"
export PKG_CONFIG=""
export SDK_PREFIX="/opt/KasperskyOS-Community-Edition-1.0.1.4"
BUILD_SIM_TARGET="y"
export PATH="$SDK_PREFIX/toolchain/bin:$PATH"

#cmake -G "Unix Makefiles" \
#      -D CMAKE_BUILD_TYPE:STRING=Debug \
#      -D CMAKE_INSTALL_PREFIX:STRING=$BUILD/../install \
#      -D CMAKE_TOOLCHAIN_FILE=$SDK_PREFIX/toolchain/share/toolchain-arm-kos.cmake





export forte_bin_dir="bin/kos"

#set to boost-include directory
export forte_boost_test_inc_dirs=""
#set to boost-library directory
export forte_boost_test_lib_dirs=""

if [ ! -d "$forte_bin_dir" ]; then
  mkdir -p "$forte_bin_dir"
fi

if [ -d "$forte_bin_dir" ]; then
  
  echo "For building forte go to $forte_bin_dir and execute \"make\""
  echo "forte can be found at ${forte_bin_dir}/src"
  echo "forte_tests can be found at ${forte_bin_dir}/tests"
  
  
  cd "./$forte_bin_dir"
  
  cmake -G "Unix Makefiles" -DFORTE_ARCHITECTURE=kos -DFORTE_COM_ETH=ON -DFORTE_COM_FBDK=ON -DFORTE_COM_LOCAL=ON -DFORTE_TESTS=OFF -DFORTE_TESTS_INC_DIRS=${forte_boost_test_inc_dirs} -DFORTE_TESTS_LINK_DIRS=${forte_boost_test_inc_dirs} -DFORTE_MODULE_CONVERT=ON -DFORTE_MODULE_IEC61131=ON -DFORTE_MODULE_UTILS=ON ../../ \
   -D CMAKE_BUILD_TYPE:STRING=Debug \
   -D CMAKE_INSTALL_PREFIX:STRING=$BUILD/../install \
   -D CMAKE_TOOLCHAIN_FILE=$SDK_PREFIX/toolchain/share/toolchain-arm-kos.cmake
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
