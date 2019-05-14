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

// Note: This is not intended to be a standalone implementation file.

#include <limits>
#include "error.hpp"
#include "utils.hpp"
#include "debug.hpp"

#include <complex>
#include <iostream>


using std::cout;
using std::endl;
using std::complex;

static const std::string MISSING_VALUE_NAME = "_FillValue";





//==============================================================================
// Constructing and Destructing
//==============================================================================

template<typename S, typename T>
variable_t<S, T>::variable_t() {
    // nothing to do
}

template<typename S, typename T>
variable_t<S, T>::variable_t(const variable_t<S, T>& other) {
    this->load_from_var(other);
}

template<typename S, typename T>
variable_t<S, T>::variable_t(const std::string name, const netcdf_file_t* file) {
    this->load_from_netcdf(name, file);
}

template<typename S, typename T>
variable_t<S, T>::variable_t(size_t num_dims, dimension_t<T>** dims) {
    this->load_from_dims(num_dims, dims);
}

template<typename S, typename T>
variable_t<S, T>::variable_t(size_t num_dims, const dimension_t<T>** dims) {
    this->load_from_dims(num_dims, dims);
}





//==============================================================================
// Re-Initializing
//==============================================================================

template<typename S, typename T>
void variable_t<S, T>::load_from_var(const variable_t<S, T>&) {
    // TODO
    TODO
}

template<typename S, typename T>
void variable_t<S, T>::load_from_netcdf(const std::string name, const netcdf_file_t* file) {
    if (!file->has_var(name)) {
        throw eof_error_t("Variable \"" + name + "\" does not exist in this NetCDF file");
    }

    netcdf_var_t var_id = file->get_var(name);
    size_t num_dims = file->get_var_n_dims(var_id);
    dimension_t<T>** dims = new dimension_t<T>*[num_dims];

    // Load the dimensions
    for (size_t i = 0; i < num_dims; i++) {
        netcdf_dim_t dim_id = file->get_var_dim(var_id, i);
        std::string dim_name = file->get_dim_name(dim_id);

        try {
            dims[i] = new dimension_t<T>(dim_name, file);

        } catch (eof_error_t err) {
            // Cleanup memory allocations
            for (size_t j = 0; j < num_dims; j++) {
                delete dims[j];
            }
            delete[] dims;

            // Rethrow the exception
            throw;
        }
    }

    // Try loading the missing value
    if (file->has_attr(var_id, MISSING_VALUE_NAME)) {
        this->set_missing_value(file->get_attr<S>(var_id, MISSING_VALUE_NAME));
    } else if (file->has_fill(var_id)) {
        this->set_missing_value(file->get_fill<S>(var_id));
    }

    // Set the name and dimensions
    this->set_dims(num_dims, dims);

    // Load the data from NetCDF
    if (this->data != nullptr) {
        delete[] this->data;
    }

    // TODO Change to get_vara_vals to fix strided accesses later
    this->data = file->get_var_vals<S>(var_id);

    /*
    // TODO Change all missing values to NaN

    if (this->has_missing_value() && std::numeric_limits<S>::has_quiet_NaN) {
        size_t size = 1;
        for (size_t i = 0; i < this->get_num_dims(); i++) {
            size *= this->get_dim(i)->get_size();
        }

        S missing_value = this->get_missing_value();
        S nan_value = std::numeric_limits<S>::quiet_NaN();
        for (size_t i = 0; i < size; i++) {
            if (this->data[i] == missing_value) {
                this->data[i] = nan_value;
            }
        }
    }

    // TODO should the missing value be unset now? set to NaN?
    */
}

template<typename S, typename T>
void variable_t<S, T>::load_from_dims(size_t num_dims, dimension_t<T>** dims) {
    this->set_dims(num_dims, dims);
}

template<typename S, typename T>
void variable_t<S, T>::load_from_dims(size_t num_dims, const dimension_t<T>** dims) {
    this->set_dims(num_dims, dims);
}





//==============================================================================
// Clearing Fields
//==============================================================================

template<typename S, typename T>
variable_t<S, T>::~variable_t() {
    this->clear();
}

