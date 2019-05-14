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


#include "dft.hpp"





//=============================================================================
// Implementation of Class basic_dft_t
//=============================================================================

template<typename T>
basic_dft_t<T>::basic_dft_t() {
    // ...
}

template<typename T>
basic_dft_t<T>::~basic_dft_t() {
    // ...
}

template<typename T>
void basic_dft_t<T>::calculate(
    matrix_t<T>* input,
    matrix_t<std::complex<T>>* output
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();

    // TODO do some real SVD calculations. This is all fake
    TODO

    output->set_shape(rows, cols);
    for (size_t r = 0; r < output->get_rows(); r++) {
        for (size_t c = 0; c < output->get_cols(); c++) {
            output->set_elem(r, c, r + c);
        }
    }
}

template<typename T>
void basic_dft_t<T>::calculate(
    matrix_t<std::complex<T>>* input,
    matrix_t<std::complex<T>>* output
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();

    // TODO do some real SVD calculations. This is all fake
    TODO

    output->set_shape(rows, cols);
    for (size_t r = 0; r < output->get_rows(); r++) {
        for (size_t c = 0; c < output->get_cols(); c++) {
            output->set_elem(r, c, r + c);
        }
    }
}

template<typename T>
void basic_dft_t<T>::inverse(
    matrix_t<std::complex<T>>* input,
    matrix_t<std::complex<T>>* output
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();

    // TODO do some real SVD calculations. This is all fake
    TODO

    output->set_shape(rows, cols);
    for (size_t r = 0; r < output->get_rows(); r++) {
        for (size_t c = 0; c < output->get_cols(); c++) {
            output->set_elem(r, c, r + c);
        }
    }

}

template<typename T>
void basic_dft_t<T>::analytic(
    matrix_t<T>* input,
    matrix_t<std::complex<T>>* output
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();

    // TODO do some real SVD calculations. This is all fake
    TODO

    output->set_shape(rows, cols);
    for (size_t r = 0; r < output->get_rows(); r++) {
        for (size_t c = 0; c < output->get_cols(); c++) {
            output->set_elem(r, c, r + c);
        }
    }
}

template<typename T>
void basic_dft_t<T>::analytic(
    matrix_t<std::complex<T>>* input,
    matrix_t<std::complex<T>>* output
) {
    const size_t rows = input->get_rows();
    const size_t cols = input->get_cols();

    // TODO do some real SVD calculations. This is all fake
    TODO

    output->set_shape(rows, cols);
    for (size_t r = 0; r < output->get_rows(); r++) {
        for (size_t c = 0; c < output->get_cols(); c++) {
            output->set_elem(r, c, r + c);
        }
    }
}
