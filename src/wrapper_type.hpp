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

#ifndef WRAPPER_TYPE_HPP
#define WRAPPER_TYPE_HPP


/**
 * This macro defines a new wrapper type which is a strict subtype of another.
 * The two types have interchangeable values, but they must be explicitly
 * casted back and forth.
 *
 * NOTE: The following code is almost certainly safe in practice, but it is
 * technically undefined behavior. If an architecture/compiler fails when
 * attempting to run this, please change to a sane one and try again.
 * 
 * ```
 * WRAPPER_TYPE(myint, int)
 * 
 * int main() {
 *     int   a = 1;
 *     myint b = *(myint*) &a;
 *     int   c = *(int*) &b;
 *     
 *     assert(a == c);
 * }
 * ```
 * 
 * The wrapper type defines the following methods (where wrapper_t and base_t
 * represent the names of the wrapper and base types respectively):
 *   - wrapper_t()
 *      // for creating a default wrapper
 *
 *   - explicit wrapper_t(base_t)
 *      // for creating a wrapper from a base value
 *
 *   - explicit operator base_t() const
 *      // for casting a wrapper to its base value
 *
 *   - bool operator ==(const wrapper_t& other) const
 *      // for comparing wrapper values for equality
 *
 *   - bool operator !=(const wrapper_t& other) const
 *      // for comparing wrapper values for inequality
 */
#define WRAPPER_TYPE(wrapper_t, base_t)                                        \
struct wrapper_t {                                                             \
private:                                                                       \
    base_t value;                                                              \
                                                                               \
public:                                                                        \
    wrapper_t() { }                                                            \
                                                                               \
    explicit wrapper_t(base_t value) {                                         \
        this->value = value;                                                   \
    }                                                                          \
                                                                               \
    explicit operator base_t() const {                                         \
        return this->value;                                                    \
    }                                                                          \
                                                                               \
    bool operator ==(const wrapper_t& other) const {                           \
        return this->value == other.value;                                     \
    }                                                                          \
                                                                               \
    bool operator !=(const wrapper_t& other) const {                           \
        return this->value != other.value;                                     \
    }                                                                          \
};                                                                             \
                                                                               \
static_assert(sizeof(wrapper_t) == sizeof(base_t),                             \
    "Creation of wrapper type failed: "                                        \
    "sizeof(" #wrapper_t ") != sizeof(" #base_t ")"                            \
);





#endif