template<typename S, typename T>
void variable_t<S, T>::clear() {
    this->clear_dims();
}

template<typename S, typename T>
void variable_t<S, T>::clear_dims() {
    if (this->dims != nullptr) {
        for (size_t i = 0; i < this->get_num_dims(); i++) {
            delete this->dims[i];
        }
        delete[] this->dims;
        this->dims = nullptr;
    }

    if (this->striding != nullptr) {
        delete[] this->striding;
        this->striding = nullptr;
    }

    if (this->data != nullptr) {
        delete[] this->data;
        this->data = nullptr;
    }

    this->num_dims = 0;
}





//==============================================================================
// Getting and Setting Fields
//==============================================================================

template<typename S, typename T>
size_t variable_t<S, T>::get_num_dims() const {
    return this->num_dims;
}

template<typename S, typename T>
bool variable_t<S, T>::has_dim(const std::string name) const {
    for (size_t i = 0; i < this->get_num_dims(); i++) {
        if (name == this->dims[i]->get_name()) {
            return true;
        }
    }

    return false;
}

template<typename S, typename T>
size_t variable_t<S, T>::find_dim(const std::string name) const {
    for (size_t i = 0; i < this->get_num_dims(); i++) {
        if (name == this->dims[i]->get_name()) {
            return i;
        }
    }

    throw eof_error_t("Requested dimension does not exist");
}

template<typename S, typename T>
void variable_t<S, T>::rename_dim(size_t index, const std::string new_name) {
    this->dims[index]->set_name(new_name);
}

template<typename S, typename T>
void variable_t<S, T>::rename_dim(const std::string old_name, const std::string new_name) {
    this->dims[this->find_dim(old_name)]->set_name(new_name);
}

template<typename S, typename T>
const dimension_t<T>* variable_t<S, T>::get_dim(size_t index) const {
    return (const dimension_t<T>*) this->dims[index];
}

template<typename S, typename T>
const dimension_t<T>* variable_t<S, T>::get_dim(const std::string name) const {
    return (const dimension_t<T>*) this->dims[this->find_dim(name)];
}

template<typename S, typename T>
const dimension_t<T>** variable_t<S, T>::get_dims() const {
    return (const dimension_t<T>**) this->dims;
}

template<typename S, typename T>
void variable_t<S, T>::set_dims(size_t num_dims, dimension_t<T>** dims) {
    this->clear_dims();
    this->num_dims = num_dims;
    this->dims = dims;

    // Load the striding
    this->striding = new size_t[num_dims];
    size_t product = 1;
    for (size_t i = 0; i < num_dims; i++) {
        striding[num_dims - 1 - i] = product;
        product *= dims[num_dims - 1 - i]->get_size();
    }

    // Create a new data array as large as the product of all dimension sizes
    this->data = new S[product];
}

template<typename S, typename T>
void variable_t<S, T>::set_dims(size_t num_dims, const dimension_t<T>** dims) {
    dimension_t<T>** new_dims = new dimension_t<T>*[num_dims];
    for (size_t i = 0; i < num_dims; i++) {
        new_dims[i] = new dimension_t<T>(*dims[i]);
    }

    this->set_dims(num_dims, new_dims);
}

template<typename S, typename T>
bool variable_t<S, T>::has_missing_value() const {
    return this->contains_missing_value;
}

template<typename S, typename T>
const S variable_t<S, T>::get_missing_value() const {
    return this->missing_value;
}

template<typename S, typename T>
void variable_t<S, T>::set_missing_value(S missing_value) {
    this->missing_value = missing_value;
    this->contains_missing_value = true;
}

template<typename S, typename T>
void variable_t<S, T>::unset_missing_value() {
    this->contains_missing_value = false;
}





//==============================================================================
// Miscellaneous
//==============================================================================

