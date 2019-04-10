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

#ifndef MKL_SVD_HPP
#define MKL_SVD_HPP

/** Use svd_t */
#include "svd.hpp"

#include <cstdlib>
#include <complex>





//==============================================================================
// Declaration
//==============================================================================

template<typename T>
class mkl_svd_t : public svd_t<T> {
private:
    size_t num_threads;
    
public:
    /**
     * Create an instance running on the default number of threads
     */
    mkl_svd_t();
    
    /**
     * Create an instance running on the specified number of threads
     */
    mkl_svd_t(size_t num_threads);
    
    /**
     * 
     */
    ~mkl_svd_t();
    
    /**
     * Set the number of threads to run on
     */
    void set_num_threads(size_t num_threads);
    
    /**
     * 
     */
    void calculate(
        matrix_t<T>* input,
        matrix_t<T>* u,
        matrix_t<T>* s,
        matrix_t<T>* vt
    );
    
    /**
     * 
     */
    void calculate(
        matrix_t<std::complex<T>>* input,
        matrix_t<std::complex<T>>* u,
        matrix_t<T>*               s,
        matrix_t<std::complex<T>>* vt
    );
    
};





//==============================================================================
// Implementation
//==============================================================================

#include "mkl_svd.tpp"




#endif
