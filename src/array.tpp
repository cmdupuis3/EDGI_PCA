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

#include "array.hpp"

/** Use size_t */
#include <cstddef>
#include "debug.hpp"

template<typename T>
array_t<T>::array_t() {
    this->size = 0;
    this->data = nullptr;
}

template<typename T>
array_t<T>::array_t(size_t size) : array_t() {
    this->set_shape(size);
}

template<typename T>
array_t<T>::array_t(const array_t& other) {
    const T* vals = other.data();
    size_t size = other.size;
    this->set_size(size);
    for (size_t i = 0; i < size; i++) {
        this->data[i] = vals[i];
    }
}

template<typename T>
array_t<T>::~array_t() {
    if (data != nullptr) {
        delete[] this->data;
    }
}





template<typename T>
void array_t<T>::set_size(size_t size) {
    if (data != nullptr) {
        delete[] this->data;
        this->data = nullptr;
    }
    
    this->size = size;
    if (size != 0) {
        this->data = new T[size];
    }
}

template<typename T>
size_t array_t<T>::get_size() const {
    return this->size;
}

template<typename T>
const T* array_t<T>::get_data() const {
    return this->data;
}

template<typename T>
T* array_t<T>::get_data_unsafe() {
    return this->data;
}

template<typename T>
T& array_t<T>::at(size_t i) {
    return this->data[i];
}





template<typename T>
void array_t<T>::get_elem(size_t i, T* value) const {
    *value = this->data[i];
}

template<typename T>
void array_t<T>::get_slice(size_t i0, size_t di, T* buffer) const {
    for (size_t i = 0; i < di; i++) {
        buffer[i] = this->data[i0 + i];
    }
}





template<typename T>
T array_t<T>::get_elem(size_t i) const {
    return this->data[i];
}

template<typename T>
T* array_t<T>::get_slice(size_t i0, size_t di) const {
    T* buffer = new T[di];
    this->get_slice(i0, di, buffer);
    return buffer;
}





template<typename T>
void array_t<T>::set_elem(size_t i, T value) const {
    this->data[i] = value;
}

template<typename T>
void array_t<T>::set_slice(size_t i0, size_t di, const T* buffer) {
    for (size_t i = 0; i < di; i++) {
        this->data[i0 + i] = buffer[i];
    }
}

