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

#ifndef DFT_HPP
#define DFT_HPP


/** Use matrix_t */
#include "matrix.hpp"

/** Use std::complex */
#include <complex>





//=============================================================================
// Declaration of Abstract Class dft_t
//=============================================================================

template<typename T>
class dft_t {
public:
    virtual ~dft_t() {}

    /**
     *
     */
    virtual void calculate(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    ) = 0;

    /**
     *
     */
    virtual void calculate(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    ) = 0;

    /**
     *
     */
    virtual void inverse(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    ) = 0;

    /**
     *
     */
    virtual void analytic(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    ) = 0;

    /**
     *
     */
    virtual void analytic(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    ) = 0;
};


//=============================================================================
// Declaration of Class basic_dft_t
//=============================================================================

template<typename T>
class basic_dft_t : public dft_t<T> {
public:
    basic_dft_t();

    ~basic_dft_t();

    /**
     *
     */
    void calculate(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    );

    /**
     *
     */
    void calculate(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );

    /**
     *
     */
    void inverse(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );

    /**
     *
     */
    void analytic(
        matrix_t<T>* input,
        matrix_t<std::complex<T>>* output
    );

    /**
     *
     */
    void analytic(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* output
    );
};



//=============================================================================
// Template Implementation
//=============================================================================

#include "dft.tpp"

#endif
