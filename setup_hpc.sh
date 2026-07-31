#!/bin/bash

echo
echo "=============================================="
echo "      MonteCarlo-POC HPC Environment"
echo "=============================================="
echo

#--------------------------------------------------
# Initialize Spack
#--------------------------------------------------

source /home/apps/SPACK/spack/share/spack/setup-env.sh

#--------------------------------------------------
# Load GCC Runtime
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
# GCC Runtime Libraries
#--------------------------------------------------

GCC14_ROOT=/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso

export LD_LIBRARY_PATH="$GCC14_ROOT/lib64:$GCC14_ROOT/lib:$LD_LIBRARY_PATH"

echo
echo "Environment Loaded"

echo
echo "Compiler:"
which nvc++

echo
echo "CMake:"
which cmake

echo
echo "GCC:"
which gcc

echo
echo "=============================================="
echo "Ready!"
echo "=============================================="