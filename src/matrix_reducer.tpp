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

#include <complex>
#include <functional>


template<typename T>
std::function<bool(T)> is_equal_to(T val) {
    return [val](T arg) {
        return arg == val;
    };
}

template<typename T>
static void reduce_cols(
    matrix_t<T>* mat,
    std::function<bool(T)> predicate,
    int* map_reduced,
    int* map_restored,
    size_t* reduced_size
) {
    size_t count = 0;
    for (size_t c = 0; c < mat->get_cols(); c++) {
        bool missing_col = true;
        for (size_t r = 0; r < mat->get_rows(); r++) {
            if (!predicate(mat->get_elem(r, c))) {
                missing_col = false;
                break;
            }
        }

        if (missing_col) {
            map_reduced[c] = -1;
        } else {
            map_reduced[c] = count;
            map_restored[count] = c;
            count++;
        }
    }
    
    *reduced_size = count;
}





template<typename T>
matrix_reducer_t<T>::matrix_reducer_t(matrix_t<T>* mat, T missing_value)
: matrix_reducer_t(mat, is_equal_to<T>(missing_value)) {
    // ...
}

template<typename T>
matrix_reducer_t<T>::matrix_reducer_t(matrix_t<T>* mat, std::function<bool(T)> predicate) {
    // TODO Normal reduction doesn't remove all NaN values.
    // This matrix will be left unchanged:
    //      [1 NaN 3]
    //      [4  5  6]
    //      [7 NaN 9]
    
    // Reduce cols
    this->num_restored_cols = mat->get_cols();
    this->map_restored_cols = new int[this->num_restored_cols];
    this->map_reduced_cols = new int[this->num_restored_cols];
    reduce_cols(mat, predicate, this->map_restored_cols, this->map_reduced_cols, &this->num_reduced_cols);
}

template<typename T>
matrix_reducer_t<T>::~matrix_reducer_t() {
    delete[] this->map_restored_cols;
    delete[] this->map_reduced_cols;
}

template<typename T>
matrix_t<T>* matrix_reducer_t<T>::reduce(const matrix_t<T>* mat) const {
    size_t rows = mat->get_rows();
    matrix_t<T>* reduced = new matrix_t<T>(rows, this->num_reduced_cols);
    
    T buffer[rows];
    for (size_t c = 0; c < this->num_reduced_cols; c++) {
        int mapped_c = this->map_reduced_cols[c];
        
        if (mapped_c < 0) {
            throw eof_error_t("(Internal Error) Negative map value during reduction");
        } else {
            mat->get_col(mapped_c, buffer);
            reduced->set_col(c, buffer);
        }
    }

    return reduced;
}

template<typename T>
matrix_t<T>* matrix_reducer_t<T>::restore(const matrix_t<T>* mat, T fill) const {
    size_t rows = mat->get_rows();
    matrix_t<T>* restored = new matrix_t<T>(rows, this->num_restored_cols);
    
    T fill_buffer[rows];
    for (size_t i = 0; i < rows; i++) {
        fill_buffer[i] = fill;
    }
    
    T buffer[rows];
    for (size_t c = 0; c < this->num_restored_cols; c++) {
        int mapped_c = this->map_restored_cols[c];
        
        if (mapped_c < 0) {
            restored->set_col(c, fill_buffer);
        } else {
            mat->get_col(mapped_c, buffer);
            restored->set_col(c, buffer);
        }
    }
    
    return restored;
}
template<typename T>
matrix_t<std::complex<T>>* matrix_reducer_t<T>::reduce(const matrix_t<std::complex<T>>* mat) const {
    size_t rows = mat->get_rows();
    matrix_t<std::complex<T>>* reduced = new matrix_t<std::complex<T>>(rows, this->num_reduced_cols);

    std::complex<T> buffer[rows];
    for (size_t c = 0; c < this->num_reduced_cols; c++) {
        int mapped_c = this->map_reduced_cols[c];

        if (mapped_c < 0) {
            throw eof_error_t("(Internal Error) Negative map value during reduction");
        } else {
            mat->get_col(mapped_c, buffer);
            reduced->set_col(c, buffer);
        }
    }

    return reduced;
}

template<typename T>
matrix_t<std::complex<T>>* matrix_reducer_t<T>::restore(const matrix_t<std::complex<T>>* mat, std::complex<T> fill) const {
    size_t rows = mat->get_rows();
    matrix_t<std::complex<T>>* restored = new matrix_t<std::complex<T>>(rows, this->num_restored_cols);

    std::complex<T> fill_buffer[rows];
    for (size_t i = 0; i < rows; i++) {
        fill_buffer[i] = fill;
    }

    std::complex<T> buffer[rows];
    for (size_t c = 0; c < this->num_restored_cols; c++) {
        int mapped_c = this->map_restored_cols[c];

        if (mapped_c < 0) {
            restored->set_col(c, fill_buffer);
        } else {
            mat->get_col(mapped_c, buffer);
            restored->set_col(c, buffer);
        }
    }

    return restored;
}

template<typename T>
size_t matrix_reducer_t<T>::get_reduced_cols() const {
    return this->num_reduced_cols;
}

template<typename T>
size_t matrix_reducer_t<T>::get_restored_cols() const {
    return this->num_restored_cols;
}

