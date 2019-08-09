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

#include "attribute.hpp"


//==============================================================================
// Private Methods
//==============================================================================

//======================================
// Initializing
//======================================

void attribute_t::load_from_attr(const attribute_t& att) {
    this->set_name(att.name);
    this->set_type(att.type);
    this->set_length(att.length);
    this->set_value(att.value);
}

void attribute_t::load_from_value(std::string name, nc_type type, size_t length, void* value) {
    this->set_name(name);
    this->set_type(type);
    this->set_length(length);
    this->set_value(value);
}

void attribute_t::load_from_netcdf(const std::string name, const netcdf_file_t* file, const std::string var_name) {

    netcdf_var_t var_id = file->get_var(var_name);

    // Check that this attribute exists
    if (!file->has_attr(var_id, name)) {
        throw eof_error_t("Attribute \"" + name + "\" does not exist in variable \"" + var_name + "\"");
    }

    this->set_name(name);
    this->set_type(file->get_attr_type(var_id, name));
    this->set_length(file->get_attr_len(var_id, name));
    this->set_value(file->get_attr_val(var_id, name));

}


void attribute_t::load_from_netcdf(const std::string name, const netcdf_file_t* file) {
    // Check that this attribute exists
    if (!file->has_attr(name)) {
        throw eof_error_t("Global attribute \"" + name + "\" does not exist");
    }

    this->set_name(name);
    this->set_type(file->get_attr_type(name));
    this->set_length(file->get_attr_len(name));
    this->set_value(file->get_attr_val(name));

}

//==============================================================================
// Public Methods
//==============================================================================

//======================================
// Constructing and Destructing
//======================================

attribute_t::attribute_t(const attribute_t& attr) {
    this->load_from_attr(attr);
}

attribute_t::attribute_t(std::string name, nc_type type, size_t length, void* value) {
    this->load_from_value(name, type, length, value);
}

attribute_t::attribute_t(std::string name, const netcdf_file_t* file, const std::string var_name){
    this->load_from_netcdf(name, file, var_name);
}

attribute_t::attribute_t(std::string name, const netcdf_file_t* file){
    this->load_from_netcdf(name, file);
}

attribute_t::~attribute_t(){
    this->value = nullptr;
}

//======================================
// Getting and Setting Fields
//======================================

const std::string attribute_t::get_name() const {
    return this->name;
}

nc_type attribute_t::get_type() const {
    return this->type;
}

size_t attribute_t::get_length() const {
    return this->length;
}

const void* attribute_t::get_value() const {
    return this->value;
}

void attribute_t::set_name(std::string name) {
    this->name = name;
}

void attribute_t::set_type(nc_type type) {
    this->type = type;
}

void attribute_t::set_length(size_t length) {
    this->length = length;
}

void attribute_t::set_value(void* value) {
    this->value = value; // shallow copy alert!
}

bool attribute_t::operator==(const attribute_t& that) const {

    if (this->name != that.get_name()) {
        return false;
    } else if (this->type != that.get_type()) {
        return false;
    } else if (this->length != that.get_length()){
        return false;
    } else if (!memcmp(this->value, that.get_value(), this->length)){
        return false;
    } else {
        return true;
    }

}

bool attribute_t::operator!=(const attribute_t& that) const {
    return !(*this == that);
}

