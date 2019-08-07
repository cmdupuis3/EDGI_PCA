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

#include "netcdf_file.hpp"

extern "C" {
    #include "netcdf.h"
}

#include "error.hpp"
#include "debug.hpp"

#include "attributes.hpp"

// <string> included in header
using std::string;


#define NETCDF_ERROR_CHECK(nc_expr) {                                          \
    int _nc_status = nc_expr;                                                  \
    if (_nc_status != NC_NOERR) {                                              \
        throw eof_error_t(string(nc_strerror(_nc_status)));                    \
    }                                                                          \
}

#define NETCDF_ERROR_CHECK_MSG(message, nc_expr) {                             \
    int _nc_status = nc_expr;                                                  \
    if (_nc_status != NC_NOERR) {                                              \
        throw eof_error_t(message + string(nc_strerror(_nc_status)));          \
    }                                                                          \
}





//==============================================================================
// Local functions
//==============================================================================

namespace {

    size_t product(int len, const size_t arr[]) {
        size_t prod = 1;
        for (int i = 0; i < len; i++) {
            prod *= arr[i];
        }
        return prod;
    }

}





//==============================================================================
// Private Methods
//==============================================================================

void netcdf_file_t::init(const string filename, netcdf_mode_t mode) {
    const char* cstr = filename.c_str();

    if (mode == NETCDF_READ) {
        NETCDF_ERROR_CHECK_MSG(
            "Failed to open file \"" + filename + "\". ",
            nc_open(cstr, NC_NOWRITE, &this->file_id)
        );

    } else if (mode == NETCDF_WRITE) {
        try {
            NETCDF_ERROR_CHECK(
                nc_create(cstr, NC_NETCDF4 | NC_CLASSIC_MODEL | NC_NOCLOBBER, &this->file_id)
            );
            this->end_def();

        } catch (eof_error_t) {
            NETCDF_ERROR_CHECK_MSG(
                "Failed to open file \"" + filename + "\". ",
                nc_open(cstr, NC_WRITE, &this->file_id)
            );
        }

    } else if (mode == NETCDF_OVERWRITE) {
        NETCDF_ERROR_CHECK_MSG(
            "Failed to open file \"" + filename + "\". ",
            nc_create(cstr, NC_NETCDF4 | NC_CLASSIC_MODEL | NC_CLOBBER, &this->file_id)
        );
        this->end_def();

    } else {
        throw eof_error_t("Package error: this statement should be unreachable");
    }

    this->close_file = true;
    this->closed = false;
}





//==============================================================================
// Constructors and destructors
//==============================================================================

netcdf_file_t::netcdf_file_t(const string filename, netcdf_mode_t mode) {
    this->init(filename, mode);
}

netcdf_file_t::netcdf_file_t(const string filename) {
    this->init(filename, NETCDF_WRITE);
}

netcdf_file_t::netcdf_file_t(int file_id) {
    this->file_id = file_id;
    this->close_file = false;
    this->closed = false;
}

netcdf_file_t::~netcdf_file_t() {
    if (this->close_file && !this->closed) {
        this->close();
    }
}





//==============================================================================
// Miscellaneous functions
//==============================================================================

int netcdf_file_t::get_file_id() const {
    return this->file_id;
}

void netcdf_file_t::sync() {
    NETCDF_ERROR_CHECK(
        nc_sync(this->get_file_id());
    );
}

void netcdf_file_t::close() {
    NETCDF_ERROR_CHECK(
        nc_close(this->get_file_id())
    );
    this->closed = true;
}

void netcdf_file_t::begin_def() {
    NETCDF_ERROR_CHECK(
        nc_redef(this->get_file_id());
    );
}

void netcdf_file_t::end_def() {
    NETCDF_ERROR_CHECK(
        nc_enddef(this->get_file_id());
    );
}





//==============================================================================
// Attribute functions
//==============================================================================

bool netcdf_file_t::has_fill(netcdf_var_t var) const {
    int no_fill;
    NETCDF_ERROR_CHECK(
        nc_inq_var_fill(this->get_file_id(), (int) var, &no_fill, nullptr)
    );
    return !no_fill;
}

template<typename T>
T netcdf_file_t::get_fill(netcdf_var_t var) const {
    int no_fill;
    T fill_value;
    NETCDF_ERROR_CHECK(
        nc_inq_var_fill(this->get_file_id(), (int) var, &no_fill, (void*) &fill_value)
    );
    if (no_fill) {
        throw eof_error_t("This variable does not have a fill value.");
    }

    return fill_value;
}

