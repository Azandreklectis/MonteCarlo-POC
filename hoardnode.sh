#!/bin/bash
#SBATCH --job-name=hoardnode.sh
#SBATCH --partition=gpu
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --gres=gpu:1
#SBATCH --time=08:00:00
#SBATCH --output=hoardnode.sh.%j.out
#SBATCH --error=hoardnode.sh.%j.err

source setup_hpc.sh

cd ~/MonteCarlo-POC

echo "Node: $(hostname)"
echo "Job : $SLURM_JOB_ID"

sleep 8h