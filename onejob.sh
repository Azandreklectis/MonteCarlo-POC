#!/bin/bash
#SBATCH --job-name=openacc
#SBATCH --partition=gpu
#SBATCH --gres=gpu:1
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=48:00:00
#SBATCH --output=job.%j.out
#SBATCH --error=job.%j.err
#SBATCH --reservation=IUAC

set -e

source setup_hpc.sh

export LD_LIBRARY_PATH=/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso/lib64:/home/apps/SPACK/spack/opt/spack/linux-almalinux8-cascadelake/gcc-13.2.0/gcc-14.2.0-tzadgwdvjenkrgflefidpb7lz636pcso/lib:$LD_LIBRARY_PATH

export ACC_DEVICE_TYPE=nvidia
export NVCOMPILER_ACC_NOTIFY=3

echo "======================================================"
echo "                 SLURM DEBUG INFO"
echo "======================================================"

echo "Hostname:"
hostname

echo
echo "SLURM_JOB_ID = $SLURM_JOB_ID"

echo
echo "Current directory:"
pwd

echo
echo "SLURM_SUBMIT_DIR:"
echo "$SLURM_SUBMIT_DIR"

echo
echo "HOME:"
echo "$HOME"

echo
echo "Scratch project:"
ls -ld /scratch/hpctw41/MonteCarlo-POC || true

echo
echo "Home project:"
ls -ld /home/hpctw41/MonteCarlo-POC || true

echo
echo "Scratch executable:"
ls -lh /scratch/hpctw41/MonteCarlo-POC/build/OpenACC_Test || true
md5sum /scratch/hpctw41/MonteCarlo-POC/build/OpenACC_Test || true

echo
echo "Home executable:"
ls -lh /home/hpctw41/MonteCarlo-POC/build/OpenACC_Test
md5sum /home/hpctw41/MonteCarlo-POC/build/OpenACC_Test

echo
echo "Executable timestamps:"
stat /scratch/hpctw41/MonteCarlo-POC/build/OpenACC_Test || true
stat /home/hpctw41/MonteCarlo-POC/build/OpenACC_Test

echo
echo "DatasetParameters checksum:"
md5sum /home/hpctw41/MonteCarlo-POC/Core/DatasetParameters.h
md5sum /scratch/hpctw41/MonteCarlo-POC/Core/DatasetParameters.h || true

echo
echo "Output directory BEFORE execution:"
ls -la /home/hpctw41/MonteCarlo-POC/build || true
ls -la /scratch/hpctw41/MonteCarlo-POC/build || true

echo
echo "======================================================"
echo "RUNNING EXECUTABLE"
echo "======================================================"

cd /scratch/hpctw41/MonteCarlo-POC/build

echo
echo "PWD after cd:"
pwd

echo
echo "Running:"
readlink -f ./OpenACC_Test

ldd ./OpenACC_Test | grep libstdc++

./OpenACC_Test

echo
echo "======================================================"
echo "PROGRAM FINISHED"
echo "======================================================"

echo
echo "Output directory AFTER execution:"
ls -la /home/hpctw41/MonteCarlo-POC/build || true
ls -la /scratch/hpctw41/MonteCarlo-POC/build || true