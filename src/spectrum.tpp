// Note: This is not intended to be a standalone implementation file.

#include "error.hpp"
#include "utils.hpp"
#include "debug.hpp"

#include <iterator>



template<typename S, typename T>
spectrum_t<S, T>::spectrum_t() {
    this->dft = new basic_dft_t<T>();
}

template<typename S, typename T>
spectrum_t<S, T>::~spectrum_t() {
    delete this->dft;
}
template<typename S, typename T>
void spectrum_t<S, T>::set_dft(dft_t<T>* dft) {
    delete this->dft;
    this->dft = dft;
}

template<typename S, typename T>
std::vector<variable_t<std::complex<T>, T>*> spectrum_t<S, T>::get_spectra(
    std::vector<variable_t<S, T>*> input_vars,
    std::string input_dim,
    dimension_t<T>* spectrum_dim,
    matrix_t<std::complex<T>>** spectra,
    matrix_reducer_t<S>** reducers
) {
    std::vector<variable_t<std::complex<T>, T>*> output_vars;

    size_t col = 0;
    for (size_t i = 0; i < input_vars.size(); i++) {
        variable_t<S, T>* var = input_vars[i];
        size_t size = reducers[i]->get_reduced_cols();

        matrix_t<std::complex<T>>* mat = spectra[i]->get_submatrix(0, col, spectra[i]->get_rows(), size);
        matrix_t<std::complex<T>>* restored = reducers[i]->restore(mat, 10.f*var->get_absmax());
        variable_t<std::complex<T>, T>* output = var->from_matrix_complex(restored, input_dim, spectrum_dim);

        delete restored;
        delete mat;

        output_vars.push_back(output);
        col += size;
    }

    return output_vars;
}

template<typename S, typename T>
std::vector<variable_t<std::complex<T>, T>*> spectrum_t<S, T>::analytic(
    variable_t<S, T>* input_var,
    const std::string input_dim,
    const size_t input_nthreads
) {
    return this->analytic({input_var}, input_dim, input_nthreads);
}

template<typename S, typename T>
std::vector<variable_t<std::complex<T>, T>*> spectrum_t<S, T>::analytic(
    std::initializer_list<variable_t<S, T>*> vars,
    const std::string input_dim,
    const size_t input_nthreads
) {
    std::vector<variable_t<S, T>*> input_vars(std::begin(input_vars_list), std::end(input_vars_list));
    return this->analytic(input_vars, input_dim, input_nthreads);
}


template<typename S, typename T>
std::vector<variable_t<std::complex<T>, T>*> spectrum_t<S, T>::analytic(
    std::vector<variable_t<S, T>*> input_vars,
    const std::string input_dim,
    const size_t input_nthreads
) {
    if (input_vars.size() == 0) {
        throw eof_error_t("No variables to be analyzed");
    }

    matrix_reducer_t<S>* reducers[input_vars.size()];

    size_t size = 0;
    size_t num_vars = input_vars.size();
    matrix_t<S>** matrices;
    matrices = new matrix_t<S>*[num_vars];
    matrix_t<std::complex<T>>** spectra;
    spectra = new matrix_t<std::complex<T>>*[num_vars];

    for (size_t i = 0; i < num_vars; i++) {
        variable_t<S, T>* var = input_vars[i];
        matrix_t<S>* unreduced = var->to_matrix(input_dim);

        if (var->has_missing_value()) {
            reducers[i] = new matrix_reducer_t<S>(unreduced, var->get_missing_value());
        } else {
            reducers[i] = new matrix_reducer_t<S>(unreduced, always_false<S>());
        }

        size += reducers[i]->get_reduced_cols();
        matrices[i] = reducers[i]->reduce(unreduced);

        spectra[i] = new matrix_t<std::complex<T>>();
        this->dft->analytic(matrices[i], spectra[i]);

        delete matrices[i];
        delete unreduced;
    }

/* Don't actually need this in analytic, but will be needed in calculate and inverse
    size_t tlen = spectra[0]->get_dim(input_dim)->get_size();
    T* frequencies[tlen];
    for (size_t i = 0; i < tlen; i++) {
        frequencies[i] = 0; // meh
    }
    dimension_t<T> freq("frequency", tlen, frequencies);
*/

    dimension_t<T>* same_dim = (dimension_t<T>*)input_vars.at(0)->get_dim(input_dim);
    std::vector<variable_t<std::complex<T>, T>*> output_vars = this->get_spectra(input_vars, input_dim, same_dim, spectra, reducers);

    for (size_t i = 0; i < input_vars.size(); i++) {
        delete reducers[i];
    }

    return output_vars;
}
