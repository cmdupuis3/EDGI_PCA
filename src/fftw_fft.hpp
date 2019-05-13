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

#ifndef FFTW_DFT_HPP
#define FFTW_DFT_HPP

#include "matrix.hpp"
#include "dft.hpp"

#include <complex>

template<typename T>
class fftw_fft_t : public dft_t<T> {
private:
    size_t num_threads;

public:
    /**
     * Create an instance running on the default number of threads
     */
    fftw_fft_t();

    /**
     * Create an instance running on the specified number of threads
     */
    fftw_fft_t(size_t num_threads);

    ~fftw_fft_t();

    void calculate(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    );

    void calculate(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );

    void inverse(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );

    void analytic(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    );

    void analytic(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );

};

#include "fftw_fft.tpp"

#endif
