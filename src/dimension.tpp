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

#include "error.hpp"
#include "debug.hpp"





//==============================================================================
// Private Methods
//==============================================================================

//======================================
// Initializing
//======================================

template<typename T>
void dimension_t<T>::load_from_dim(const dimension_t<T>& dim) {
    this->set_name(dim.name);
    this->set_values(dim.size, dim.values);
    this->set_attrs(dim.get_num_attrs(), dim.get_attrs());
}

template<typename T>
void dimension_t<T>::load_from_values(const std::string name, size_t size, const T* values, size_t num_attrs, attribute_t** attrs) {
    this->set_name(name);
    this->set_values(size, values);
    this->set_attrs(num_attrs, attrs);
}

template<typename T>
void dimension_t<T>::load_from_netcdf(const std::string name, const netcdf_file_t* file) {
    // Check that this dimension exists
    if (!file->has_dim(name)) {
        throw eof_error_t("Dimension \"" + name + "\" does not exist");
    }

    // Check that this dimension has a variable with the same name
    if (!file->has_var(name)) {
        throw eof_error_t("Dimension \"" + name + "\" has no variable with the same name");
    }

    netcdf_dim_t dim_id = file->get_dim(name);
    netcdf_var_t var_id = file->get_var(name);

    // Check that this dimension's corresponding variable has only a single dimension
    if (file->get_var_n_dims(var_id) != 1) {
        throw eof_error_t("Dimension \"" + name + "\" has a coorresponding variable with multiple dimensions");
    }

    // Check that the single dimension of this dimension's coorresponding variable is the same as this dimension
    if (file->get_var_dim(var_id, 0) != dim_id) {
        throw eof_error_t("Dimension \"" + name + "\" has a coorresponding variable with a different underlying dimension");
    }

    T* values = file->get_var_vals<T>(var_id);
    this->set_name(name);
    this->set_values(file->get_dim_len(dim_id), values);

    size_t num_attrs = file->get_n_attrs(var_id);
    std::cout << std::endl << file->get_n_attrs(var_id) << std::endl;
    attribute_t** attrs = new attribute_t*[num_attrs];
    for (size_t i = 0; i < num_attrs; i++) {
        std::string attr_name = file->get_attr(var_id, i);
        attrs[i] = new attribute_t(attr_name, file, name);
    }
    this->set_attrs(num_attrs, attrs);

    delete[] values;
}


//==============================================================================
// Public Methods
//==============================================================================

//======================================
// Constructing and Destructing
//======================================

template<typename T>
dimension_t<T>::dimension_t(const dimension_t<T>& dim) {
    this->load_from_dim(dim);
}

template<typename T>
dimension_t<T>::dimension_t(const std::string name, size_t size, const T* values, size_t num_attrs, attribute_t** attrs) {
    this->load_from_values(name, size, values, num_attrs, attrs);
}

template<typename T>
dimension_t<T>::dimension_t(const std::string name, const netcdf_file_t* file) {
    this->load_from_netcdf(name, file);
}

template<typename T>
dimension_t<T>::~dimension_t() {
    delete[] this->values;
    this->values = nullptr;
}


template<typename T>
void dimension_t<T>::clear_attrs() {
    if (this->attrs != nullptr) {
        for (size_t i = 0; i < this->get_num_attrs(); i++) {
            delete this->attrs[i];
        }
        delete[] this->attrs;
        this->attrs = nullptr;
    }

    this->num_attrs = 0;
}


//======================================
// Getting and Setting Fields
//======================================

template<typename T>
size_t dimension_t<T>::get_num_attrs() const {
    return this->num_attrs;
}

template<typename T>
bool dimension_t<T>::has_attr(const std::string name) const {
    for (size_t i = 0; i < this->get_num_attrs(); i++) {
        if (name == this->attrs[i]->get_name()) {
            return true;
        }
    }

    return false;
}

template<typename T>
size_t dimension_t<T>::find_attr(const std::string name) const {
    for (size_t i = 0; i < this->get_num_attrs(); i++) {
        if (name == this->attrs[i]->get_name()) {
            return i;
        }
    }

    throw eof_error_t("Requested attribute does not exist");
}

template<typename T>
void dimension_t<T>::rename_attr(size_t index, const std::string new_name) {
    this->attrs[index]->set_name(new_name);
}

template<typename T>
void dimension_t<T>::rename_attr(const std::string old_name, const std::string new_name) {
    this->attrs[this->find_attr(old_name)]->set_name(new_name);
}

template<typename T>
const attribute_t* dimension_t<T>::get_attr(size_t index) const {
    return (const attribute_t*) this->attrs[index];
}

template<typename T>
const attribute_t* dimension_t<T>::get_attr(const std::string name) const {
    return (const attribute_t*) this->attrs[this->find_attr(name)];
}

template<typename T>
const attribute_t** dimension_t<T>::get_attrs() const {
    return (const attribute_t**) this->attrs;
}

template<typename T>
void dimension_t<T>::set_attrs(size_t num_attrs, attribute_t** attrs) {
    this->clear_attrs();
    this->num_attrs = num_attrs;
    this->attrs = attrs;
}

template<typename T>
void dimension_t<T>::set_attrs(size_t num_attrs, const attribute_t** attrs) {
    attribute_t** new_attrs = new attribute_t*[num_attrs];
    for (size_t i = 0; i < num_attrs; i++) {
        new_attrs[i] = new attribute_t(*attrs[i]);
    }

    this->set_attrs(num_attrs, new_attrs);
}

template<typename T>
const std::string dimension_t<T>::get_name() const {
    return this->name;
}

template<typename T>
size_t dimension_t<T>::get_size() const {
    return this->size;
}

template<typename T>
const T* dimension_t<T>::get_values() const {
    return this->values;
}

template<typename T>
void dimension_t<T>::set_name(const std::string name) {
    this->name = name;
}

template<typename T>
void dimension_t<T>::set_values(size_t size, const T* values) {
    this->size = size;
    this->values = new T[size];
    for (size_t i = 0; i < size; i++) {
        this->values[i] = values[i];
    }
}




//======================================
// Operators
//======================================

template<typename T>
bool dimension_t<T>::operator==(const dimension_t<T>& that) const {
    if (this->get_name() != that.get_name()) {
        return false;
    }

    if (this->get_size() != that.get_size()) {
        return false;
    }

    size_t num_vals = this->get_size();
    const T* this_vals = this->get_values();
    const T* that_vals = that.get_values();
    for (size_t i = 0; i < num_vals; i++) {
        if (this_vals[i] != that_vals[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool dimension_t<T>::operator!=(const dimension_t<T>& that) const {
    return !(*this == that);
}

