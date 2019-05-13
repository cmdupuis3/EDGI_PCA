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

#include "fftw_fft.hpp"

#include <cstdlib>
#include <complex>
#include <cmath>
#include <ctime>
#include <omp.h>
#include <random>

using std::complex;
using std::rand;
using std::srand;
using std::time;

/** Must be after <complex> */
#include <fftw3.h>

#include "debug.hpp"


static const size_t DEFAULT_NUM_FFT_THREADS = 4;


template<typename T>
fftw_fft_t<T>::fftw_fft_t() {
    omp_set_num_threads(DEFAULT_NUM_FFT_THREADS);
    this->num_threads = DEFAULT_NUM_FFT_THREADS;
}

template<typename T>
fftw_fft_t<T>::fftw_fft_t(size_t num_threads) {
    omp_set_num_threads(num_threads);
    this->num_threads = num_threads;

    //fftw_init_threads();
    //fftw_plan_with_nthreads(num_threads);
}

template<typename T>
fftw_fft_t<T>::~fftw_fft_t() {
    // ...
    //fftw_cleanup_threads();
}


//==============================================================================
// Template Specializations
//==============================================================================


template<>
void fftw_fft_t<float>::calculate(
    matrix_t<float>* input,
    matrix_t<std::complex<float>>* output
) {

    int thread;
    size_t cols = input->get_cols();
    size_t rows = input->get_rows();
    float               slice_in[num_threads][rows];
    std::complex<float> slice_out[num_threads][(int)(floor(rows/2)+1)];

    // Check and setup output
    bool delete_output = false;
    if (output == nullptr) {
        delete_output = true;
        output = new matrix_t<std::complex<float>>(floor(rows/2)+1, cols);
    } else {
        output->set_shape(floor(rows/2)+1, cols);
    }

    /** Use sentinel pattern to gather wisdom for later */

    float* in_temp = new float[rows];
    for(int j = 0; j < rows; j++){
        in_temp[j] = rand() % 10 * sin(j);
    }

    complex<float>* out_temp = new complex<float>[(int)(floor(rows/2)+1)];

    fftwf_plan planw = fftwf_plan_dft_r2c_1d(
            rows,
            in_temp,
            reinterpret_cast<fftwf_complex*>(out_temp),
            FFTW_PATIENT);

    fftwf_execute(planw);
    char* wisdom = fftwf_export_wisdom_to_string();
    fftwf_destroy_plan(planw);

    fftwf_plan* plans = new fftwf_plan[cols];
    #pragma omp parallel for private(thread)
    for(size_t x = 0; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (float*)(slice_in[thread]));

        plans[x] = fftwf_plan_dft_r2c_1d(
            rows,
            slice_in[thread],
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_PATIENT);

        input->get_col(x, (float*)(slice_in[thread]));
        fftwf_execute(plans[x]);

        // Normalization factor
        for(size_t i = 0; i < floor(rows/2)+1; i++){
            slice_out[thread][i] /= rows;
        }

        output->set_col(x, (std::complex<float>*)(slice_out[thread]));
    }

}

template<>
void fftw_fft_t<float>::calculate(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* output
) {

    /*
    int thread;
    size_t cols = input->get_cols();
    size_t rows = input->get_rows();
    std::complex<float> slice_in[num_threads][rows];
    std::complex<float> slice_out[num_threads][floor(rows/2)+1];

    output->set_shape(floor(rows/2)+1, cols);

    //* Use sentinel pattern to gather wisdom for later
    complex<float>* in_temp = new complex<float>[rows];
    for(int j = 0; j < rows; j++){
        in_temp[j] = complex<float>(rand() % 10 * sin(j), rand() % 10 * cos(j));
    }

    complex<float>* out_temp = new complex<float>[floor(rows/2)+1];

    fftwf_plan planw = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(in_temp),
            reinterpret_cast<fftwf_complex*>(out_temp),
            FFTW_FORWARD,
            FFTW_PATIENT);

    fftwf_execute(planw);
    char* wisdom = fftwf_export_wisdom_to_string();
    fftwf_destroy_plan(planw);

    fftwf_plan* plans = new fftwf_plan[cols];
    #pragma omp parallel for private(thread)
    for(size_t x = 0; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));

        plans[x] = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(slice_in[thread]),
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_FORWARD,
            FFTW_PATIENT);

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));
        fftwf_execute(plans[x]);

        // Normalization factor
        for(size_t i = 0; i < floor(rows/2)+1; i++){
            slice_out[thread][i] /= floor(rows/2)+1;
        }

        output->set_col(x, (std::complex<float>*)(slice_out[thread]));
    }

*/
}


