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

#ifndef ERROR_HPP
#define ERROR_HPP

/** Use size_t */
#include <cstdlib>

/** Use std::string */
#include <string>

/** Use std::stringstream */
#include <sstream>

/** Use std::cout, std::endl */
#include <iostream>

/** Use std::exception */
#include <exception>





/**
 * Maximum size of the backtrace
 */
const size_t BACKTRACE_CAPACITY = 64;

/**
 * Print the current backtrace to stderr
 */
void backtrace();





#define TODO {                                                      \
    FATAL("This code is not yet implemented")                                  \
}

#define UNDEFINED(type) ([]{                                                   \
    FATAL("This value is not yet defined")                                     \
    return *(type*) 0;                                                         \
}())

#define FATAL(message) {                                                       \
    std::cout << "FATAL: EOF Analysis error at "                               \
            << __FILE__ << ":" << __LINE__ << std::endl                        \
            << "    " << message << std::endl;                                 \
    std::stringstream s;                                                       \
    s << message;                                                              \
    backtrace();                                                               \
    throw eof_error_t(s.str());                                                \
}

#define WARNING(message) {                                                     \
    std::cout << "WARNING: EOF Analysis warning at "                           \
            << __FILE__ << ":" << __LINE__ << std::endl                        \
            << "    " << message << std::endl;                                 \
}

#define NOTE(message) {                                                        \
    std::cout << "NOTE: EOF Analysis note at "                                 \
            << __FILE__ << ":" << __LINE__ << std::endl                        \
            << "    " << message << std::endl;                                 \
}





class eof_error_t : public std::exception {
private:
    std::string msg;
    
    void init(std::string msg);
    
public:
    eof_error_t(const char* msg);
    eof_error_t(std::string msg);
    
    ~eof_error_t() throw();
    
    const std::string str() const throw();
    
    const char* what() const throw();
};





#endif

