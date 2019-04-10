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

#include "svd.hpp"





//=============================================================================
// Implementation of Class basic_svd_t
//=============================================================================

template<typename T>
basic_svd_t<T>::basic_svd_t() {
    // ...
}

template<typename T>
basic_svd_t<T>::~basic_svd_t() {
    // ...
}

template<typename T>
void basic_svd_t<T>::calculate(
    matrix_t<T>* input,
    matrix_t<T>* u,
    matrix_t<T>* s,
    matrix_t<T>* vt
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();
    
    // TODO do some real SVD calculations. This is all fake
    TODO
    
    u->set_shape(rows, rows);
    for (size_t r = 0; r < u->get_rows(); r++) {
        for (size_t c = 0; c < u->get_cols(); c++) {
            u->set_elem(r, c, r + c);
        }
    }
    
    s->set_shape(1, rows <= cols ? rows : cols);
    for (size_t r = 0; r < s->get_rows(); r++) {
        for (size_t c = 0; c < s->get_cols(); c++) {
            s->set_elem(r, c, r + c);
        }
    }
    
    vt->set_shape(cols, cols);
    for (size_t r = 0; r < vt->get_rows(); r++) {
        for (size_t c = 0; c < vt->get_cols(); c++) {
            vt->set_elem(r, c, r + c);
        }
    }
}

template<typename T>
void basic_svd_t<T>::calculate(
    matrix_t<std::complex<T>>* input,
    matrix_t<std::complex<T>>* u,
    matrix_t<T>*               s,
    matrix_t<std::complex<T>>* vt
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();
    
    // TODO do some real SVD calculations. This is all fake
    TODO
    
    u->set_shape(rows, rows);
    for (size_t r = 0; r < u->get_rows(); r++) {
        for (size_t c = 0; c < u->get_cols(); c++) {
            u->set_elem(r, c, r + c);
        }
    }
    
    s->set_shape(1, rows <= cols ? rows : cols);
    for (size_t r = 0; r < s->get_rows(); r++) {
        for (size_t c = 0; c < s->get_cols(); c++) {
            s->set_elem(r, c, r + c);
        }
    }
    
    vt->set_shape(cols, cols);
    for (size_t r = 0; r < vt->get_rows(); r++) {
        for (size_t c = 0; c < vt->get_cols(); c++) {
            vt->set_elem(r, c, r + c);
        }
    }
}

