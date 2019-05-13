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

// Note: This is not intended to be a standalone implementation file.

#include "error.hpp"
#include "utils.hpp"
#include "debug.hpp"

#include <iterator>





//==============================================================================
// Local Helper Functions
//==============================================================================

template<typename S>
void subtract_mean(size_t len, S* data) {
    S mean = 0;
    for (size_t i = 0; i < len; i++) {
        mean += data[i];
    }
    mean /= len;
    for (size_t i = 0; i < len; i++) {
        data[i] -= mean;
    }
}

/*
template<typename S>
void get_mean()

template<typename S>
void standardize(size_t len, S* data) {
    
}
*/

template<typename S>
std::function<bool(S)> always_false() {
    return [](S) {
        return false;
    };
}





//==============================================================================
// Private Methods
//==============================================================================

template<typename S, typename T>
template<typename F>
void eof_t<S, T>::match_dimensions(
    const dimension_t<T>* dim0,
    const dimension_t<T>* dim1,
    F cmp
) {
    //if (this->interp != nullptr) {
        // TODO interpolate!
        
    //} else {
        if (dim0->get_size() != dim1->get_size()) {
            throw eof_error_t("Input dimension lengths do not match");
        }
        
        size_t size = dim0->get_size();
        const T* vals0 = dim0->get_values();
        const T* vals1 = dim1->get_values();
        for (size_t i = 0; i < size; i++) {
            if (!cmp(vals0[i], vals1[i])) {
                throw eof_error_t("Input dimension values do not match");
            }
        }
    //}
}

template<typename S, typename T>
void eof_t<S, T>::match_dimension_in_all_variables(
    std::vector<variable_t<S, T>*> input_vars,
    std::string dim_name
) {
    this->match_dimension_in_all_variables(input_vars, dim_name, [](T x, T y) { return x == y; });
}

/**
 * Checks that all input variables have the same dimension `dim_name`. Dimension
 * values are compared using the comparison function `cmp`. If any two
 * dimensions don't match, throw an exception
 */
template<typename S, typename T>
template<typename F>
void eof_t<S, T>::match_dimension_in_all_variables(
    std::vector<variable_t<S, T>*> input_vars,
    std::string dim_name,
    F cmp
) {
    for (const variable_t<S, T>* var : input_vars) {
        if (!var->has_dim(dim_name)) {
            throw eof_error_t("At least one variable doesn't contain dimension \"" + dim_name + "\"");
        }
    }
    
    // Check every pair of variables for equality in the selected dimension
    // (since comparing dimension values for equality may not be transitive)
    for (size_t i = 0; i < input_vars.size(); i++) {
        const variable_t<S, T>* var0 = input_vars[i];
        const dimension_t<T>* dim0 = var0->get_dim(var0->find_dim(dim_name));
        
        for (size_t j = i + 1; j < input_vars.size(); j++) {
            const variable_t<S, T>* var1 = input_vars[j];
            const dimension_t<T>* dim1 = var1->get_dim(var1->find_dim(dim_name));
            
            this->match_dimensions(dim0, dim1, cmp);
        }
    }
}

template<typename S, typename T>
void eof_t<S, T>::covariance_kernal(
    size_t num_vars,
    matrix_t<S>** matrices,
    matrix_t<S>* cov,
    const size_t num_threads){

    size_t len = matrices[0]->get_rows();
    S slice1[num_threads][len];
    S slice2[num_threads][len];

    // Start the Covariance Matrix timer
    time_t start = time(nullptr);

    int thread, xmax, ymax;
    size_t row, row_offset;
    size_t col, col_offset;
    matrix_t<S>* m;
    matrix_t<S>* n;
    // For every slice (`slice1`) in dimension `dim` of every variable
    for (size_t i = 0; i < num_vars; i++) {
        m = matrices[i];
        row_offset = i*(m->get_cols());
        xmax = m->get_cols();
        // For every slice (`slice2`) in dimension `dim` of every variable
        for (size_t j = 0; j < num_vars; j++) {
            n = matrices[j];
            col_offset = j*(n->get_cols());
            ymax = n->get_cols();

            #pragma omp parallel for private(thread)
            for (size_t x = 0; x < xmax; x++) {
                thread = omp_get_thread_num();

                m->get_col(x, (S*) slice1[thread]);
                /*
                #pragma omp critical
                {
                    std::cout << slice1[thread][0] << " " << std::endl;
                }*/
                subtract_mean(len, slice1[thread]);

                for (size_t y = 0; y < ymax; y++) {
                    n->get_col(y, (S*) slice2[thread]);
                    subtract_mean(len, slice2[thread]);

                    // Calculate the covariance of those two slices
                    cov->at(x + row_offset, y + col_offset) = dot_product(slice1[thread], slice2[thread], len) / (S) (len - 1);
                }
            }
        }
    }

    // Print the time required to compute the covariance matrix
    time_t end = time(nullptr);
    double time = difftime(end,start);
    std::cout << "covmat: " << time << "s; ";
}

