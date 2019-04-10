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

#ifndef ARRAY_HPP
#define ARRAY_HPP

/** Use size_t */
#include <cstddef>
#include "debug.hpp"





//==============================================================================
// Declaration
//==============================================================================

template<typename T>
class array_t {
private:
    size_t size;
    
    T* data;
    
public:
    array_t();
    
    array_t(size_t size);
    
    array_t(const array_t& other);
    
    ~array_t();
    
    
    
    void set_size(size_t size);
    
    size_t get_size() const;
    
    const T* get_data() const;
    
    T* get_data_unsafe();
    
    T& at(size_t i);
    
    
    
    void get_elem(size_t i, T* value) const;
    
    void get_slice(size_t i0, size_t di, T* buffer) const;
    
    
    
    T get_elem(size_t i) const;
    
    T* get_slice(size_t i0, size_t di) const;
    
    
    
    void set_elem(size_t i, T value) const;
    
    void set_slice(size_t i0, size_t di, const T* buffer);
    
};





//==============================================================================
// Implementation
//==============================================================================

#include "array.tpp"

#endif