template<typename S, typename T>
const S* variable_t<S, T>::get_data() const {
    return (const S*) this->data;
}
/*
template<typename S, typename T>
const std::complex<S>* variable_t<S, T>::get_data_complex() const {
    return (const std::complex<S>*) this->data;
}
*/
template<typename S, typename T>
void variable_t<S, T>::get_slice(const size_t* start, const size_t* size, S* slice) const {
    size_t len = this->get_num_dims();
    size_t start_index = dot_product<size_t>(start, this->striding, len);
    size_t n = 0;

    nested_for(this->get_num_dims(), size, [&, this](size_t* indices) {
        size_t index = start_index + dot_product<size_t>(indices, this->striding, len);
        slice[n] = this->data[index];
        n++;
    });
}

template<typename S, typename T>
void variable_t<S, T>::set_slice(const size_t* start, const size_t* size, const S* slice) {
    size_t len = this->get_num_dims();
    size_t start_index = dot_product<size_t>(start, this->striding, len);
    size_t n = 0;

    nested_for(this->get_num_dims(), size, [&, this](size_t* indices) {
        size_t index = start_index + dot_product<size_t>(indices, this->striding, len);
        this->data[index] = slice[n];
        n++;
    });
}

/**
 * Writes the variable 'var' to to the NetCDF file 'file' under the variable
 * name 'name'. This function is not a method of variable_t because C++
 * prohibits partial specialization of a single method within a template class.
 */
template<typename S, typename T>
void write_var(const variable_t<S, T>* var, const std::string name, netcdf_file_t* file) {
    netcdf_dim_t dim_ids[var->get_num_dims()];
    for (size_t i = 0; i < var->get_num_dims(); i++) {
        const dimension_t<T>* dim = var->get_dim(i);

        // Define this dimension if it doesn't already exist
        // Either way, get the dimension ID
        if (!file->has_dim(dim->get_name())) {
            file->begin_def();
            dim_ids[i] = file->def_dim(dim->get_name(), dim->get_size());
            file->end_def();
        } else {
            dim_ids[i] = file->get_dim(dim->get_name());
        }

        // Define this dimension's variable if it doesn't already exist
        // Either way, get the variable ID
        netcdf_var_t var_id;
        if (!file->has_var(dim->get_name())) {
            file->begin_def();
            var_id = file->def_var<T>(dim->get_name(), 1, &dim_ids[i]);
            file->end_def();
        } else {
            var_id = file->get_var(dim->get_name());
        }

        // Set this dimension's values
        file->set_var_vals<T>(var_id, dim->get_values());
    }

    // Define this variable if it doesn't already exist
    // Either way, get the variable ID
    netcdf_var_t var_id;
    if (!file->has_var(name)) {
        file->begin_def();
        var_id = file->def_var<S>(name, var->get_num_dims(), dim_ids);

        // Write the fill value, if applicable
        if (var->has_missing_value()) {
            file->set_fill(var_id, var->get_missing_value(), MISSING_VALUE_NAME);
        }
        file->end_def();
    } else {
        var_id = file->get_var(name);
    }

    /*
    // Write the missing value, if applicable
    if (var->has_missing_value()) {
        file->begin_def();
        file->set_attr(var_id, MISSING_VALUE_NAME, var->get_missing_value());
        file->end_def();
    }
    */


    // Set its values and synchronize the file
    file->set_var_vals<S>(var_id, var->get_data());
    file->sync();
}

/**
 * Overloads write_var for complex-valued variables
 */