template
int netcdf_file_t::get_fill<int>(netcdf_var_t var) const;

template
long netcdf_file_t::get_fill<long>(netcdf_var_t var) const;

template
float netcdf_file_t::get_fill<float>(netcdf_var_t var) const;

template
double netcdf_file_t::get_fill<double>(netcdf_var_t var) const;



template<>
void netcdf_file_t::set_fill<int>(netcdf_var_t var, const int fill_value, const string fill_name){
    NETCDF_ERROR_CHECK(
        nc_put_att_int(this->get_file_id(), (int) var, fill_name.c_str(), NC_INT, 1, &fill_value)
    );
}

template<>
void netcdf_file_t::set_fill<long>(netcdf_var_t var, const long fill_value, const string fill_name){
    NETCDF_ERROR_CHECK(
        nc_put_att_long(this->get_file_id(), (int) var, fill_name.c_str(), NC_LONG, 1, &fill_value)
    );
}

template<>
void netcdf_file_t::set_fill<float>(netcdf_var_t var, const float fill_value, const string fill_name){
    NETCDF_ERROR_CHECK(
        nc_put_att_float(this->get_file_id(), (int) var, fill_name.c_str(), NC_FLOAT, 1, &fill_value)
    );
}

template<>
void netcdf_file_t::set_fill<double>(netcdf_var_t var, const double fill_value, const string fill_name){
    NETCDF_ERROR_CHECK(
        nc_put_att_double(this->get_file_id(), (int) var, fill_name.c_str(), NC_DOUBLE, 1, &fill_value)
    );
}

bool netcdf_file_t::has_attr(netcdf_var_t var, const string name) const {
    int attr;
    int status = nc_inq_attid(this->get_file_id(), (int) var, name.c_str(), &attr);
    if (status == NC_NOERR) {
        return true;
    } else if (status == NC_ENOTATT) {
        return false;
    } else {
        NETCDF_ERROR_CHECK(status);
        return false;
    }
}

bool netcdf_file_t::has_attr(const string name) const {
    return this->has_attr( (netcdf_var_t) NC_GLOBAL, name);
}

netcdf_att_t netcdf_file_t::get_attr(netcdf_var_t var, const string name) const {
    int val;
    NETCDF_ERROR_CHECK(
        nc_inq_attid(this->get_file_id(), (int) var, name.c_str(), &val)
    );
    return (netcdf_att_t) val;
}

netcdf_att_t netcdf_file_t::get_attr(const string name) const {
    return this->get_attr( (netcdf_var_t) NC_GLOBAL, name);
}

void netcdf_file_t::set_attr(netcdf_var_t var, const string name, nc_type type, size_t length, void* val) {
    NETCDF_ERROR_CHECK(
        nc_put_att(this->get_file_id(), (int) var, name.c_str(), type, length, val)
    );
}
void netcdf_file_t::set_attr(const string name, nc_type type, size_t length, void* val) {
    this->set_attr( (netcdf_var_t) NC_GLOBAL, name, type, length, val);
}



//==============================================================================
// Dimension functions
//==============================================================================

bool netcdf_file_t::has_dim(const string name) const {
    int dim;
    int status = nc_inq_dimid(this->get_file_id(), name.c_str(), &dim);
    if (status == NC_NOERR) {
        return true;
    } else if (status == NC_EBADDIM) {
        return false;
    } else {
        NETCDF_ERROR_CHECK(status);
        return false;
    }
}

netcdf_dim_t netcdf_file_t::get_dim(const string name) const {
    int dim;
    NETCDF_ERROR_CHECK(
        nc_inq_dimid(this->get_file_id(), name.c_str(), &dim)
    );
    return (netcdf_dim_t) dim;
}

string netcdf_file_t::get_dim_name(netcdf_dim_t dim) const {
    char cname[NC_MAX_NAME + 1];
    NETCDF_ERROR_CHECK(
        nc_inq_dimname(this->get_file_id(), (int) dim, cname)
    );
    return string(cname);
}

size_t netcdf_file_t::get_dim_len(netcdf_dim_t dim) const {
    size_t len;
    NETCDF_ERROR_CHECK(
        nc_inq_dimlen(this->get_file_id(), (int) dim, &len)
    );
    return len;
}

netcdf_dim_t netcdf_file_t::def_dim(const string name, size_t len) {
    int dim;
    NETCDF_ERROR_CHECK(
        nc_def_dim(this->get_file_id(), name.c_str(), len, &dim)
    );
    return (netcdf_dim_t) dim;
}





