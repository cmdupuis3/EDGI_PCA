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

#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

/** Use size_t */
#include <cstddef>

/** Use std::string */
#include <string>

/** Use memcmp for NetCDF attributes */
#include <cstring>

#include "netcdf_file.hpp"


/* NetCDF attributes, mostly for copying from input to output. Not templated
 * because using nc_types are more flexible in dealing with attributes of
 * unknown type.
 */

class attribute_t {
private:
    //==========================================================================
    // Private Fields
    //==========================================================================

    std::string name;

    nc_type type = -100000; // something NetCDF won't treat as valid

    size_t type_size = 0;

    size_t length = 0;

    void* value = nullptr;



    //==========================================================================
    // Private Methods
    //==========================================================================

    //==================================
    // Initializing
    //==================================

    void load_from_attr(const attribute_t& attr);

    void load_from_value(std::string name, nc_type type, size_t type_size, size_t length, void* value);

    void load_from_netcdf(std::string name, const netcdf_file_t* file, const std::string var_name);

    void load_from_netcdf(std::string name, const netcdf_file_t* file);



public:
    //==========================================================================
    // Public Methods
    //==========================================================================

    //==================================
    // Constructing and Destructing
    //==================================

    attribute_t();

    attribute_t(const attribute_t& attr);

    attribute_t(std::string name, nc_type type, size_t type_size, size_t length, void* value);

    attribute_t(std::string name, const netcdf_file_t* file, const std::string var_name);

    attribute_t(std::string name, const netcdf_file_t* file);

    ~attribute_t();



    //==================================
    // Getting and Setting Fields
    //==================================

    const std::string get_name() const;

    nc_type get_type() const;

    size_t get_type_size() const;

    size_t get_length() const;

    const void* get_value() const;

    void set_name(std::string);

    void set_type(nc_type type);

    void set_type_size(size_t type_size);

    void set_length(size_t length);

    void set_value(void* value);



    //==================================
    // Operators
    //==================================

    bool operator==(const attribute_t& that) const;

    bool operator!=(const attribute_t& that) const;

};

















#endif