template<typename S, typename T>
void eof_t<S, T>::spectral_covariance_kernal(
    size_t num_vars,
    matrix_t<S>** matrices,
    matrix_t<S>* cov,
    int omegas_len,
    T* omegas,
    const size_t num_threads){

    size_t len = matrices[0]->get_rows();
    S slice1[num_threads][len];
    S slice2[num_threads][len];

    // Start the Covariance Matrix timer
    time_t start = time(nullptr);

    int thread, xmax, ymax;
    size_t row, row_offset;
    size_t col, col_offset;
    matrix_t<S>* m;
    matrix_t<S>* n;
    // For every slice (`slice1`) in dimension `dim` of every variable
    for (size_t i = 0; i < num_vars; i++) {
        m = matrices[i];
        row_offset = i*(m->get_cols());
        xmax = m->get_cols();
        // For every slice (`slice2`) in dimension `dim` of every variable
        for (size_t j = 0; j < num_vars; j++) {
            n = matrices[j];
            col_offset = j*(n->get_cols());
            ymax = n->get_cols();

            #pragma omp parallel for private(thread)
            for (size_t x = 0; x < xmax; x++) {
                thread = omp_get_thread_num();

                m->get_col(x, (S*) slice1[thread]);
                subtract_mean(len, slice1[thread]);

                for (size_t y = 0; y < ymax; y++) {
                    n->get_col(y, (S*) slice2[thread]);
                    subtract_mean(len, slice2[thread]);

                    // Calculate the covariance of those two slices
                    cov->at(x + row_offset, y + col_offset) = convolve(slice1[thread], slice2[thread], omegas, len);
                }
            }
        }
    }

    // Print the time required to compute the covariance matrix
    time_t end = time(nullptr);
    double time = difftime(end,start);
    std::cout << "covmat: " << time << "s; ";
}

template<typename S, typename T>
void eof_t<S, T>::circular_covariance_kernal(
    size_t num_vars,
    matrix_t<S>** matrices,
    matrix_t<S>* cov,
    const size_t num_threads){

    if (!std::is_same<S,T>::value){
        FATAL("Circular covariance is currently only implemented for real-valued data.")
    }else{
        size_t len = matrices[0]->get_rows();
        S slice1[num_threads][len];
        S slice2[num_threads][len];

        // Start the Covariance Matrix timer
        time_t start = time(nullptr);

        int thread, xmax, ymax;
        size_t row, row_offset;
        size_t col, col_offset;
        matrix_t<S>* m;
        matrix_t<S>* n;
        // For every slice (`slice1`) in dimension `dim` of every variable
        for (size_t i = 0; i < num_vars; i++) {
            m = matrices[i];
            row_offset = i*(m->get_cols());
            xmax = m->get_cols();
            // For every slice (`slice2`) in dimension `dim` of every variable
            for (size_t j = 0; j < num_vars; j++) {
                n = matrices[j];
                col_offset = j*(n->get_cols());
                ymax = n->get_cols();

                #pragma omp parallel for private(thread)
                for (size_t x = 0; x < xmax; x++) {
                    thread = omp_get_thread_num();

                    m->get_col(x, (S*) slice1[thread]);
                    subtract_mean(len, slice1[thread]);

                    for (size_t y = 0; y < ymax; y++) {
                        n->get_col(y, (S*) slice2[thread]);
                        subtract_mean(len, slice2[thread]);

                        // Calculate the covariance of those two slices
                        cov->at(x + row_offset, y + col_offset) = circ_cov(slice1[thread], slice2[thread], len);
                    }
                }
            }
        }

        // Print the time required to compute the covariance matrix
        time_t end = time(nullptr);
        double time = difftime(end,start);
        std::cout << "covmat: " << time << "s; ";

    }
}


/**
 * TODO
 */
template<typename S, typename T>
void eof_t<S, T>::make_covariance_matrix(
    std::vector<variable_t<S, T>*> input_vars,
    std::string dim,
    matrix_t<S>* cov,
    matrix_reducer_t<S>** reducers,
    const size_t num_threads,
    bool is_circular,
    bool is_spectral,
    int omegas_len,
    T* omegas
) {
    size_t size = 0;
    size_t num_vars = input_vars.size();
    matrix_t<S>** matrices;
    matrices = new matrix_t<S>*[num_vars];
    for (size_t i = 0; i < num_vars; i++) {
        variable_t<S, T>* var = input_vars[i];
        matrix_t<S>* unreduced = var->to_matrix(dim);
        
        if (var->has_missing_value()) {
            reducers[i] = new matrix_reducer_t<S>(unreduced, var->get_missing_value());
        } else {
            reducers[i] = new matrix_reducer_t<S>(unreduced, always_false<S>());
        }
        
        size += reducers[i]->get_reduced_cols();
        matrices[i] = reducers[i]->reduce(unreduced);
        delete unreduced;
    }
    
    cov->set_shape(size, size);
    
    // TODO interpolate here? The data is organized into neat matrices so this
    // is probably the best place to interpolate
    
    // This assumes that all matrices have the same number of rows. If we
    // need to, we've already interpolated
    
    // kernel calls
    if(is_circular){
        this->circular_covariance_kernal(num_vars, matrices, cov, num_threads);
    }else{
        if(is_spectral){
            if(!omegas){
                FATAL("Data is spectral, but no frequency data is present!")
            }else{
                this->spectral_covariance_kernal(num_vars, matrices, cov, omegas_len, omegas, num_threads);
            }
        }else{
            this->covariance_kernal(num_vars, matrices, cov, num_threads);
        }
    }


    for (int i = 0; i < num_vars; i++) {
        delete matrices[i];
    }

}

