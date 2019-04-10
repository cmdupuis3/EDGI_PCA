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

#include "plasma_svd.hpp"

#include <cstdlib>
#include <plasma.h>
#include <omp.h>
#include <ctime>

#include <complex>
using std::complex;

#include "debug.hpp"

static const size_t DEFAULT_NUM_THREADS = 4;





template<typename T>
plasma_svd_t<T>::plasma_svd_t() {
    this->set_num_threads(DEFAULT_NUM_THREADS);
}

template<typename T>
plasma_svd_t<T>::plasma_svd_t(size_t num_threads) {
    this->set_num_threads(num_threads);
}

template<typename T>
plasma_svd_t<T>::~plasma_svd_t() {
    // ...
}

template<typename T>
void plasma_svd_t<T>::set_num_threads(size_t num_threads) {
    this->num_threads = num_threads;
}





//==============================================================================
// Template Specializations
//==============================================================================

template<>
void plasma_svd_t<float>::calculate(
    matrix_t<float>* input,
    matrix_t<float>* u,
    matrix_t<float>* s,
    matrix_t<float>* vt
) {
    // Get the size of each dimension
    size_t rows = input->get_rows();
    size_t cols = input->get_cols();
    size_t min_dim = (rows <= cols) ? rows : cols;
    
    // Check and setup argument u
    bool delete_u = false;
    if (u == nullptr) {
        delete_u = true;
        u = new matrix_t<float>(rows, rows);
    } else {
        u->set_shape(rows, rows);
    }
    
    // Check and setup argument s
    bool delete_s = false;
    if (s == nullptr) {
        delete_s = true;
        s = new matrix_t<float>(1, min_dim);
    } else {
        s->set_shape(1, min_dim);
    }
    
    // Check and setup argument vt
    bool delete_vt = false;
    if (vt == nullptr) {
        delete_vt = true;
        vt = new matrix_t<float>(cols, cols);
    } else {
        vt->set_shape(cols, cols);
    }
    
    // Initialize Plasma
    PLASMA_Init(this->num_threads);
    
    // Allocate Plasma workspace
    PLASMA_desc* handle;
    PLASMA_Alloc_Workspace_ssyevd(rows, cols, &handle);
    
    // Start the SVD timer
    time_t start = time(nullptr);
    
    // Launch SVD solver
    // TODO check return value for success / error code
/*
    PLASMA_sgesdd(
        PlasmaVec,                  // compute all vectors in U
        PlasmaVec,                  // compute all vectors in VT
        rows,                       // number of rows in M
        cols,                       // number of columns in M
        input->get_data_unsafe(),   // M
        rows,                       // leading dimension of M
        s->get_data_unsafe(),       // S
        handle,                     // memory handle
        u->get_data_unsafe(),       // U
        rows,                       // leading dimension of U
        vt->get_data_unsafe(),      // VT
        cols                        // leading dimension of VT
    );
*/
    PLASMA_ssyevd(PlasmaVec,                // jobz
                  PlasmaUpper,              // uplo
                  rows,                     // N
                  input->get_data_unsafe(), // A
                  rows,                     // LDA
                  s->get_data_unsafe(),     // W
                  handle,                   // descT
                  u->get_data_unsafe(),     // Q
                  rows);                    // LDQ
    
    // Print the time required to compute the SVD
    time_t end = time(nullptr);
    double time = difftime(end,start);
    std::cout << "svd: " << time << "s; ";
    
    // Clean up Plasma
    PLASMA_Dealloc_Handle((void**) &handle);
    PLASMA_Finalize();
    
    // Cleanup optional arguments u, s, and vt
    if (delete_u) {
        delete u;
    }
    if (delete_s) {
        delete s;
    }
    if (delete_vt) {
        delete vt;
    }
}

template<>
void plasma_svd_t<float>::calculate(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* u,
    matrix_t<float>*               s,
    matrix_t<std::complex<float>>* vt
) {
    // Get the size of each dimension
    size_t rows = input->get_rows();
    size_t cols = input->get_cols();
    size_t min_dim = (rows <= cols) ? rows : cols;
    
    // Check and setup argument u
    bool delete_u = false;
    if (u == nullptr) {
        delete_u = true;
        u = new matrix_t<std::complex<float>>(rows, rows);
    } else {
        u->set_shape(rows, rows);
    }
    
    // Check and setup argument s
    bool delete_s = false;
    if (s == nullptr) {
        delete_s = true;
        s = new matrix_t<float>(1, min_dim);
    } else {
        s->set_shape(1, min_dim);
    }
    
    // Check and setup argument vt
    bool delete_vt = false;
    if (vt == nullptr) {
        delete_vt = true;
        vt = new matrix_t<std::complex<float>>(cols, cols);
    } else {
        vt->set_shape(cols, cols);
    }
    
    // Initialize Plasma
    PLASMA_Init(this->num_threads);
    
    // Allocate Plasma workspace
    PLASMA_desc* handle;
    PLASMA_Alloc_Workspace_cheevd(rows, cols, &handle);
    
    // Start the SVD timer
    time_t start = time(nullptr);
    
    // Launch SVD solver
    // TODO check return value for success / error code
/*
    PLASMA_cgesdd(
        PlasmaVec,                                      // compute all vectors in U
        PlasmaVec,                                      // compute all vectors in VT
        rows,                                           // number of rows in M
        cols,                                           // number of columns in M
        (PLASMA_Complex32_t*) input->get_data_unsafe(), // M
        rows,                                           // leading dimension of M
        s->get_data_unsafe(),                           // S
        handle,                                         // memory handle
        (PLASMA_Complex32_t*) u->get_data_unsafe(),     // U
        rows,                                           // leading dimension of U
        (PLASMA_Complex32_t*) vt->get_data_unsafe(),    // VT
        cols                                            // leading dimension of VT
    );
*/
    PLASMA_cheevd(PlasmaVec,                                        // jobz
                  PlasmaUpper,                                      // uplo
                  rows,                                             // N
                  (PLASMA_Complex32_t*) input->get_data_unsafe(),   // A
                  rows,                                             // LDA
                  s->get_data_unsafe(),                             // W
                  handle,                                           // descT
                  (PLASMA_Complex32_t*) u->get_data_unsafe(),       // Q
                  rows);                                            // LDQ
    
    // Print the time required to compute the SVD
    time_t end = time(nullptr);
    double time = difftime(end,start);
    std::cout << "svd: " << time << "s; ";
    
    // Clean up Plasma
    PLASMA_Dealloc_Handle((void**) &handle);
    PLASMA_Finalize();
    
    // Cleanup optional arguments u, s, and vt
    if (delete_u) {
        delete u;
    }
    if (delete_s) {
        delete s;
    }
    if (delete_vt) {
        delete vt;
    }
}

