##!/bin/bash
#
#echo
#echo "=============================================="
#echo "      MonteCarlo-POC HPC Environment"
#echo "=============================================="
#echo
#
##--------------------------------------------------
## Initialize Spack
##--------------------------------------------------
#
#source /home/apps/SPACK/spack/share/spack/setup-env.sh
#
##--------------------------------------------------
## Load Compiler
##--------------------------------------------------
#
#module load compiler/gcc/12.4
#
##--------------------------------------------------
## Load NVIDIA HPC SDK
##--------------------------------------------------
#
#spack load nvhpc@24.3
#
##--------------------------------------------------
## Load CMake
##--------------------------------------------------
#
#spack load cmake@3.31.11
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
#
#if command -v nvidia-smi &> /dev/null
#then
#    nvidia-smi --query-gpu=name,memory.total,driver_version \
#               --format=csv,noheader
#else
#    echo "nvidia-smi not available."
#fi
#
#echo
#echo "=============================================="
#echo "Environment Ready!"
#echo "=============================================="
#echo