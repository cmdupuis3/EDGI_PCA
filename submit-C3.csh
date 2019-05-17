#!/bin/csh
#SBATCH --account=gfdl_f
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --output=edgi.out
#SBATCH --job-name=edgi 
#SBATCH --mail-type=fail
#SBATCH --partition=batch
#SBATCH --qos=normal
#SBATCH --clusters=c3

source env.gfdl-gaea
set PATH=$PATH\:`pwd`
./edgi -n 32 -f fv_core.res.tile1.nc:eofs.nc -v T:T_eofs -d zaxis_1

