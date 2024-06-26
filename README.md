
# EDGI Principal Component Analysis

### Chris Dupuis and Curtis Bechtel

A command line utility for computing principal component 
analyses (or empirical orthogonal functions) from data 
stored in NetCDF files.

Parallel computation of several flavors of PCA is available,
including support for real- and complex-valued data, 
circular-valued data, data stored in multiple files, and
PCA with multiple variables.
Any of these flavors of PCA may be computed along any
dimension of the user's choice, and may have missing rows
of data in any configuration along the remaining dimensions.

Parallelism is implemented with OpenMP, and is well-suited
to experiments on a desktop or a single HPC node.

For simple cases, the necessary command is rarely longer
than a line or two on a command line. See the examples below, and "EDGIer APIs:
Scalable, Feature-Rich Empirical Orthogonal Function 
Analysis of Distributed Geoscientific Data That 'Just Works'"
for more information.

## Compilation:

Currently, builds with the Intel compiler plus MKL, and GCC plus OpenBLAS and FFTW are supported, with 
the option of using the PLASMA package. Example makefiles (Makefile.intel-mkl, Makefile.gcc-openblas) 
are included; one should be able to copy it as "Makefile" and run "make build" to build the edgi executable.
Run "make help" to show build instructions. Some library paths in each makefile will need to be set by the user,
and have been gathered at the top.

## Usage:
    edgi <options>

### Options:
    -h                        Show help message.
    -f <i>:<o> ... (required) Read data from file <i> and write to file <o>. Multiple
                              <i>:<o> pairs can be specified and separated by spaces.
    -v <i>:<o> ... (required) Calculate EOFs on variable <i> and output as variable <o>. Multiple
                              <i>:<o> pairs can be specified and separated by spaces
    -c <i>:<o> ... (optional) Add imaginary component <i> to variable and output as <o>. Number
                              of <i>:<o> pairs must equal number of variable <i>:<o> pairs.
    -C         ... (optional) Flag for circular data. Calculates circular covariance instead of
                              regular covariance, which will enhance EOFs for circular variables.
                              Units assumed to be radians. For use with real-valued data.
    -S         ... (optional) Flag for spectral data. Assumes -d refers to an angular frequency
                              variable, and weights covariance calculation for T-series accordingly. 
    -H         ... (optional) Calculate analytic signal before running. Uses a Hilbert transform on
                              a real-valued variable to generate complex-valued results. 
    -d <i>     ... (required) T-dimension name, i.e., the dimension the EOFs are calculated along.
    -n <i>     ... (required) Set the number of cores to use.
    
### Examples:

* Bare bones:  
    `edgi -f file.nc:file_eofs.nc -v var:var_eofs -d time -n 32`

* Single variable split across multiple files:  
    `edgi -f file1.nc:file1_eofs.nc file2.nc:file2_eofs.nc file3.nc:file3_eofs.nc -v var:var_eofs -d time -n 32`

* EOFs of two variables simultaneously, both split across two files:  
    `edgi -f file1.nc:file1_eofs.nc file2.nc:file2_eofs.nc -v var1:var1_eofs var2:var2_eofs -d time -n 32`

* Complex-valued EOFs of a real-valued variable using analytic signals:  
    `edgi -f file1.nc:file1_eofs.nc -v var:var_eofs -d time -n 32 -H`
    
* Complex variable stored as component variables:  
    `edgi -f file1.nc:file1_eofs.nc -v var_re:var_eofs_re -c var_im:var_im_eofs -d time -n 32`

* Directional variable (e.g., wind stress direction, ocean current direction, etc):  
    `edgi -f file1.nc:file1_eofs.nc -v var:var_eofs -d time -n 32 -C`

* Spectral variable:  
    `edgi -f file1.nc:file1_eofs.nc -v var:var_eofs -d frequency -n 32 -S`

* EOFs along ensemble member dimension:  
    `edgi -f file1.nc:file1_eofs.nc -v var:var_eofs -d member -n 32`
