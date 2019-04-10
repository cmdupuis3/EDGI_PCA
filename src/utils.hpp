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

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include "debug.hpp"

/**
 * Calculates the dot product of two vectors
 */
template<typename T>
T dot_product(const T* v1, const T* v2, size_t length) {
    T sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

/**
 * Calculates the circular covariance of two vectors
 */
template<typename T>
T circ_cov(const T* v1, const T* v2, size_t length) {

    T cterm1 = 0;
    T sterm1 = 0;
    T cterm2 = 0;
    T sterm2 = 0;
    for (size_t i = 0; i < length; i++) {
        cterm1 += cos(v1[i]);
        sterm1 += sin(v1[i]);
        cterm2 += cos(v2[i]);
        sterm2 += sin(v2[i]);
    }

    T t1 = atan2(sterm1 / length, cterm1 / length);
    T t2 = atan2(sterm2 / length, cterm2 / length);

    T numerator = 0;
    T denominator1 = 0;
    T denominator2 = 0;
    for (size_t i = 0; i < length; i++) {
        numerator += sin(v1[i] - t1)*sin(v2[i] - t2);
        denominator1 += pow(sin(v1[i] - t1),2);
        denominator2 += pow(sin(v2[i] - t2),2);
    }

    T cov = numerator / sqrt(denominator1 * denominator2);

    return cov;
}


template<typename T>
T circ_cov(const std::complex<T>* v1, const std::complex<T>* v2, size_t length) {

    T cterm1 = 0;
    T sterm1 = 0;
    T cterm2 = 0;
    T sterm2 = 0;
    for (size_t i = 0; i < length; i++) {
        cterm1 += v1[i].real();
        sterm1 += v1[i].imag();
        cterm2 += v2[i].real();
        sterm2 += v2[i].imag();
    }

    T t1 = atan2(sterm1 / length, cterm1 / length);
    T t2 = atan2(sterm2 / length, cterm2 / length);

    T numerator = 0;
    T denominator1 = 0;
    T denominator2 = 0;
    for (size_t i = 0; i < length; i++) {
        // trig identity alert: sin(a-b) = sin(a)*cos(b) - cos(a)*sin(b)
        numerator +=         (v1[i].imag()*cos(t1) - v1[i].real()*sin(t1))
                            *(v2[i].imag()*cos(t2) - v2[i].real()*sin(t2));
        denominator1 += pow( (v1[i].imag()*cos(t1) - v1[i].real()*sin(t1)), 2);
        denominator2 += pow( (v2[i].imag()*cos(t2) - v2[i].real()*sin(t2)), 2);
    }

    T cov = numerator / sqrt(denominator1 * denominator2);

    return cov;
}

/**
 * Calculates the convolution of two vectors using the trapezoid rule
 *
 * @param omegas[in] Angular frequencies, assumed to be sorted either in
 *                   increasing or decreasing order.
 */
template<typename S, typename T>
S convolve(const S* v1, const S* v2, const T* omegas, size_t length) {
    S sum = 0;
    for (size_t i = 0; i < length - 1; i++) {
        //std::cout << omegas[i]<< std::endl;
        sum += 0.5f * omegas[i] * (v1[i+1] - v1[i]) * (v2[i+1] - v2[i]);
    }

    return sum;
}

/**
 * Simulates arbitrarily nested for loops in which every loop starts at 0,
 * ends at a constant, and steps by 1. Will break if any value of the
 * lengths array is 0.
 */
template<typename F>
void nested_for(size_t n, const size_t* lengths, F func) {
    size_t indices[n];
    for (size_t i = 0; i < n; i++) {
        indices[i] = 0;
    }
    
    while (true) {
        func(indices);
        
        for (size_t i = n - 1; i >= 0; i--) {
            indices[i] += 1;
            
            if (indices[i] < lengths[i]) {
                // We're done carrying
                break;
            } else if (i == 0) {
                // Carrying out of the last dimension so nothing more to do
                return;
            }
            
            // Reset the current dimension value and continue carrying
            indices[i] = 0;
        }
    }
}

#endif

