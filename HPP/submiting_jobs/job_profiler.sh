#!/bin/bash
#SBATCH -A my_project
#SBATCH -p partition
#SBATCH -N 2
#SBATCH -n 16
#SBATCH --exclusive
#SBATCH -t 2-0 
#SBATCH -J Job_name
#SBATCH -o slurm.%j.out
#SBATCH --mail-type=ALL
#SBATCH --mail-user=alarcj137@gmail.com
#
# see `module help remora`:
#
# REMORA will create a set of folders with a number of files that contain
# the values for the parameters previously introduced.
#
# REMORA will also create a set of HTML files with the results plotted for
# an easier analysis. There's a summary HTML file in the main results folder
# that contains links to the different results. Transfer the results to your
# own machine to visualize them.
#
# The following environment variables control the behaviour of the tool:
#
#  - REMORA_PERIOD  - How often memory usage is checked. Default
#                     is 10 seconds.
#  - REMORA_VERBOSE - Verbose mode will save all information to
#                     a file. Default is 0 (off).
#  - REMORA_MODE    - FULL for all stats, BASIC for memory and cpu only.
#                     Default if FULL.
#  - REMORA_TMPDIR  - Directory for intermediate files. Default is the
#                     remora output directory.
#
# The remora module also defines the following environment variables:
# REMORA_DIR, REMORA_LIB, REMORA_INC and REMORA_BIN for the location
# of the REMORA distribution, libraries, include files, and tools respectively.
#
# To generate a summary report after a crash use:
# 
#   remora_post_crash <JOBID>
#

remora ibrun my_parallel_program [arguments]