template<typename S, typename T>
void write_complex_var(const variable_t<std::complex<S>, T>* var, const std::string name_re, const std::string name_im, netcdf_file_t* file) {
    size_t total_size = 1;
    netcdf_dim_t dim_ids[var->get_num_dims()];
    for (size_t i = 0; i < var->get_num_dims(); i++) {
        const dimension_t<T>* dim = var->get_dim(i);

        // Define this dimension if it doesn't already exist
        // Either way, get the dimension ID
        if (!file->has_dim(dim->get_name())) {
            file->begin_def();
            dim_ids[i] = file->def_dim(dim->get_name(), dim->get_size());
            file->end_def();
        } else {
            dim_ids[i] = file->get_dim(dim->get_name());
        }

        // Define this dimension's variable if it doesn't already exist
        // Either way, get the variable ID
        netcdf_var_t var_id;
        if (!file->has_var(dim->get_name())) {
            file->begin_def();
            var_id = file->def_var<T>(dim->get_name(), 1, &dim_ids[i]);
            file->end_def();
        } else {
            var_id = file->get_var(dim->get_name());
        }

        // Set this dimension's values and update the total size
        file->set_var_vals<T>(var_id, dim->get_values());
        total_size *= dim->get_size();
    }

    // Define this variable if it doesn't already exist
    // Either way, get the variable ID
    netcdf_var_t var_re_id;
    if (!file->has_var(name_re)) {
        file->begin_def();
        var_re_id = file->def_var<S>(name_re, var->get_num_dims(), dim_ids);

        // Write the fill value, if applicable
        if (var->has_missing_value()) {
            file->set_fill(var_re_id, var->get_missing_value().real(), MISSING_VALUE_NAME);
        }
        file->end_def();
    } else {
        var_re_id = file->get_var(name_re);
    }

    netcdf_var_t var_im_id;
    if (!file->has_var(name_im)) {
        file->begin_def();
        var_im_id = file->def_var<S>(name_im, var->get_num_dims(), dim_ids);

        // Write the fill value, if applicable
        if (var->has_missing_value()) {
            file->set_fill(var_im_id, var->get_missing_value().imag(), MISSING_VALUE_NAME);
        }
        file->end_def();
    } else {
        var_im_id = file->get_var(name_im);
    }

    // Separate the real and imaginary parts
    S* data_re = new S[total_size];
    S* data_im = new S[total_size];
    const std::complex<S>* data = var->get_data();
    for (size_t i = 0; i < total_size; i++) {
        data_re[i] = data[i].real();
        data_im[i] = data[i].imag();
    }
    /*
    // Write the missing value, if applicable
    if (var->has_missing_value()) {
        file->begin_def();
        file->set_attr(var_re_id, MISSING_VALUE_NAME + "_re", var->get_missing_value().real());
        file->set_attr(var_im_id, MISSING_VALUE_NAME + "_im", var->get_missing_value().imag());
        file->end_def();
    }
    */

    // Set its values and synchronize the file
    file->set_var_vals<S>(var_re_id, data_re);
    file->set_var_vals<S>(var_im_id, data_im);
    file->sync();
}

template<>
void variable_t<float, float>::write(const std::string name, netcdf_file_t* file) const {
    write_var(this, name, file);
}

template<>
void variable_t<std::complex<float>, float>::write_complex(const std::string name_re, const std::string name_im, netcdf_file_t* file) const {
    write_complex_var(this, name_re, name_im, file);
}





//==============================================================================
// Transformations
//==============================================================================

template<typename S, typename T>
matrix_t<S>* variable_t<S, T>::to_matrix(std::string dim_name) const {
    size_t num_dims = this->get_num_dims();
    size_t dim_ind = this->find_dim(dim_name);

    size_t cols = 1;
    size_t rows = 0;
    size_t traverse_shape[num_dims];
    size_t slice_shape[num_dims];
    for (size_t i = 0; i < num_dims; i++) {
        size_t size = this->get_dim(i)->get_size();
        if (i != dim_ind) {
            traverse_shape[i] = size;
            slice_shape[i] = 1;
            cols *= size;
        } else {
            traverse_shape[i] = 1;
            slice_shape[i] = size;
            rows = size;
        }
    }

    matrix_t<S>* mat = new matrix_t<S>(rows, cols);
    size_t c = 0;
    nested_for(num_dims, traverse_shape, [&](size_t* indices) {
        S slice[rows];
        this->get_slice(indices, (size_t*) slice_shape, (S*) slice);
        mat->set_col(c, (S*) slice);
        c++;
    });

    return mat;
}

