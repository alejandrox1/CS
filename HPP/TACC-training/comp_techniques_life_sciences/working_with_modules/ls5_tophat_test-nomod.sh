#!/bin/bash
#
#SBATCH -A my_project
#SBATCH -p normal
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 10:00
#SBATCH -J tophat_test
#SBATCH -o slurm.%j.out
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alarcj137@gmail.com
set -e                                                                          
set -o pipefail                                                                 
set -o posix
module load boost/1.59                                                          
module load perl/5.22.1                                                         
module load bowtie/2.2.6

export PATH=$PATH:/work/05692/jochoa/lonestar/apps/tophat/v2.1.1/bin

export APPS_SRC="${WORK}/apps"
export TOPHAT_TEST="${APPS_SRC}/tophat-test"

mkdir -p "${TOPHAT_TEST}" && \
    wget --no-check-certificate https://ccb.jhu.edu/software/tophat/downloads/test_data.tar.gz && \
    tar -xvzf test_data.tar.gz && \
    cd test_data && \
    tophat -r 20 test_ref reads_1.fq reads_2.fq