/**
 * TODO
 */
template<typename S, typename T>
std::vector<variable_t<S, T>*> eof_t<S, T>::get_eofs(
    std::vector<variable_t<S, T>*> input_vars,
    std::string input_dim,
    dimension_t<T>* eof_dim,
    matrix_t<S>* u,
    matrix_reducer_t<S>** reducers
) {
    std::vector<variable_t<S, T>*> output_vars;
    
    size_t col = 0;
    for (size_t i = 0; i < input_vars.size(); i++) {
        variable_t<S, T>* var = input_vars[i];
        size_t size = reducers[i]->get_reduced_cols();
        
        matrix_t<S>* mat = u->get_submatrix(0, col, u->get_rows(), size);
        matrix_t<S>* restored = reducers[i]->restore(mat, 10.f*var->get_absmax());
        variable_t<S, T>* output = var->from_matrix(restored, input_dim, eof_dim);

        delete restored;
        delete mat;
        
        output_vars.push_back(output);
        col += size;
    }
    
    return output_vars;
}





//==============================================================================
// Public Methods
//==============================================================================

/**
 * TODO
 */
template<typename S, typename T>
eof_t<S, T>::eof_t() {
    this->svd = new basic_svd_t<T>();
    //this->interp = nullptr;
}

/**
 * TODO
 */
template<typename S, typename T>
eof_t<S, T>::~eof_t() {
    delete this->svd;
    /*
    if (this->interp != nullptr) {
        delete this->interp;
    }
    */
}

/**
 * TODO
 */
template<typename S, typename T>
void eof_t<S, T>::set_svd(svd_t<T>* svd) {
    delete this->svd;
    this->svd = svd;
}

/**
 * TODO
 */
/*
template<typename S, typename T>
void eof_t<S, T>::set_interp(interp_t<S>* interp) {
    if (this->interp != nullptr) {
        delete this->interp;
    }
    
    this->interp = interp;
}
*/

/**
 * TODO
 */
/*
template<typename S, typename T>
void eof_t<S, T>::no_interp() {
    if (this->interp != nullptr) {
        delete this->interp;
    }
}
*/

/**
 * TODO
 */
template<typename S, typename T>
std::vector<variable_t<S, T>*> eof_t<S, T>::calculate(
    variable_t<S, T>* input_var,
    const std::string input_dim,
    const size_t input_nthreads,
    bool is_circular,
    bool is_spectral,
    int omegas_len,
    T* omegas
) {
    return this->calculate({input_var}, input_dim, input_nthreads, is_circular, is_spectral, omegas_len, omegas);
}

/**
 * TODO
 */
template<typename S, typename T>
std::vector<variable_t<S, T>*> eof_t<S, T>::calculate(
    std::initializer_list<variable_t<S, T>*> input_vars_list,
    const std::string input_dim,
    const size_t input_nthreads,
    bool is_circular,
    bool is_spectral,
    int omegas_len,
    T* omegas
) {
    std::vector<variable_t<S, T>*> input_vars(std::begin(input_vars_list), std::end(input_vars_list));
    return this->calculate(input_vars, input_dim, input_nthreads, is_circular, is_spectral, omegas_len, omegas);
}

/**
 * TODO
 */
template<typename S, typename T>
std::vector<variable_t<S, T>*> eof_t<S, T>::calculate(
    std::vector<variable_t<S, T>*> input_vars,
    const std::string input_dim,
    const size_t input_nthreads,
    bool is_circular,
    bool is_spectral,
    int omegas_len,
    T* omegas
) {
    if (input_vars.size() == 0) {
        throw eof_error_t("No variables to be analyzed");
    }
    
    if(is_spectral){
        if(omegas_len == -1){
            FATAL("Number of spectral frequencies was not specified.")
        }
        if(!omegas){
            FATAL("Spectral frequencies were not specified.")
        }
    }

    this->match_dimension_in_all_variables(input_vars, input_dim);
    
    matrix_t<S> cov;
    matrix_reducer_t<S>* reducers[input_vars.size()];
    this->make_covariance_matrix(input_vars, input_dim, &cov, reducers, input_nthreads, is_circular, is_spectral, omegas_len, omegas);
    
    matrix_t<T> s;
    matrix_t<S> u;
    this->svd->calculate(&cov, &u, &s, nullptr);
    
    const T* row = s.get_row(0);
    std::string output_dim = "eigenvalues";
    dimension_t<T> eof_dim(output_dim, s.get_cols(), row);
    std::vector<variable_t<S, T>*> output_vars = this->get_eofs(input_vars, input_dim, &eof_dim, &u, reducers);
    delete[] row;
    
    for (size_t i = 0; i < input_vars.size(); i++) {
        delete reducers[i];
    }
    
    return output_vars;
}