template<>
void fftw_fft_t<float>::inverse(
    matrix_t<std::complex<float>>* input,
    matrix_t<float>* output
) {

    int thread;
    size_t cols = input->get_cols();
    size_t rows = input->get_rows();
    std::complex<float> slice_in[num_threads][rows];
    float              slice_out[num_threads][2*(rows - 1)];

    output->set_shape(2*(rows - 1), cols);

    /** Use sentinel pattern to gather wisdom for later */

    complex<float>* in_temp = new complex<float>[rows];
    for(int j = 0; j < rows; j++){
        in_temp[j] = complex<float>(rand() % 10 * sin(j), rand() % 10 * cos(j));
    }

    complex<float>* out_temp = new complex<float>[2*(rows - 1)];

    fftwf_plan planw = fftwf_plan_dft_c2r_1d(
        rows,
        reinterpret_cast<fftwf_complex*>(slice_in[thread]),
        slice_out[thread],
        FFTW_PATIENT);

    fftwf_execute(planw);
    char* wisdom = fftwf_export_wisdom_to_string();
    fftwf_destroy_plan(planw);

    fftwf_plan* plans = new fftwf_plan[cols];
    #pragma omp parallel for private(thread)
    for(size_t x = 0; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));

        plans[x] = fftwf_plan_dft_c2r_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(slice_in[thread]),
            slice_out[thread],
            FFTW_PATIENT);

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));
        fftwf_execute(plans[x]);

        output->set_col(x, (float*)(slice_out[thread]));
    }

}


template<>
void fftw_fft_t<float>::inverse(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* output
) {

    int thread;
    size_t cols = input->get_cols();
    size_t rows = input->get_rows();
    std::complex<float> slice_in[num_threads][rows];
    std::complex<float> slice_out[num_threads][2*(rows - 1)];

    output->set_shape(2*(rows - 1), cols);

    //* Use sentinel pattern to gather wisdom for later

    complex<float>* in_temp = new complex<float>[rows];
    for(int j = 0; j < rows; j++){
        in_temp[j] = complex<float>(rand() % 10 * sin(j), rand() % 10 * cos(j));
    }

    complex<float>* out_temp = new complex<float>[2*(rows - 1)];

    fftwf_plan planw = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(in_temp),
            reinterpret_cast<fftwf_complex*>(out_temp),
            FFTW_BACKWARD,
            FFTW_PATIENT);

    fftwf_execute(planw);
    char* wisdom = fftwf_export_wisdom_to_string();
    fftwf_destroy_plan(planw);

    fftwf_plan* plans = new fftwf_plan[cols];
    #pragma omp parallel for private(thread)
    for(size_t x = 0; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));

        plans[x] = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(slice_in[thread]),
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_BACKWARD,
            FFTW_PATIENT);

        input->get_col(x, (std::complex<float>*)(slice_in[thread]));
        fftwf_execute(plans[x]);

        output->set_col(x, (std::complex<float>*)(slice_out[thread]));
    }

}

template<>
void fftw_fft_t<float>::analytic(
    matrix_t<float>* input,
    matrix_t<std::complex<float>>* output
) {

    size_t cols = input->get_cols();
    size_t rows = input->get_rows();
    output->set_shape(rows, cols);

    matrix_t<std::complex<float>>* transformed = new matrix_t<std::complex<float>>(floor(rows/2)+1, cols);
    this->calculate(input, transformed);

    #pragma omp parallel for
    for(size_t x = 0; x < cols; x++){
        std::complex<float> slice_in[(int)(floor(rows/2)+1)];
        std::complex<float> slice_out[rows];

        transformed->get_col(x, (std::complex<float>*) slice_in);
        for(size_t i = 0; i < floor(rows/2)+1; i++){
            slice_out[i] = std::complex<float>(0.0,1.0) * slice_in[i];
        }
        transformed->set_col(x, (std::complex<float>*) slice_out);
    }

    matrix_t<std::complex<float>>* im = new matrix_t<std::complex<float>>(rows, cols);
    this->inverse(transformed, im);

    for(size_t x = 0; x < cols; x++){
        float               slice_in[rows];
        std::complex<float> slice_im[rows];
        std::complex<float> slice_out[rows];
        input->get_col(x, (float*)slice_in);
        im->get_col(x, (std::complex<float>*)slice_im);
        for(size_t i = 0; i < rows; i++){
            slice_out[i] = std::complex<float>(slice_in[i], 0.0) + (std::complex<float>(0.0,1.0) * slice_im[i]);
        }
        output->set_col(x, (std::complex<float>*) slice_out);
    }

}

template<>
void fftw_fft_t<float>::analytic(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* output
) {

    /*
    size_t cols = input->get_cols();
    size_t rows = input->get_rows();

    matrix_t<std::complex<float>>* transformed = new matrix_t<std::complex<float>>(rows, cols);
    this->calculate(input, transformed);

    float xf;
    int* xh = new int[rows];
    for(size_t i = 0; i < rows; i++){
        xf = ((int)i + 1) - input->get_rows()/2.f;
        if(xf - 0.5 < 0){
            xh[i] = -1;
        }else{
            xh[i] = 1;
        }
    }

    #pragma omp parallel for shared(xh)
    for(size_t x = 0; x < cols; x++){
        std::complex<float> slice_in[rows];
        std::complex<float> slice_out[rows];

        transformed->get_col(x, (std::complex<float>*) slice_in);
        for(size_t i = 0; i < rows; i++){
            slice_out[i] = xh[i] * complex<float>(0.0, 1.0) * slice_in[i];
        }
        transformed->set_col(x, (std::complex<float>*) slice_out);
    }

    this->inverse(transformed, output);

*/
}

