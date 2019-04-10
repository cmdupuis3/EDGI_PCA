/***********************************************************************
 *                   GNU Lesser General Public License
 *
 * This file is part of the EDGI prototype package, developed by the 
 * GFDL Flexible Modeling System (FMS) group.
 *
 * EDGI is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * EDGI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with EDGI.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#ifndef NETCDF_FILE_HPP
#define NETCDF_FILE_HPP

#include <string>
using std::string;

#include <fstream>
#include <netcdf.h>

#include "wrapper_type.hpp"





//==============================================================================
// Structs, Enums, and Typedefs
//==============================================================================

/**
 * This enum lists the different modes in which a NetCDF file can be opened.
 */
enum netcdf_mode_t {
    NETCDF_READ,
    NETCDF_WRITE,
    NETCDF_OVERWRITE,
};

/**
 * This type represents a NetCDF dimension within a NetCDF file. For
 * convenience and efficiency within the implementation, it can be explicitly
 * casted to/from int.
 */
WRAPPER_TYPE(netcdf_dim_t, int)

/**
 * This type represents a NetCDF variable within a NetCDF file. For
 * convenience and efficiency within the implementation, it can be explicitly
 * casted to/from int.
 */
WRAPPER_TYPE(netcdf_var_t, int)





//==============================================================================
// Class Declaration
//==============================================================================

/**
 *
 */
class netcdf_file_t {
private:
    
    /**
     * The NetCDF ID of the file
     */
    int file_id;
    
    /**
     * Should the file be closed when this object is destroyed?
     */
    bool close_file;
    
    /**
     * Has the file been closed yet?
     */
    bool closed;
    
    
    
    
    
    /**
     * Initializes this netcdf_file_t object. Called only internally
     */
    void init(const std::string filename, netcdf_mode_t mode);
    
    
    
    
    
public:
    
    /**
     * Creates a netcdf_file_t object by opening the file with the given mode
     */
    netcdf_file_t(const std::string filename, netcdf_mode_t mode);
    
    /**
     * Creates a netcdf_file_t object by opening the file with the NETCDF_WRITE mode
     */
    netcdf_file_t(const std::string filename);
    
    /**
     * Creates a netcdf_file_t object from the file ID of an already opened NetCDF file
     */
    netcdf_file_t(int file_id);
    
    /**
     * Deletes this netcdf_file_t object
     */
    ~netcdf_file_t();
    
    
    
    
    
    //==========================================================================
    // Miscellaneous file functions
    //==========================================================================
     
    /**
     * Returns the ID of the NetCDF file that this object contains
     */
    int get_file_id() const;
    
    /**
     * Synchronize the memory buffers with the copy of this file, writing
     * and reading data as necessary
     */
    void sync();
    
    /**
     * Close this NetCDF file object, writing any un-synchronized data first
     */
    void close();
    
    /**
     * Begin define mode so that dimensions and variables can be safely defined
     * without corrupting the underlying file
     */
    void begin_def();
    
    /**
     * End define mode so that data can be read from and written to the
     * underlying file
     */
    void end_def();
    
    
    
    
    
    //==========================================================================
    // Attribute functions
    //==========================================================================
    
    /**
     * Returns whether a variable has been defined with a fill value
     */
    bool has_fill(netcdf_var_t var) const;
    
    /**
     * Returns the fill value of a variable
     */
    template<typename T>
    T get_fill(netcdf_var_t var) const;
    
    /**
     * Sets the fill value of a variable. This can only be used in define mode
     * after defining a new variable.
     */
    template<typename T>
    void set_fill(netcdf_var_t var, const T val, const std::string name);
    
    /**
     * Returns whether the given attribute already exists in this NetCDF file
     */
    bool has_attr(netcdf_var_t var, const std::string name) const;
    
    /**
     * Returns the named attribute associated with the given variable
     */
    template<typename T>
    T get_attr(netcdf_var_t var, const std::string name) const;
    
    /**
     * Sets the value of the named attribute associated with the given variable
     */
    template<typename T>
    void set_attr(netcdf_var_t var, const std::string name, const T val);
    
    
    
    
    
    //==========================================================================
    // Dimension functions
    //==========================================================================
    
    /**
     * Returns whether the given dimension already exists in this NetCDF file
     */
    bool has_dim(const std::string name) const;
    
    /**
     * Returns the netcdf_dim_t of the dimension with the given name
     */
    netcdf_dim_t get_dim(const std::string name) const;
    
    /**
     * Returns the name of the dimension with the given netcdf_dim_t
     */
    std::string get_dim_name(netcdf_dim_t dim) const;
    
    /**
     * Returns the length of the dimension with the given netcdf_dim_t
     */
    size_t get_dim_len(netcdf_dim_t dim) const;
    
    /**
     * Defines a new dimension with the given name and length. Must be in
     * define mode (begin_def)
     */
    netcdf_dim_t def_dim(const std::string name, size_t len);
    
    
    
    
    
    //==========================================================================
    // Variable functions
    //==========================================================================
    
    /**
     * Returns the total number of variables in this file
     */
    int get_n_vars() const;
    
    /**
     * Returns whether the given variable already exists in this NetCDF file
     */
    bool has_var(const std::string name) const;
    
    /**
     * Returns the netcdf_var_t of the variable with the given name
     */
    netcdf_var_t get_var(const std::string name) const;
    
    /**
     * Returns the name of the variable represented by var
     */
    std::string get_var_name(netcdf_var_t var) const;
    
    /**
     * Returns the number of dimensions of the variable represented by var
     */
    int get_var_n_dims(netcdf_var_t var) const;
    
    /**
     * Returns an array of netcdf_dim_t's for all dimensions of the variable represented
     * by var. This array must be deleted by the caller.
     */
    netcdf_dim_t* get_var_dims(netcdf_var_t var) const;
    
    /**
     * Returns the [i]th dimension of the variable represented by var
     */
    netcdf_dim_t get_var_dim(netcdf_var_t var, int i) const;
    
    /**
     * Returns the combined length of all dimensions of the variable given by
     * var, such that an array of this length could hold all of its values.
     */
    size_t get_var_len(netcdf_var_t var) const;
    
    /**
     * Returns the length of the [i]th dimension of the variable represented by
     * var
     */
    size_t get_var_dim_len(netcdf_var_t var, int i) const;
    
    /**
     * Returns the name of the [i]th dimension of the variable represented by
     * var
     */
    std::string get_var_dim_name(netcdf_var_t var, int i) const;
    
    /**
     * Define a new variable with the specified name, number of dimensions, and
     * dimension objects. Must be in define mode (begin_def)
     */
    template<typename T>
    netcdf_var_t def_var(const std::string name, size_t n_dims, const netcdf_dim_t* dim_ids);
    
    /**
     * Sets all values of the variable represented by var to the values in the
     * parameter vals.
     */
    template<typename T>
    void set_var_vals(netcdf_var_t var, const T* vals);
    
    /**
     * Sets a selection of the values of the variable represented by var to the
     * values in the parameter vals.
     */
    template<typename T>
    void set_var_vals(netcdf_var_t var, const T* vals, const size_t* start, const size_t* count);
    
    /**
     * Returns an array containing all values of the variable represented by
     * var. This array must be deleted by the caller.
     */
    template<typename T>
    T* get_var_vals(netcdf_var_t var) const;
    
    /**
     * Returns an array containing selected values of the variable represented
     * by var. Parameters start and count determine where in each dimension to
     * start reading values and how many to read. The returned array must be
     * deleted by the caller.
     */
    template<typename T>
    T* get_var_vals(netcdf_var_t var, const size_t* start, const size_t* count) const;
    
};

#endif
