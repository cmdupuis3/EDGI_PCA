
# EDGI Principal Component Analysis

### Chris Dupuis and Curtis Bechtel

A command line utility for computing empirical orthogonal
functions (EOFs) from data stored in NetCDF files.

This prototype version demonstrates parallel computation
of several flavors of principal component analysis (PCA;
basically the same as EOFs), including support for real-
and complex-valued data, data stored in multiple files,
PCA with multiple variables, and PCA of circular data.
Any of these flavors of PCA may be computed along any
dimension of the user's choice, and may have arbitrary
distributions of missing data along orthogonal dimensions.

Parallelism is implemented with OpenMP, and is well-suited
to experiments on a desktop or a single HPC node.

For simple cases, the necessary command is rarely longer
than a line or two on a command line. See "EDGIer APIs:
Scalable, Feature-Rich Empirical Orthogonal Function 
Analysis of Distributed Geoscientific Data That 'Just Works'"
for more information.
