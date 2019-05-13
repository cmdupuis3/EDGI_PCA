
#ifndef EOF_HPP
#define EOF_HPP

/** Use std::string */
#include <string>

/** Use std::vector */
#include <vector>

/** Use std::initializer_list */
#include <initializer_list>

/** Use std::complex */
#include <complex>

/** Use std::conditional */
#include <type_traits>

/** Use variable_t */
#include "variable.hpp"

/** Use svd_t */
#include "svd.hpp"

/** Use svd_t */
#include "dft.hpp"

/** Use interp_t */
//#include "interp.hpp"

/** Use matrix_reducer_t */
#include "matrix_reducer.hpp"





//==============================================================================
// Declaration
//==============================================================================

template<typename S, typename T>
class eof_t {
private:
    //==========================================================================
    // Private Fields
    //==========================================================================

    svd_t<T>* svd = nullptr;
    
    //interp_t<S>* interp = nullptr;
    
    
    
    //==========================================================================
    // Private Methods
    //==========================================================================
    
    template<typename F>
    void match_dimensions(
        const dimension_t<T>* dim0,
        const dimension_t<T>* dim1,
        F cmp
    );
    
    void match_dimension_in_all_variables(
        std::vector<variable_t<S, T>*> input_vars,
        std::string dim_name
    );
    
    template<typename F>
    void match_dimension_in_all_variables(
        std::vector<variable_t<S, T>*> input_vars,
        std::string dim_name,
        F cmp
    );

    void covariance_kernal(
        size_t num_vars,
        matrix_t<S>** matrices,
        matrix_t<S>* cov,
        const size_t num_threads);

    void circular_covariance_kernal(
        size_t num_vars,
        matrix_t<S>** matrices,
        matrix_t<S>* cov,
        const size_t num_threads);

    void spectral_covariance_kernal(
        size_t num_vars,
        matrix_t<S>** matrices,
        matrix_t<S>* cov,
        int omegas_len,
        T* omegas,
        const size_t num_threads);
    
    void make_covariance_matrix(
        std::vector<variable_t<S, T>*> input_vars,
        std::string dim_name,
        matrix_t<S>* cov,
        matrix_reducer_t<S>** reducers,
        const size_t num_threads,
        bool is_circular,
        bool is_spectral = false,
        int omegas_len = -1,
        T* omegas = nullptr
    );
    
    std::vector<variable_t<S, T>*> get_eofs(
        std::vector<variable_t<S, T>*> input_vars,
        std::string input_dim,
        dimension_t<T>* eof_dim,
        matrix_t<S>* vt,
        matrix_reducer_t<S>** reducers
    );
    
    
    
public:
    //==========================================================================
    // Public Methods
    //==========================================================================
    
    eof_t();
    
    ~eof_t();
    
    void set_svd(svd_t<T>* svd);

    //void set_interp(interp_t<S>* interp);
    
    //void no_interp();

    std::vector<variable_t<S, T>*> calculate(
        variable_t<S, T>* input_var,
        const std::string input_dim,
        const size_t input_nthreads,
        bool is_circular,
        bool is_spectral = false,
        int omegas_len = -1,
        T* omegas = nullptr
    );
    
    std::vector<variable_t<S, T>*> calculate(
        std::initializer_list<variable_t<S, T>*> vars,
        const std::string input_dim,
        const size_t input_nthreads,
        bool is_circular,
        bool is_spectral = false,
        int omegas_len = -1,
        T* omegas = nullptr
    );
    
    std::vector<variable_t<S, T>*> calculate(
        std::vector<variable_t<S, T>*> input_vars,
        const std::string input_dim,
        const size_t input_nthreads,
        bool is_circular,
        bool is_spectral = false,
        int omegas_len = -1,
        T* omegas = nullptr
    );
    
    /*
    template<template<typename> class L>
    std::vector<variable_t<S, T>*> calculate(
        L<variable_t<S, T>*> vars,
        const std::string input_dim
    );
    */
};





//==============================================================================
// Aliases
//==============================================================================


template<typename T>
using real_eof_t = eof_t<T, T>;

template<typename T>
using complex_eof_t = eof_t<std::complex<T>, T>;





//==============================================================================
// Implementation
//==============================================================================

#include "eof.tpp"

#endif

