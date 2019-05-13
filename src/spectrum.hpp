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

#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

/** Use std::vector */
#include <vector>

/** Use std::initializer_list */
#include <initializer_list>

/** Use std::complex */
#include <complex>

/** Use variable_t */
#include "variable.hpp"

/** Use dft_t */
#include "dft.hpp"

/** Use matrix_reducer_t */
#include "matrix_reducer.hpp"




template<typename S, typename T>
class spectrum_t {
private:

    dft_t<T>* dft = nullptr;

    std::vector<variable_t<std::complex<T>, T>*> get_spectra(
        std::vector<variable_t<S, T>*> input_vars,
        std::string input_dim,
        dimension_t<T>* spectrum_dim,
        matrix_t<std::complex<T>>** spectra,
        matrix_reducer_t<S>** reducers
    );

public:

    spectrum_t();

    ~spectrum_t();

    void set_dft(dft_t<T>* dft);

    std::vector<variable_t<std::complex<T>, T>*> calculate(
        variable_t<S, T>* input_var,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> calculate(
        std::initializer_list<variable_t<S, T>*> vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> calculate(
        std::vector<variable_t<S, T>*> input_vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> inverse(
        variable_t<S, T>* input_var,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> inverse(
        std::initializer_list<variable_t<S, T>*> vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> inverse(
        std::vector<variable_t<S, T>*> input_vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> analytic(
        variable_t<S, T>* input_var,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> analytic(
        std::initializer_list<variable_t<S, T>*> vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

    std::vector<variable_t<std::complex<T>, T>*> analytic(
        std::vector<variable_t<S, T>*> input_vars,
        const std::string input_dim,
        const size_t input_nthreads
    );

};


//==============================================================================
// Aliases
//==============================================================================


template<typename T>
using real_spectrum_t = spectrum_t<T, T>;

template<typename T>
using complex_spectrum_t = spectrum_t<std::complex<T>, T>;


//=============================================================================
// Implementation
//=============================================================================

#include "spectrum.tpp"

#endif
