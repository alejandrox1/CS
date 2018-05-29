#!/bin/bash
#SBATCH -A my_project
#SBATCH -p partition
#SBATCH -N 2
#SBATCH -n 16
#SBATCH --exclusive
#SBATCH -t 2-0 
#SBATCH -J Job_name
#SBATCH -o slurm.%j.out
#SBATCH -o slurm.%j.eout
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alarcj137@gmail.com
#
# Running sequential jobs in batches by using Launcher - a simple utility for 
# executing multiple sequential or multi-threaded applications in a single 
# multi-node batch job https://github.com/TACC/launcher 
#
# Script taken from: 
# Computational Techniques for Life Sciences - Task Distribution
# https://tacc.github.io/ctls2017/docs/optimization_parallelization/optimization_parallelization_04.html
#
module load launcher

# Configure launcher
EXECUTABLE=${TACC_LAUNCHER_DIR}/init_launcher
PRUN=${TACC_LAUNCHER_DIR}/paramrun
CONTROL_FILE=launcher_cmds.txt
WORKDIR=.

# Start launcher
$PRUN $EXECUTABLE $CONTROL_FILE
