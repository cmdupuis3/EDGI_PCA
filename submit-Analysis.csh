#!/bin/csh
#SBATCH --account=gfdl_f
#SBATCH --time=00:45:00
#SBATCH --nodes=1
#SBATCH --mincpus=7
#SBATCH --output=edgi.out
#SBATCH --job-name=edgi 
#SBATCH --mail-type=fail
#SBATCH --partition=analysis
#SBATCH --clusters=gfdl

source env.gfdl-analysis
set PATH=$PATH\:`pwd`
./edgi -n 7 -f vars_a.nc:SST_a_eofs.nc -v SST_a:SST_a_eofs -d time