//==============================================================================
// Variable functions
//==============================================================================

int netcdf_file_t::get_n_vars() const {
    int n_vars;
    NETCDF_ERROR_CHECK(
        nc_inq_nvars(this->get_file_id(), &n_vars)
    );
    return n_vars;
}

bool netcdf_file_t::has_var(const string name) const {
    int var;
    int status = nc_inq_varid(this->get_file_id(), name.c_str(), &var);
    if (status == NC_NOERR) {
        return true;
    } else if (status == NC_ENOTVAR) {
        return false;
    } else {
        NETCDF_ERROR_CHECK(status);
        return false;
    }
}

netcdf_var_t netcdf_file_t::get_var(const string name) const {
    int var;
    NETCDF_ERROR_CHECK(
        nc_inq_varid(this->get_file_id(), name.c_str(), &var)
    );
    return (netcdf_var_t) var;
}

string netcdf_file_t::get_var_name(netcdf_var_t var) const {
    char cname[NC_MAX_NAME + 1];
    NETCDF_ERROR_CHECK(
        nc_inq_varname(this->get_file_id(), (int) var, cname)
    );
    string name = cname;
    return name;
}

int netcdf_file_t::get_var_n_dims(netcdf_var_t var) const {
    int n_dims;
    NETCDF_ERROR_CHECK(
        nc_inq_varndims(this->get_file_id(), (int) var, &n_dims)
    );
    return n_dims;
}

netcdf_dim_t netcdf_file_t::get_var_dim(netcdf_var_t var, int i) const {
    int dims[this->get_var_n_dims(var)];
    NETCDF_ERROR_CHECK(
        nc_inq_vardimid(this->get_file_id(), (int) var, dims)
    );
    return (netcdf_dim_t) dims[i];
}

netcdf_dim_t* netcdf_file_t::get_var_dims(netcdf_var_t var) const {
    int* dims = new int[this->get_var_n_dims(var)];
    NETCDF_ERROR_CHECK(
        nc_inq_vardimid(this->get_file_id(), (int) var, dims)
    );
    return (netcdf_dim_t*) dims;
}

size_t netcdf_file_t::get_var_len(netcdf_var_t var) const {
    size_t n_dims = this->get_var_n_dims(var);
    int dim_ids[n_dims];
    size_t dim_lens[n_dims];

    NETCDF_ERROR_CHECK(
        nc_inq_vardimid(this->get_file_id(), (int) var, dim_ids)
    );
    for (size_t i = 0; i < n_dims; i++) {
        NETCDF_ERROR_CHECK(
        nc_inq_dimlen(this->get_file_id(), dim_ids[i], &dim_lens[i])
    );
    }
    return product(n_dims, dim_lens);
}

size_t netcdf_file_t::get_var_dim_len(netcdf_var_t var, int i) const {
    return this->get_dim_len(this->get_var_dim(var, i));
}

string netcdf_file_t::get_var_dim_name(netcdf_var_t var, int i) const {
    return this->get_dim_name(this->get_var_dim(var, i));
}

template<>
netcdf_var_t netcdf_file_t::def_var<int>(const string name, size_t n_dims, const netcdf_dim_t* dim_ids) {
    int var;
    NETCDF_ERROR_CHECK(
        nc_def_var(this->get_file_id(), name.c_str(), NC_INT, (int) n_dims, (const int*) dim_ids, &var)
    );
    return (netcdf_var_t) var;
}

template<>
netcdf_var_t netcdf_file_t::def_var<long>(const string name, size_t n_dims, const netcdf_dim_t* dim_ids) {
    int var;
    NETCDF_ERROR_CHECK(
        nc_def_var(this->get_file_id(), name.c_str(), NC_LONG, (int) n_dims, (const int*) dim_ids, &var)
    );
    return (netcdf_var_t) var;
}

template<>
netcdf_var_t netcdf_file_t::def_var<float>(const string name, size_t n_dims, const netcdf_dim_t* dim_ids) {
    int var;
    NETCDF_ERROR_CHECK(
        nc_def_var(this->get_file_id(), name.c_str(), NC_FLOAT, (int) n_dims, (const int*) dim_ids, &var)
    );
    return (netcdf_var_t) var;
}

template<>
netcdf_var_t netcdf_file_t::def_var<double>(const string name, size_t n_dims, const netcdf_dim_t* dim_ids) {
    int var;
    NETCDF_ERROR_CHECK(
        nc_def_var(this->get_file_id(), name.c_str(), NC_DOUBLE, (int) n_dims, (const int*) dim_ids, &var)
    );
    return (netcdf_var_t) var;
}

