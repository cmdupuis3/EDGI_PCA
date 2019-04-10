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

#ifndef REDUCER_HPP
#define REDUCER_HPP

template<typename T>
class variable_reducer_t {
private:
    size_t* dim_sizes;
    
public:
    variable_reducer_t(variable_t<T>* var, std::string dim);
    
    matrix_t<T>* flatten(const variable_t<T>* var, std::string dim) const;
    
    variable_t<T>* restore(const matrix_t<T>* data, dimension_t<T>* dim) const;
};

#endif

