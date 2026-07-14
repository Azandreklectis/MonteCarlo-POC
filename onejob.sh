#!/bin/bash
#SBATCH --job-name=openacc
#SBATCH --partition=gpu
#SBATCH --gres=gpu:1
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --output=job.%j.out
#SBATCH --error=job.%j.err
#SBATCH --reservation=iuac

source setup_hpc.sh

export LD_LIBRARY_PATH=/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso/lib64:/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso/lib:$LD_LIBRARY_PATH

#
#echo
#echo "===== LD_LIBRARY_PATH ====="
#echo "$LD_LIBRARY_PATH"
#
#echo "meeeoooooooowwwwwwww"
#
#echo
#echo "===== OpenACC_Test dependencies ====="
ldd ~/OpenACC_Test/build/OpenACC_Test | grep libstdc++

cd ~/OpenACC_Test/build

export NVCOMPILER_ACC_NOTIFY=3

./OpenACC_Test