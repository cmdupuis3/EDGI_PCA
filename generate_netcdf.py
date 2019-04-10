#!/usr/bin/env python

import netcdf
import numpy as np
import sys

missing_value = -0.0001
time_step = 7
n_lat = 10
n_lon = 18
n_time = 12

args = sys.argv[1:]
if len(args) == 0:
    args = ['temp']

def var_func(lat, lon, time):
    return 15 + 15 * np.sin(6.28 * time * time_step / 365) * np.cos(3.14 * lat / 180) + np.random.randint(20) / 10

def is_missing(lat, lon, time):
    return (lat + lon) % 9 == 3;

ds = netcdf.Dataset('sample.nc', 'w', 'NETCDF4')

ds.createDimension('lat', n_lat)
ds.createDimension('lon', n_lon)
ds.createDimension('time', n_time)

varlat = ds.createVariable('lat', float, 'lat')
varlon = ds.createVariable('lon', float, 'lon')
vartime = ds.createVariable('time', float, 'time')

varlat[:] = np.arange(-90, 90 + 1, 180 // (n_lat - 1))
varlon[:] = np.arange(0, 360, 360 // n_lon)
vartime[:] = np.arange(0, n_time * time_step, time_step)

for varname in args:
    var = ds.createVariable(varname, float, ('lat', 'lon', 'time'), fill_value=missing_value)
    
    for lat in xrange(n_lat):
        for lon in xrange(n_lon):
            for time in xrange(n_time):
                if is_missing(lat, lon, time):
                    var[lat, lon, time] = missing_value
                else:
                    var[lat, lon, time] = var_func(varlat[lat], varlon[lon], vartime[time])
    
    #var._FillValue = missing_value

ds.close()

