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

#ifndef MATRIX_REDUCER_HPP
#define MATRIX_REDUCER_HPP

#include <functional>





//==============================================================================
// Declaration
//==============================================================================

template<typename T>
class matrix_reducer_t {
private:
    //size_t num_reduced_rows;
    //size_t num_restored_rows;
    size_t num_reduced_cols;
    size_t num_restored_cols;
    
    //int* map_reduced_rows;
    //int* map_restored_rows;
    int* map_reduced_cols;
    int* map_restored_cols;
    
public:
    matrix_reducer_t(matrix_t<T>* mat, T missing_value);
    matrix_reducer_t(matrix_t<T>* mat, std::function<bool(T)> predicate);
    
    ~matrix_reducer_t();
    
    matrix_t<T>* reduce(const matrix_t<T>* mat) const;
    matrix_t<T>* restore(const matrix_t<T>* mat, T fill) const;
    
    size_t get_reduced_cols() const;
    size_t get_restored_cols() const;
};





//==============================================================================
// Implementation
//==============================================================================

#include "matrix_reducer.tpp"

#endif

