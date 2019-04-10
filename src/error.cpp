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

#include "error.hpp"

#include <execinfo.h>
#include <unistd.h>

#include <string.h>
// strdup

// <cstdlib> included in header
// size_t

// <string> included in header
using std::string;





void backtrace() {
    // get void*'s for all entries on the stack
    void *array[BACKTRACE_CAPACITY];
    size_t size = backtrace(array, BACKTRACE_CAPACITY);
    
    // print out all the frames to stderr
    std::cerr << "Backtrace:" << std::endl;
    backtrace_symbols_fd(array, size, STDERR_FILENO);
}





void eof_error_t::init(string msg) {
    this->msg = msg;
}

eof_error_t::eof_error_t(const char* msg) {
    this->init(string(msg));
}

eof_error_t::eof_error_t(string msg) {
    this->init(msg);
}

eof_error_t::~eof_error_t() throw() { }

const std::string eof_error_t::str() const throw() {
    return "EOF Analysis Error: " + this->msg;
}

const char* eof_error_t::what() const throw() {
    return strdup(this->str().c_str());
}

