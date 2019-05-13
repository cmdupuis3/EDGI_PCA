
#include "fftw_fft.hpp"

#include <cstdlib>
#include <omp.h>
#include <ctime>

#include <complex>
using std::complex;

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
    std::complex<float> slice_out[num_threads][rows];
    bool has_wisdom[num_threads];

    // Check and setup output
    bool delete_output = false;
    if (output == nullptr) {
        delete_output = true;
        output = new matrix_t<std::complex<float>>(rows, cols);
    } else {
        output->set_shape(rows, cols);
    }

    /** Use sentinel pattern to gather wisdom for later */

    input->get_col(0, (float*) slice_in[0]);

    fftwf_plan plan = fftwf_plan_dft_r2c_1d(
        rows,
        slice_in[0],
        reinterpret_cast<fftwf_complex*>(slice_out[0]),
        FFTW_PATIENT);

    fftwf_execute(plan);
    output->set_col(0, (std::complex<float>*) slice_out[0]);
    char* wisdom = fftwf_export_wisdom_to_string();

    #pragma omp parallel for private(thread)
    for(size_t x = 1; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (float*) slice_in[thread]);

        plan = fftwf_plan_dft_r2c_1d(
            rows,
            slice_in[thread],
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_PATIENT);

        fftwf_execute(plan);

        // Normalization factor
        for(size_t i = 0; i < rows; i++){
            slice_out[thread][rows] /= rows;
        }

        output->set_col(x, (std::complex<float>*) slice_out[thread]);
    }

}

template<>
void fftw_fft_t<float>::calculate(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* output
) {

    int thread;
    size_t cols = input->get_cols();

    size_t rows = input->get_rows();
    std::complex<float> slice_in[num_threads][rows];
    std::complex<float> slice_out[num_threads][rows];
    bool has_wisdom[num_threads];

    output->set_shape(rows, cols);

    /** Use sentinel pattern to gather wisdom for later */

    input->get_col(0, (std::complex<float>*) slice_in[0]);

    fftwf_plan plan = fftwf_plan_dft_1d(
        rows,
        reinterpret_cast<fftwf_complex*>(slice_in[0]),
        reinterpret_cast<fftwf_complex*>(slice_out[0]),
        FFTW_FORWARD,
        FFTW_PATIENT);

    fftwf_execute(plan);
    output->set_col(0, (std::complex<float>*) slice_out[0]);
    char* wisdom = fftwf_export_wisdom_to_string();

    #pragma omp parallel for private(thread)
    for(size_t x = 1; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (std::complex<float>*) slice_in[thread]);

        plan = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(slice_in[thread]),
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_FORWARD,
            FFTW_PATIENT);

        fftwf_execute(plan);

        // Normalization factor
        for(size_t i = 0; i < rows; i++){
            slice_out[thread][rows] /= rows;
        }

        output->set_col(x, (std::complex<float>*) slice_out[thread]);
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
    std::complex<float> slice_out[num_threads][rows];
    bool has_wisdom[num_threads];

    output->set_shape(rows, cols);

    /** Use sentinel pattern to gather wisdom for later */

    input->get_col(0, (std::complex<float>*) slice_in[0]);

    fftwf_plan plan = fftwf_plan_dft_1d(
        rows,
        reinterpret_cast<fftwf_complex*>(slice_in[0]),
        reinterpret_cast<fftwf_complex*>(slice_out[0]),
        FFTW_BACKWARD,
        FFTW_PATIENT);

    fftwf_execute(plan);
    output->set_col(0, (std::complex<float>*) slice_out[0]);
    char* wisdom = fftwf_export_wisdom_to_string();

    #pragma omp parallel for private(thread)
    for(size_t x = 1; x < cols; x++){
        thread = omp_get_thread_num();

        #pragma omp critical
        {
        fftwf_import_wisdom_from_string(wisdom);
        }

        input->get_col(x, (std::complex<float>*) slice_in[thread]);

        plan = fftwf_plan_dft_1d(
            rows,
            reinterpret_cast<fftwf_complex*>(slice_in[thread]),
            reinterpret_cast<fftwf_complex*>(slice_out[thread]),
            FFTW_BACKWARD,
            FFTW_PATIENT);

        fftwf_execute(plan);

        // Normalization factor
        for(size_t i = 0; i < rows; i++){
            slice_out[thread][rows] /= rows;
        }

        output->set_col(x, (std::complex<float>*) slice_out[thread]);
    }

}

template<>
void fftw_fft_t<float>::analytic(
    matrix_t<float>* input,
    matrix_t<std::complex<float>>* output
) {

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

}


template<>
void fftw_fft_t<float>::analytic(
    matrix_t<std::complex<float>>* input,
    matrix_t<std::complex<float>>* output
) {

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

}


