
# EOF Analysis

### Chris Dupuis and Curtis Bechtel

A C++ library for performing transformations and computing empirical orthogonal
functions (EOFs) from data stored in NetCDF files.



# Class Documentation

### `array_t`

Unused class for storing and accessing in from fixed-size arrays.

### `dimension_t`

Stores information about a NetCDF dimension. This includes the name, size, and
values stored in this dimension's associated variable.

For example, given a NetCDF file that contains a dimension `time` of size 5 and
a variable `time` with a single dimension `[time]` and values `[1, 2, 3, 4, 5]`,
the dimension object representing the `time` dimension would be named `time`,
have a size of `5`, and contain the values `[1, 2, 3, 4, 5]`.

### `variable_t`

Stored information about a NetCDF variable. This includes the dimensions of this
variable 

### `eof_t`

A class for computing EOFs from `variable_t` objects.

### `eof_error_t`

The standard exception type thrown by all classes in this package when they
encounter errors.

### `matrix_t`

Class for storing and accessing data in fixed-size matrices.

### `matrix_reducer_t`

CLass for reducing matrices based on a specified missing value.

### `netcdf_file_t`

Class for reading from and writing to NetCDF files.

### `plasma_svd_t`

Class for performing singular value decomposition using PLASMA.

### `reducer_t`

Unused class for reducing matrices. Replaced by `matric_reducer_t`.

### `svd_t`

Base class for performing singular value decomposition. Any class that performs
SVD should inherit from this one.

### `transform_t`

Unused base class for transforming `variable_t` objects. A potential use would
include a subclass for discrete fourier transforms.

