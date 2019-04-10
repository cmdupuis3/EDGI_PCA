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

#include "matrix.hpp"

/** Use size_t */
#include <cstddef>
#include "debug.hpp"

template<typename T>
matrix_t<T>::matrix_t() {
    this->rows = 0;
    this->cols = 0;
    this->data = nullptr;
}

template<typename T>
matrix_t<T>::matrix_t(size_t rows, size_t cols) : matrix_t() {
    this->set_shape(rows, cols);
}

template<typename T>
matrix_t<T>::matrix_t(const matrix_t& other) {
    this->set_shape(other.rows, other.cols);
    
    const T* vals = other.data();
    size_t size = other.rows * other.cols;
    for (size_t i = 0; i < size; i++) {
        this->data[i] = vals[i];
    }
}

template<typename T>
matrix_t<T>::~matrix_t() {
    if (data != nullptr) {
        delete[] this->data;
    }
}





template<typename T>
void matrix_t<T>::set_shape(size_t rows, size_t cols) {
    if (data != nullptr) {
        delete[] this->data;
        this->data = nullptr;
    }
    
    this->rows = rows;
    this->cols = cols;
    if (rows * cols != 0) {
        this->data = new T[rows * cols];
    }
}

template<typename T>
size_t matrix_t<T>::get_rows() const {
    return this->rows;
}

template<typename T>
size_t matrix_t<T>::get_cols() const {
    return this->cols;
}

template<typename T>
const T* matrix_t<T>::get_data() const {
    return this->data;
}

template<typename T>
T* matrix_t<T>::get_data_unsafe() {
    return this->data;
}

template<typename T>
T& matrix_t<T>::at(size_t r, size_t c) {
    return this->data[r * cols + c];
}





template<typename T>
void matrix_t<T>::get_elem(size_t r, size_t c, T* value) const {
    *value = this->data[r * cols + c];
}

template<typename T>
void matrix_t<T>::get_row(size_t r, T* buffer) const {
    for (size_t c = 0; c < this->cols; c++) {
        buffer[c] = this->data[r * cols + c];
    }
}

template<typename T>
void matrix_t<T>::get_col(size_t c, T* buffer) const {
    for (size_t r = 0; r < this->rows; r++) {
        buffer[r] = this->data[r * cols + c];
    }
}

template<typename T>
void matrix_t<T>::get_slice(size_t r0, size_t c0, size_t dr, size_t dc, T* buffer) const {
    for (size_t i = 0; i < dr; i++) {
        for (size_t j = 0; j < dc; j++) {
            buffer[i * dc + j] = this->data[(r0 + i) * cols + (c0 + j)];
        }
    }
}

template<typename T>
void matrix_t<T>::get_submatrix(size_t r0, size_t c0, size_t dr, size_t dc, matrix_t<T>* mat) const {
    mat->set_shape(dr, dc);
    this->get_slice(r0, c0, dr, dc, mat->get_data_unsafe());
}





template<typename T>
T matrix_t<T>::get_elem(size_t r, size_t c) const {
    return this->data[r * cols + c];
}

template<typename T>
T* matrix_t<T>::get_row(size_t r) const {
    T* buffer = new T[this->cols];
    this->get_row(r, buffer);
    return buffer;
}

template<typename T>
T* matrix_t<T>::get_col(size_t c) const {
    T* buffer = new T[this->rows];
    this->get_col(c, buffer);
    return buffer;
}

template<typename T>
T* matrix_t<T>::get_slice(size_t r0, size_t c0, size_t dr, size_t dc) const {
    T* buffer = new T[dr * dc];
    this->get_slice(r0, c0, dr, dc, buffer);
    return buffer;
}

template<typename T>
matrix_t<T>* matrix_t<T>::get_submatrix(size_t r0, size_t c0, size_t dr, size_t dc) const {
    matrix_t<T>* mat = new matrix_t<T>(dr, dc);
    this->get_submatrix(r0, c0, dr, dc, mat);
    return mat;
}





template<typename T>
void matrix_t<T>::set_elem(size_t r, size_t c, T value) {
    this->data[r * cols + c] = value;
}

template<typename T>
void matrix_t<T>::set_row(size_t r, const T* buffer) {
    for (size_t c = 0; c < this->cols; c++) {
        this->data[r * cols + c] = buffer[c];
    }
}

template<typename T>
void matrix_t<T>::set_col(size_t c, const T* buffer) {
    for (size_t r = 0; r < this->rows; r++) {
        this->data[r * cols + c] = buffer[r];
    }
}

template<typename T>
void matrix_t<T>::set_slice(size_t r0, size_t c0, size_t dr, size_t dc, const T* buffer) {
    for (size_t i = 0; i < dr; i++) {
        for (size_t j = 0; j < dc; j++) {
            this->data[(r0 + i) * cols + (c0 + j)] = buffer[i * dc + j];
        }
    }
}

template<typename T>
void matrix_t<T>::set_submatrix(size_t r0, size_t c0, size_t dr, size_t dc, const matrix_t<T>* mat) {
    this->set_slice(r0, c0, dr, dc, mat->get_data());
}