/*
template<typename S, typename T>
const S get_var_absmax(const variable_t<S, T>* var) {

    size_t product = 1;
    for (size_t i = 0; i < var->get_num_dims(); i++) {
        product *= var->get_dims()[var->get_num_dims() - 1 - i]->get_size();
    }

    S absmax = 0;
    for (size_t i = 0; i < product; i++){
        if(absmax < abs(var->get_data()[i])) absmax = abs(var->get_data()[i]);
    }

    return absmax;
}

template<typename S, typename T>
const std::complex<S> get_var_absmax_complex(const variable_t<std::complex<S>, T>* var) {

    size_t product = 1;
    for (size_t i = 0; i < var->get_num_dims(); i++) {
        product *= var->get_dims()[var->get_num_dims() - 1 - i]->get_size();
    }

    S absmax_re = 0;
    S absmax_im = 0;
    for (size_t i = 0; i < product; i++){
        if(absmax_re < abs(var->get_data()[i].real())) absmax_re = abs(var->get_data()[i].real());
        if(absmax_im < abs(var->get_data()[i].imag())) absmax_im = abs(var->get_data()[i].imag());
    }

    return std::complex<S>(absmax_re, absmax_im);
}
*/

template<typename S, typename T>
const S variable_t<S, T>::get_absmax() const {

    size_t product = 1;
    for (size_t i = 0; i < this->get_num_dims(); i++) {
        product *= this->get_dims()[this->get_num_dims() - 1 - i]->get_size();
    }

    S absmax = 0;
    for (size_t i = 0; i < product; i++){
        if(absmax < abs(this->get_data()[i])) absmax = abs(this->get_data()[i]);
    }

    return absmax;
}

template<>
const std::complex<float> variable_t<std::complex<float>, float>::get_absmax() const {

    size_t product = 1;
    for (size_t i = 0; i < this->get_num_dims(); i++) {
        product *= this->get_dims()[this->get_num_dims() - 1 - i]->get_size();
    }

    float absmax_re = 0;
    float absmax_im = 0;
    for (size_t i = 0; i < product; i++){
        if(absmax_re < abs(this->get_data()[i].real())) absmax_re = abs(this->get_data()[i].real());
        if(absmax_im < abs(this->get_data()[i].imag())) absmax_im = abs(this->get_data()[i].imag());
    }

    return std::complex<float>(absmax_re, absmax_im);
}

template<typename S, typename T>
variable_t<S, T>* variable_t<S, T>::from_matrix(const matrix_t<S>* mat, std::string dim_name, dimension_t<T>* new_dim) const {
    size_t num_dims = this->get_num_dims();
    size_t dim_ind = this->find_dim(dim_name);

    dimension_t<T>** dims = new dimension_t<T>*[num_dims];
    size_t cols = 1;
    size_t rows = 0;
    size_t traverse_shape[num_dims];
    size_t* slice_shape;
    slice_shape = new size_t[num_dims];
    for (size_t i = 0; i < num_dims; i++) {
        if (i != dim_ind) {
            dims[i] = new dimension_t<T>(*this->get_dim(i));
            traverse_shape[i] = dims[i]->get_size();
            slice_shape[i] = 1;
            cols *= dims[i]->get_size();
        } else {
            dims[i] = new dimension_t<T>(*new_dim);
            traverse_shape[i] = 1;
            slice_shape[i] = dims[i]->get_size();
            rows = dims[i]->get_size();
        }
    }

    if (cols != mat->get_cols() || rows != mat->get_rows()) {
        throw eof_error_t("Matrix has incorrect dimensions for loading into a new variable");
    }

    variable_t<S, T>* var = new variable_t<S, T>(num_dims, dims);
    var->set_missing_value(10.f*(this->get_absmax()));

    size_t c = 0;
    nested_for(num_dims, traverse_shape, [&](size_t* indices) {
        S slice[rows];
        mat->get_col(c, (S*) slice);
        var->set_slice(indices, (size_t*) slice_shape, (S*) slice);
        c++;
    });

    return var;
}
template<typename S, typename T>
variable_t<std::complex<T>, T>* variable_t<S, T>::from_matrix_complex(const matrix_t<std::complex<T>>* mat, std::string dim_name, dimension_t<T>* new_dim) const {
    size_t num_dims = this->get_num_dims();
    size_t dim_ind = this->find_dim(dim_name);

    dimension_t<T>** dims = new dimension_t<T>*[num_dims];
    size_t cols = 1;
    size_t rows = 0;
    size_t traverse_shape[num_dims];
    size_t* slice_shape;
    slice_shape = new size_t[num_dims];
    for (size_t i = 0; i < num_dims; i++) {
        if (i != dim_ind) {
            dims[i] = new dimension_t<T>(*this->get_dim(i));
            traverse_shape[i] = dims[i]->get_size();
            slice_shape[i] = 1;
            cols *= dims[i]->get_size();
        } else {
            dims[i] = new dimension_t<T>(*new_dim);
            traverse_shape[i] = 1;
            slice_shape[i] = dims[i]->get_size();
            rows = dims[i]->get_size();
        }
    }

    if (cols != mat->get_cols() || rows != mat->get_rows()) {
        throw eof_error_t("Matrix has incorrect dimensions for loading into a new variable");
    }

    variable_t<std::complex<T>, T>* var = new variable_t<std::complex<T>, T>(num_dims, dims);
    var->set_missing_value(std::complex<T>(1.0,1.0)*10.f*(this->get_absmax()));

    size_t c = 0;
    nested_for(num_dims, traverse_shape, [&](size_t* indices) {
        std::complex<T> slice[rows];
        mat->get_col(c, (std::complex<T>*) slice);
        var->set_slice(indices, (size_t*) slice_shape, (std::complex<T>*) slice);
        c++;
    });

    return var;
}

