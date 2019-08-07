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

#include "attributes.hpp"


//==============================================================================
// Private Methods
//==============================================================================

//======================================
// Initializing
//======================================

void attribute_t::load_from_att(const attribute_t& att) {
    this->set_name(att.name);
    this->set_type(att.type);
    this->set_len(att.len);
    this->set_value(att.value);
}

void attribute_t::load_from_value(std::string name, nc_type type, size_t len, const void* value) {
    this->set_name(name);
    this->set_type(type);
    this->set_len(len);
    this->set_value(value);
}

template<typename S, typename T>
void attribute_t::load_from_netcdf(const std::string name, const netcdf_file_t* file, const std::string var_name) {
    // Check that this attribute exists
    if (!file->has_attr(name)) {
        throw eof_error_t("Attribute \"" + name + "\" does not exist in variable \"" + var_name + "\"");
    }

    netcdf_var_t var_id = file->get_var(var_name);

    this->set_name(name);
    this->set_type(file->get_attr_type(var_id, name));
    this->set_len(file->get_attr_len(var_id, name));
    this->set_value(file->get_attr_val(var_id, name));

}


void attribute_t::load_from_netcdf(const std::string name, const netcdf_file_t* file) {
    // Check that this attribute exists
    if (!file->has_attr(name)) {
        throw eof_error_t("Global attribute \"" + name + "\" does not exist");
    }

    this->set_name(name);
    this->set_type(file->get_attr_type(name));
    this->set_len(file->get_attr_len(name));
    this->set_value(file->get_attr_val(name));

}



