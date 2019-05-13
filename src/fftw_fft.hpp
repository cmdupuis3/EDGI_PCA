
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
