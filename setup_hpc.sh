#!/bin/bash
#
echo
echo "=============================================="
echo "      OpenACC_Test HPC Environment"
echo "=============================================="
echo

#--------------------------------------------------
# Initialize Spack
#--------------------------------------------------

source /home/apps/SPACK/spack/share/spack/setup-env.sh

#--------------------------------------------------
# Load GCC (required runtime)
#--------------------------------------------------

spack load /xaaaxgt

#--------------------------------------------------
# Load NVIDIA HPC SDK
#--------------------------------------------------

spack load nvhpc@24.3

#--------------------------------------------------
# Load CMake
#--------------------------------------------------

spack load cmake@3.31.11

#--------------------------------------------------
# Force GCC 14 runtime first
#--------------------------------------------------

GCC14_ROOT=/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso

export LD_LIBRARY_PATH="$GCC14_ROOT/lib64:$GCC14_ROOT/lib:$LD_LIBRARY_PATH"

#
#echo
#echo "=============================================="
#echo "Environment Information"
#echo "=============================================="
#
#echo
#echo "[GCC]"
#which gcc
#gcc --version
#
#echo
#echo "[NVIDIA HPC SDK]"
#which nvc++
#nvc++ --version
#
#echo
#echo "[CMake]"
#which cmake
#cmake --version
#
#echo
#echo "[GPU]"

if command -v nvidia-smi >/dev/null 2>&1; then
    nvidia-smi --query-gpu=name,memory.total,driver_version \
               --format=csv,noheader
else
    echo "nvidia-smi not available."
fi

#echo
#echo "[libstdc++]"
ldd ~/OpenACC_Test/build/OpenACC_Test | grep libstdc++

echo
echo "=============================================="
echo "Environment Ready!"
echo "=============================================="
echo