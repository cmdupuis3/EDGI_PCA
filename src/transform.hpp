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

/** Use std::complex */
#include <complex>

/** Use array_t */
#include "array.hpp"

/** The first 64 digits of pi, more than enough for any practical uses */
#define PI 3.141592653589793238462643383279502884197169399375105820974944592






template<typename T, typename R>
class dimensional_transform_t {
public:
    virtual array_t<R> transform_var(array_t<T> var) = 0;
    
    virtual array_t<R> transform_dim(array_t<T> dim) = 0;
};

template<typename T>
class fourier_transform_t : public dimensional_transform_t<T, std::complex<T>> {
private:
    
   
public:
    array_t<std::complex<T>> transform_var(array_t<T> var) {
        
    };

    array_t<std::complex<T>> transform_dim(array_t<T> dim) {
        
    };
};




template<typename T>
dimensional_transform_t<T, std::complex<T>> fft(T start, T stop, size_t length) {
    // ...
};

template<typename T>
dimensional_transform_t<T, std::complex<T>> fft(T* values, size_t length) {
    // ...
}