template<typename S, typename T>
variable_t<std::complex<S>, T>* make_complex_variable(variable_t<S, T>* real, variable_t<S, T>* imag) {
    if (real->get_num_dims() != imag->get_num_dims()) {
        throw eof_error_t("Cannot make a complex variable from variables with different numbers of dimensions.");
    }

    size_t size = 1;
    size_t num_dims = real->get_num_dims();
    dimension_t<T>** dims = new dimension_t<T>*[num_dims];
    for (size_t i = 0; i < num_dims; i++) {
        if (*real->get_dim(i) != *imag->get_dim(i)) {
            for (size_t j = 0; j < i; j++) {
                delete dims[j];
            }
            delete[] dims;

            throw eof_error_t("Cannot make a complex variable from variables with different dimensions.");
        }

        dims[i] = new dimension_t<T>(*real->get_dim(i));
        size *= dims[i]->get_size();
    }

    // Create the resulting complex variable
    variable_t<std::complex<S>, T>* result = new variable_t<std::complex<S>, T>(num_dims, dims);

    // Get missing values from the real and imaginary parts
    S missing_re;
    S missing_im;
    std::complex<S> missing;
    bool has_missing_re = real->has_missing_value();
    bool has_missing_im = imag->has_missing_value();

    if (has_missing_re) {
        missing_re = real->get_missing_value();
    }
    if (has_missing_im) {
        missing_im = imag->get_missing_value();
    }

    if (has_missing_re | has_missing_im) {
        missing = std::complex<S>(missing_re, missing_im);
        result->set_missing_value(missing);
    }

    // Load the complex data, filling in missing values as necessary
    for (size_t i = 0; i < size; i++) {
        S re = real->data[i];
        S im = imag->data[i];

        if ((has_missing_re && re == missing_re) || (has_missing_im && im == missing_im)) {
            result->data[i] = missing;
        } else {
            result->data[i] = std::complex<S>(re, im);
        }
    }

    return result;
}

/*
template<typename S, typename T>
void variable_t<S, T>::convert(std::function<S(S)> func) {
    size_t size = 1;
    for (size_t i = 0; i < num_dims; i++) {
        size *= this->get_dim(i)->get_size();
    }

    for (size_t i = 0; i < size; i++) {
        this->data[i] = func(this->data[i]);
    }
}

template<typename S, typename T>
template<typename K>
variable_t<K>* variable_t<S, T>::get_converted(std::function<K(T)> func) {
    // TODO
}

template<typename S, typename T>
void variable_t<S, T>::transform_dim(dimensional_transform_t<T, T> transform, const std::string dim, const std::string new_dim) {
    // TODO
}

template<typename S, typename T>
template<typename K>
variable_t<K>* variable_t<S, T>::get_transformed_dim(dimensional_transform_t<T, K> transform, const std::string dim, const std::string new_dim) {
    // TODO
}
*/

