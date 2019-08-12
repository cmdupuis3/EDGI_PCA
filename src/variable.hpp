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

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

/** Use attribute_t */
#include "attribute.hpp"

/** Use dimension_t */
#include "dimension.hpp"

/** Use netcdf_file_t */
#include "netcdf_file.hpp"

/** Use matrix_t */
#include "matrix.hpp"

/** Use matrix_t */
#include "transform.hpp"

/** Use size_t */
#include <cstddef>

/** Use std::string */
#include <string>

/** Use std::function */
#include <functional>

#include <complex>



//==============================================================================
// Declaration
//==============================================================================

/**
 * Represents a netcdf variable
 */
template<typename S, typename T>
class variable_t {
private:
    //==========================================================================
    // Private Fields
    //==========================================================================

    /** The number of attributes not used for missing/fill values in this variable */
    size_t num_attrs = 0;

    /** The total number of dimensions in this variable */
    size_t num_dims = 0;

    /** This variable's dimensions */
    dimension_t<T>** dims = nullptr;

    /** This variable's attributes, excluding _FillValue and missing_value */
    attribute_t** attrs = nullptr;

    /** The striding of data in each dimension */
    size_t* striding = nullptr;

    /** The actual data */
    S* data = nullptr;

    /** Does this variable contain missing values? */
    bool contains_missing_value = false;

    /** What is the missing value? */
    S missing_value;



public:
    //==========================================================================
    // Public Methods
    //==========================================================================

    //==================================
    // Constructing and Destructing
    //==================================

    variable_t();

    variable_t(const variable_t<S, T>& other);

    variable_t(const std::string name, const netcdf_file_t* file);

    variable_t(size_t num_dims, dimension_t<T>** dims);

    variable_t(size_t num_dims, const dimension_t<T>** dims);

    ~variable_t();



    //==================================
    // Re-Initializing
    //==================================

    void load_from_var(const variable_t<S, T>& other);

    void load_from_netcdf(const std::string name, const netcdf_file_t* file);

    void load_from_dims(size_t num_dims, dimension_t<T>** dims);

    void load_from_dims(size_t num_dims, const dimension_t<T>** dims);



    //==================================
    // Clearing Fields
    //==================================

    void clear();

    void clear_dims();

    void clear_attrs();



    //==================================
    // Getting and Setting Fields
    //==================================

    size_t get_num_dims() const;

    bool has_dim(const std::string name) const;

    size_t find_dim(const std::string name) const;

    void rename_dim(size_t index, const std::string new_name);

    void rename_dim(const std::string old_name, const std::string new_name);

    const dimension_t<T>* get_dim(size_t index) const;

    const dimension_t<T>* get_dim(const std::string name) const;

    const dimension_t<T>** get_dims() const;

    void set_dims(size_t num_dims, dimension_t<T>** dims);

    void set_dims(size_t num_dims, const dimension_t<T>** dims);

    void set_dim_attrs(size_t index, size_t num_attrs, attribute_t** attrs);

    void set_dim_attrs(size_t index, size_t num_attrs, const attribute_t** attrs);



    size_t get_num_attrs() const;

    bool has_attr(const std::string name) const;

    size_t find_attr(const std::string name) const;

    void rename_attr(size_t index, const std::string new_name);

    void rename_attr(const std::string old_name, const std::string new_name);

    const attribute_t* get_attr(size_t index) const;

    const attribute_t* get_attr(const std::string name) const;

    const attribute_t** get_attrs() const;

    void set_attrs(size_t num_attrs, attribute_t** attrs);

    void set_attrs(size_t num_attrs, const attribute_t** attrs);



    bool has_missing_value() const;

    const S get_missing_value() const;

    void set_missing_value(S missing_value);

    void unset_missing_value();

    const S get_absmax() const;


    //==================================
    // Getting and Setting Data
    //==================================

    const S* get_data() const;
    //const std::complex<S>* get_data_complex() const;

    void get_slice(const size_t* start, const size_t* size, S* slice) const;

    void set_slice(const size_t* start, const size_t* size, const S* slice);

    void write(const std::string name, netcdf_file_t* file) const;

    void write_complex(const std::string name_re, const std::string name_im, netcdf_file_t* file) const;



    //==================================
    // Transformations
    //==================================

    matrix_t<S>* to_matrix(std::string dim_name) const;

    variable_t<S, T>* from_matrix(const matrix_t<S>* mat, std::string dim_name, dimension_t<T>* new_dim) const;
    variable_t<std::complex<T>, T>* from_matrix_complex(const matrix_t<std::complex<T>>* mat, std::string dim_name, dimension_t<T>* new_dim) const;

    template<typename U, typename V>
    friend variable_t<std::complex<U>, V>* make_complex_variable(variable_t<U, V>* real, variable_t<U, V>* imag);

    /*
    void convert(std::function<S(S)> func);

    template<typename K>
    variable_t<K, T>* get_converted(std::function<K(S)> func);

    void transform_dim(dimensional_transform_t<T, T> transform, const std::string dim, const std::string new_dim);

    template<typename K>
    variable_t<K>* get_transformed_dim(dimensional_transform_t<T, K> transform, const std::string dim, const std::string new_dim);
    */

};

template<typename S, typename T>
variable_t<std::complex<S>, T>* make_complex_variable(variable_t<S, T>* real, variable_t<S, T>* imag);





//==============================================================================
// Aliases
//==============================================================================

template<typename T>
using real_variable_t = variable_t<T, T>;

template<typename T>
using complex_variable_t = variable_t<std::complex<T>, T>;





//==============================================================================
// Implementation
//==============================================================================

#include "variable.tpp"

#endif

