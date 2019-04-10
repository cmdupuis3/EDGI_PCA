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

/**
 * This module is used specifically for debugging and should not be included in
 * production compilations.
 */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifdef DEBUG
    
    /** Use size_t */
    #include <stdlib.h>
    
    /** Use std::cout, std::endl */
    #include <iostream>
    
    
    
    
    /**
     * The prefix to show before debugging messages
     */
    #define DEBUG_PREFIX "[DEBUG] "
    
    /**
     * Counts the number of times this statement is encountered in the local scope,
     * printing out the occurrance number each time.
     */
    #define COUNT {                                                             \
        static int _count_ = 0;                                                 \
        std::cout << DEBUG_PREFIX << "count = " << _count_ << std::endl;        \
        _count_++;                                                              \
    }
    
    /**
     * Prints the current location (file and line number) when encountered.
     */
    #define HERE {                                                              \
        std::cout << DEBUG_PREFIX << "HERE"                                     \
            << " at "  << __FILE__                                              \
            << ":"    << __LINE__                                               \
            << " in " << __func__                                               \
            << std::endl;                                                       \
    }
    
    /**
     * Print _printa_a string or value.
     */
    #define SAY(_printa_aux_a) {                                                \
        std::cout << DEBUG_PREFIX << _printa_aux_a << std::endl;                \
    }
    
    /**
     * Print the name and value of _printa_a variable or expression.
     */
    #define PRINT(_printa_aux_a) {                                              \
        std::cout                                                               \
            << DEBUG_PREFIX << #_printa_aux_a                                   \
            << " := " << (_printa_aux_a) << std::endl;                          \
    }
    
    /**
     * Helper function for printing _printa_a one-dimensional array
     */
    #define PRINTA_AUX(_a, _len) {                                              \
        auto _printa_aux_a = _a;                                                \
        auto _printa_aux_len = _len;                                            \
        std::cout << "[";                                                       \
        if (_printa_aux_len > 0) std::cout << _printa_aux_a[0];                 \
        for (size_t i = 1; i < _printa_aux_len; i++)                            \
            std::cout << ", " << _printa_aux_a[i];                              \
        std::cout << "]" << std::endl;                                          \
    }
    
    /**
     * Print the name and value of _printa_a one-dimensional array with length len.
     */
    #define PRINTA(_a, _size) {                                                 \
        auto _printa_a = _a;                                                    \
        auto _printa_size = _size;                                              \
        std::cout << DEBUG_PREFIX << #_a << " := ";                             \
        PRINTA_AUX(_printa_a, _printa_size)                                     \
    }
    
    /**
     * Print the name and value of _printa_a two-dimensional array (matrix) with the
     * specified number of _printm_rows and columns.
     */
    #define PRINTM(_m, _rows, _cols) {                                          \
        auto _printm_m = _m;                                                    \
        auto _printm_rows = _rows;                                              \
        auto _printm_cols = _cols;                                              \
        std::cout << DEBUG_PREFIX << #_m << " := [";                            \
        if (_printm_rows > 0) {                                                 \
            PRINTA_AUX(_printm_m[0], _printm_cols)                              \
        }                                                                       \
        for (size_t r = 1; r < _printm_rows; r++) {                             \
            std::cout << std::endl << ", ";                                     \
            PRINTA_AUX(_printm_m[r], _printm_cols);                             \
        }                                                                       \
        std::cout << "]" << std::endl;                                          \
    }
    
#else
    
    #define DEBUG_FAIL static_assert(false, "Debugging code left in _printa_a production build");
    
    #define COUNT                    DEBUG_FAIL
    #define HERE                     DEBUG_FAIL
    #define SAY(_a)                  DEBUG_FAIL
    #define PRINT(_a)                DEBUG_FAIL
    #define PRINTA(_a, _size)        DEBUG_FAIL
    #define PRINTM(_m, _rows, _cols) DEBUG_FAIL
    
#endif



#endif

