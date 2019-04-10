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

#ifndef MATRIX_HPP
#define MATRIX_HPP

/** Use size_t */
#include <cstddef>
#include "debug.hpp"





//==============================================================================
// Declaration
//==============================================================================

template<typename T>
class matrix_t {
private:
    size_t rows;
    
    size_t cols;
    
    T* data;
    
public:
    matrix_t();
    
    matrix_t(size_t rows, size_t cols);
    
    matrix_t(const matrix_t& other);
    
    ~matrix_t();
    
    
    
    void set_shape(size_t rows, size_t cols);
    
    size_t get_rows() const;
    
    size_t get_cols() const;
    
    const T* get_data() const;
    
    T* get_data_unsafe();
    
    T& at(size_t r, size_t c);
    
    
    
    void get_elem(size_t r, size_t c, T* value) const;
    
    void get_row(size_t r, T* buffer) const;
    
    void get_col(size_t c, T* buffer) const;
    
    void get_slice(size_t r0, size_t c0, size_t dr, size_t dc, T* buffer) const;
    
    void get_submatrix(size_t r0, size_t c0, size_t dr, size_t dc, matrix_t<T>* mat) const;
    
    
    T get_elem(size_t r, size_t c) const;
    
    T* get_row(size_t r) const;
    
    T* get_col(size_t c) const;
    
    T* get_slice(size_t r0, size_t c0, size_t dr, size_t dc) const;
    
    matrix_t<T>* get_submatrix(size_t r0, size_t c0, size_t dr, size_t dc) const;
    
    
    
    void set_elem(size_t r, size_t c, T value);
    
    void set_row(size_t r, const T* buffer);
    
    void set_col(size_t c, const T* buffer);
    
    void set_slice(size_t r0, size_t c0, size_t dr, size_t dc, const T* buffer);
    
    void set_submatrix(size_t r0, size_t c0, size_t dr, size_t dc, const matrix_t<T>* mat);
    
};





//==============================================================================
// Implementation
//==============================================================================

#include "matrix.tpp"

#endif