template<>
void netcdf_file_t::set_var_vals<int>(netcdf_var_t var, const int* vals) {
    NETCDF_ERROR_CHECK(
        nc_put_var_int(this->get_file_id(), (int) var, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<long>(netcdf_var_t var, const long* vals) {
    NETCDF_ERROR_CHECK(
        nc_put_var_long(this->get_file_id(), (int) var, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<float>(netcdf_var_t var, const float* vals) {
    NETCDF_ERROR_CHECK(
        nc_put_var_float(this->get_file_id(), (int) var, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<double>(netcdf_var_t var, const double* vals) {
    NETCDF_ERROR_CHECK(
        nc_put_var_double(this->get_file_id(), (int) var, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<int>(netcdf_var_t var, const int* vals, const size_t* start, const size_t* count) {
    NETCDF_ERROR_CHECK(
        nc_put_vara_int(this->get_file_id(), (int) var, start, count, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<long>(netcdf_var_t var, const long* vals, const size_t* start, const size_t* count) {
    NETCDF_ERROR_CHECK(
        nc_put_vara_long(this->get_file_id(), (int) var, start, count, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<float>(netcdf_var_t var, const float* vals, const size_t* start, const size_t* count) {
    NETCDF_ERROR_CHECK(
        nc_put_vara_float(this->get_file_id(), (int) var, start, count, vals)
    );
}

template<>
void netcdf_file_t::set_var_vals<double>(netcdf_var_t var, const double* vals, const size_t* start, const size_t* count) {
    NETCDF_ERROR_CHECK(
        nc_put_vara_double(this->get_file_id(), (int) var, start, count, vals)
    );
}

template<>
int* netcdf_file_t::get_var_vals<int>(netcdf_var_t var) const {
    int* vals = new int[this->get_var_len(var)];
    NETCDF_ERROR_CHECK(
        nc_get_var_int(this->get_file_id(), (int) var, vals)
    );
    return vals;
}

template<>
long* netcdf_file_t::get_var_vals<long>(netcdf_var_t var) const {
    long* vals = new long[this->get_var_len(var)];
    NETCDF_ERROR_CHECK(
        nc_get_var_long(this->get_file_id(), (int) var, vals)
    );
    return vals;
}

template<>
float* netcdf_file_t::get_var_vals<float>(netcdf_var_t var) const {
    float* vals = new float[this->get_var_len(var)];
    NETCDF_ERROR_CHECK(
        nc_get_var_float(this->get_file_id(), (int) var, vals)
    );
    return vals;
}

template<>
double* netcdf_file_t::get_var_vals<double>(netcdf_var_t var) const {
    double* vals = new double[this->get_var_len(var)];
    NETCDF_ERROR_CHECK(
        nc_get_var_double(this->get_file_id(), (int) var, vals)
    );
    return vals;
}

template<>
int* netcdf_file_t::get_var_vals<int>(netcdf_var_t var, const size_t* start, const size_t* count) const {
    size_t size = product(this->get_var_n_dims(var), count);
    int* vals = new int[size];
    NETCDF_ERROR_CHECK(
        nc_get_vara_int(this->get_file_id(), (int) var, start, count, vals)
    );
    return vals;
}

template<>
long* netcdf_file_t::get_var_vals<long>(netcdf_var_t var, const size_t* start, const size_t* count) const {
    size_t size = product(this->get_var_n_dims(var), count);
    long* vals = new long[size];
    NETCDF_ERROR_CHECK(
        nc_get_vara_long(this->get_file_id(), (int) var, start, count, vals)
    );
    return vals;
}

template<>
float* netcdf_file_t::get_var_vals<float>(netcdf_var_t var, const size_t* start, const size_t* count) const {
    size_t size = product(this->get_var_n_dims(var), count);
    float* vals = new float[size];
    NETCDF_ERROR_CHECK(
        nc_get_vara_float(this->get_file_id(), (int) var, start, count, vals)
    );
    return vals;
}

template<>
double* netcdf_file_t::get_var_vals<double>(netcdf_var_t var, const size_t* start, const size_t* count) const {
    size_t size = product(this->get_var_n_dims(var), count);
    double* vals = new double[size];
    NETCDF_ERROR_CHECK(
        nc_get_vara_double(this->get_file_id(), (int) var, start, count, vals)
    );
    return vals;
}

