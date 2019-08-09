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

#ifndef DIMENSION_HPP
#define DIMENSION_HPP

/** Use size_t */
#include <cstddef>

/** Use std::string */
#include <string>

/** Use netcdf_file_t */
#include "netcdf_file.hpp"




//==============================================================================
// Declaration
//==============================================================================

/**
 * Represents a netcdf dimension/variable pair
 */
template<typename T>
class dimension_t {
private:
    //==========================================================================
    // Private Fields
    //==========================================================================

    std::string name;

    size_t size = 0;

    T* values = nullptr;

    /** The number of attributes not used for missing/fill values in this dimension's variable */
    size_t num_attrs = 0;

    /** This dimension variable's attributes */
    attribute_t** attrs = nullptr;

    //==========================================================================
    // Private Methods
    //==========================================================================

    //==================================
    // Initializing
    //==================================

    void load_from_dim(const dimension_t<T>& dim);

    void load_from_values(std::string name, size_t size, const T* values);

    void load_from_netcdf(std::string name, const netcdf_file_t* file);



public:
    //==========================================================================
    // Public Methods
    //==========================================================================

    //==================================
    // Constructing and Destructing
    //==================================

    dimension_t();

    dimension_t(const dimension_t<T>& dim);

    dimension_t(std::string name, size_t size, const T* values);

    dimension_t(std::string name, const netcdf_file_t* file);

    ~dimension_t();



    //==================================
    // Getting and Setting Fields
    //==================================

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



    const std::string get_name() const;

    size_t get_size() const;

    const T* get_values() const;

    void set_name(std::string);

    void set_values(size_t size, const T* values);



    //==================================
    // Operators
    //==================================

    bool operator==(const dimension_t<T>& that) const;

    bool operator!=(const dimension_t<T>& that) const;

};





//==============================================================================
// Implementation
//==============================================================================

#include "dimension.tpp"

#endif

