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

