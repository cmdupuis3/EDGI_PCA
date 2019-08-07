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

#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
#include "src/eof_analysis.hpp"
#include "src/debug.hpp"

#ifdef WITH_PLASMA
    #include "linalg/plasma_svd.hpp"
    #define SVD_TYPE plasma_svd_t
#elif WITH_MKL
    #include "linalg/mkl_svd.hpp"
    #define SVD_TYPE mkl_svd_t
#elif WITH_OPENBLAS
    #include "linalg/openblas_svd.hpp"
    #define SVD_TYPE openblas_svd_t
#endif

#include "src/fftw_fft.hpp"


using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::stoi;

int example_1(int argc, char** argv);
int example_2(int argc, char** argv);
int basic_interface(int argc, char** argv);

int main(int argc, char** argv) {
    return basic_interface(argc, argv);
}





int example_1(int argc, char** argv) {
    if (argc <= 3) {
        cerr << "Not enough arguments" << endl;
        return 1;
    }

    string file_name(argv[1]);
    string var_name(argv[2]);
    string dim_name(argv[3]);

    // Open the file
    netcdf_file_t file(file_name, NETCDF_WRITE);

    // Load the input data
    real_variable_t<float> var = real_variable_t<float>(var_name, &file);

    // Calculate the EOFs
    real_eof_t<float> eof;
    eof.set_svd(new SVD_TYPE<float>(32));
    vector<real_variable_t<float>*> vars = eof.calculate({&var}, dim_name, 32, false);

    // Write the output data
    vars[0]->write(var_name + "_eof", &file);
    delete vars[0];

    return 0;
}

int example_2(int argc, char** argv) {
    if (argc <= 6) {
        cerr << "Not enough arguments" << endl;
        return 1;
    }

    string file_in_name(argv[1]);
    string file_out_name(argv[2]);
    string real_name(argv[3]);
    string imag_name(argv[4]);
    string out_name(argv[5]);
    string dim_name(argv[6]);

    // Open the input file and load the data
    netcdf_file_t file_in(file_in_name, NETCDF_READ);
    real_variable_t<float> real = real_variable_t<float>(real_name, &file_in);
    real_variable_t<float> imag = real_variable_t<float>(imag_name, &file_in);
    complex_variable_t<float>* var = make_complex_variable(&real, &imag);

    // Calculate the EOFs
    complex_eof_t<float> eof;
    eof.set_svd(new SVD_TYPE<float>(32));
    vector<complex_variable_t<float>*> vars = eof.calculate({var}, dim_name, 32, false);

    // Open the output file and write the resulting EOFs
    netcdf_file_t file_out(file_out_name, NETCDF_WRITE);
    vars[0]->write_complex(out_name + "_eof_re", out_name + "_eof_im", &file_out);

    // Clean up
    delete var;
    delete vars[0];

    return 0;
}





vector<string> split(string s, char c) {
    vector<string> words;
    string word;
    std::istringstream stream(s);
    while(std::getline(stream, word, c)) {
        words.push_back(word);
    }
    return words;
}

vector<string> to_str_vec(char** arr, size_t len) {
    vector<string> vec;
    for (size_t i = 0; i < len; i++) {
        vec.push_back(string(arr[i]));
    }
    return vec;
}

struct arg_data_t {
    string dim_in;
    vector<string> vars_in;
    vector<string> vars_out;
    vector<string> cvars_in;
    vector<string> cvars_out;
    bool is_circular;
    bool is_spectral;
    string freq_name;
    bool do_hilbert;
    vector<string> files_in;
    vector<string> files_out;
    size_t ncores_in;
};

bool parse_args(vector<string> argv, arg_data_t* data) {
    data->dim_in = "";
    data->do_hilbert = false;
    data->is_spectral = false;
    data->is_circular = false;

    enum {
        ARG_NONE,
        ARG_DIM,
        ARG_VAR,
        ARG_CVAR,
        ARG_FILE,
        ARG_NCORES
    } state = ARG_NONE;

    for (string arg : argv) {
        if (arg[0] == '-') {
            if (arg == "-d") {
                state = ARG_DIM;
            } else if (arg == "-v") {
                state = ARG_VAR;
            } else if (arg == "-c") {
                state = ARG_CVAR;
            } else if (arg == "-C") {
                data->is_circular = true;
            } else if (arg == "-S") {
                data->is_spectral = true;
            } else if (arg == "-H") {
                data->do_hilbert = true;
            } else if (arg == "-f") {
                state = ARG_FILE;
            } else if (arg == "-n") {
                state = ARG_NCORES;
            } else if (arg == "-h") {
                return false;
            } else {
                cerr << "[ERROR] Unknown flag: '" << arg << "'" << endl;
                return false;
            }
            continue;
        }



        if (state == ARG_DIM) {
            data->dim_in = arg;

        } else if (state == ARG_VAR) {
            vector<string> words = split(arg, ':');
            size_t size = words.size();
            if (size > 2) {
                cerr << "[ERROR] Invalid variable name format: '" << arg << "'" << endl;
                return false;
            } else {
                data->vars_in.push_back(words[0]);
                data->vars_out.push_back(words[size - 1]);
            }

        } else if (state == ARG_CVAR) {
            vector<string> words = split(arg, ':');
            size_t size = words.size();
            if (size > 2) {
                cerr << "[ERROR] Invalid variable name format: '" << arg << "'" << endl;
                return false;
            } else {
                data->cvars_in.push_back(words[0]);
                data->cvars_out.push_back(words[size - 1]);
            }

        } else if (state == ARG_FILE) {
            vector<string> words = split(arg, ':');
            size_t size = words.size();
            if (size > 2) {
                cerr << "[ERROR] Invalid file name format: '" << arg << "'" << endl;
                return false;
            } else {
                data->files_in.push_back(words[0]);
                data->files_out.push_back(words[size - 1]);
            }

        } else if (state == ARG_NCORES) {
            data->ncores_in = stoi(arg);
            omp_set_num_threads(data->ncores_in);

        } else {
            cerr << "[ERROR] Expected a flag '-f', '-v', '-c', '-C', '-S', '-H', '-d', or '-n'." << endl;
            return false;
        }
    }

    if(data->is_spectral){
        data->freq_name = data->dim_in;
    }

    // Check that number of -c variables is zero or equal to -v
    if(data->cvars_in.size() != 0 &&
       data->cvars_in.size() != data->vars_in.size()){
        cerr << "[ERROR] Mismatch between number of real and imaginary component variables." << endl;
        return false;
    }

    // -c requires real data
    if(data->is_circular &&
       data->cvars_in.size() != 0){
        cerr << "[ERROR] Circular covariance kernel can only be used with real-valued data currently." << endl;
        return false;
    }

    // -H requires real data
    if(data->do_hilbert &&
       data->cvars_in.size() != 0){
        cerr << "[ERROR] Analytic signals can only be generated for real-valued data currently." << endl;
        return false;
    }

    // -S requires complex data
    if(data->is_spectral &&
       data->cvars_in.size() == 0){
        cerr << "[ERROR] Spectral data must have real and imaginary components." << endl;
        return false;
    }

    if (data->dim_in == "") {
        cerr << "[ERROR] No dimension specified." << endl;
        return false;
    } else if (data->vars_in.size() == 0) {
        cerr << "[ERROR] No variables specified." << endl;
        return false;
    } else if (data->files_in.size() == 0) {
        cerr << "[ERROR] No files specified." << endl;
        return false;
    }

    return true;
}

void usage(string cmd) {
    cerr << endl;
    cerr << "Usage:" << endl;
    cerr << "    " << cmd << " <options>" << endl;
    cerr << endl;
    cerr << "Options:" << endl;
    cerr << "    -h                        Show this help message." << endl;
    cerr << "    -f <i>:<o> ... (required) Read data from file <i> and write to file <o>. Multiple" << endl;
    cerr << "                              <i>:<o> pairs can be specified and separated by spaces." << endl;
    cerr << "    -v <i>:<o> ... (required) Calculate EOFs on variable <i> and output as variable <o>. Multiple" << endl;
    cerr << "                              <i>:<o> pairs can be specified and separated by spaces" << endl;
    cerr << "    -c <i>:<o> ... (optional) Add imaginary component <i> to variable and output as <o>. Number" << endl;
    cerr << "                              of <i>:<o> pairs must equal number of variable <i>:<o> pairs." << endl;
    cerr << "    -C         ... (optional) Flag for circular data. Calculates circular covariance instead of" << endl;
    cerr << "                              regular covariance, which will enhance EOFs for circular variables." << endl;
    cerr << "                              Units assumed to be radians. For use with real-valued data." << endl;
    cerr << "    -S         ... (optional) Flag for spectral data. Assumes -d refers to an angular frequency" << endl;
    cerr << "                              variable, and weights covariance calculation for T-series accordingly. " << endl;
    cerr << "    -H         ... (optional) Calculate analytic signal before running. Uses a Hilbert transform on" << endl;
    cerr << "                              a real- or complex-valued variable to generate complex-valued results." << endl;
    cerr << "    -d <i>     ... (required) Time dimension name." << endl;
    cerr << "    -n <i>     ... (required) Set the number of cores to use." << endl;
    cerr << endl;
}

// Sample usage:
//     bin/main.x -f sample.nc:sample_eofs.nc -v a:a_eof b:b_eof c:c_eof -ai:ai_eof bi:bi_eof ci:ci_eof -d time:eof_coef

int basic_interface(int argc, char** argv) {
    arg_data_t args;
    if (!parse_args(to_str_vec(argv + 1, argc - 1), &args)) {
        usage(string(argv[0]));
        return 1;
    }

    cout << endl << args.ncores_in << " cores: ";

    time_t start = time(nullptr);
    time_t rstart = time(nullptr); // reading time

    if (!args.do_hilbert && args.cvars_in.size() == 0) {
        vector<real_variable_t<float>*> vars_in;
        for (string filename : args.files_in) {
            netcdf_file_t file(filename, NETCDF_READ);
            for (string varname : args.vars_in) {
                vars_in.push_back(new real_variable_t<float>(varname, &file));
            }
        }

        time_t rend = time(nullptr);
        double rtime = difftime(rend,rstart);
        cout << "nc_in: " << rtime << "s; ";

        // Calculate the eofs with n cores using PLASMA
        real_eof_t<float> eof;
        eof.set_svd(new SVD_TYPE<float>(args.ncores_in));
        //eof.set_svd(new mkl_svd_t<float>(args.ncores_in));
        vector<real_variable_t<float>*> vars_out = eof.calculate(vars_in, args.dim_in, args.ncores_in, args.is_circular);

        time_t wstart = time(nullptr); // writing time

        size_t i = 0;
        for (string filename : args.files_out) {
            netcdf_file_t file(filename, NETCDF_OVERWRITE);

            for (string varname : args.vars_out) {
                vars_out[i]->write(varname, &file);

                // Clean up
                delete vars_in[i];
                delete vars_out[i];

                i++;
            }
        }

        time_t wend = time(nullptr);
        double wtime = difftime(wend,wstart);
        cout << "nc_out: " << wtime << "s; ";

    }else{

        vector<complex_variable_t<float>*> vars_in;
        if (args.do_hilbert) {

            vector<real_variable_t<float>*> rvars_in_raw;
            vector<real_variable_t<float>*> cvars_in_raw;
            vector<complex_variable_t<float>*> vars_in_raw;
            for (string filename : args.files_in) {
                netcdf_file_t file(filename, NETCDF_READ);

                for (string varname : args.vars_in) {
                    rvars_in_raw.push_back(new real_variable_t<float>(varname, &file));
                }

                if (args.cvars_in.size() != 0) {
                    for (string cvarname : args.cvars_in) {
                        cvars_in_raw.push_back(new real_variable_t<float>(cvarname, &file));
                    }
                    for (int v = 0; v < args.vars_in.size(); v++) {
                        vars_in_raw.push_back(make_complex_variable(rvars_in_raw.at(v), cvars_in_raw.at(v)));
                    }
                }
            }

            if (args.cvars_in.size() == 0) {
                real_spectrum_t<float> spec;
                spec.set_dft(new fftw_fft_t<float>(args.ncores_in));
                vars_in = spec.analytic(rvars_in_raw, args.dim_in, args.ncores_in);
            }else{
                complex_spectrum_t<float> spec;
                spec.set_dft(new fftw_fft_t<float>(args.ncores_in));
                vars_in = spec.analytic(vars_in_raw, args.dim_in, args.ncores_in);
            }
        }else{
            vector<real_variable_t<float>*> rvars_in;
            vector<real_variable_t<float>*> cvars_in;
            for (string filename : args.files_in) {
                netcdf_file_t file(filename, NETCDF_READ);
                for (string varname : args.vars_in) {
                    rvars_in.push_back(new real_variable_t<float>(varname, &file));
                }
                for (string cvarname : args.cvars_in) {
                    cvars_in.push_back(new real_variable_t<float>(cvarname, &file));
                }
                for (int v = 0; v < args.vars_in.size(); v++) {
                    vars_in.push_back(make_complex_variable(rvars_in.at(v), cvars_in.at(v)));
                }
            }
        }

        /** Load frequency values if data is spectral */
        float* omegas = nullptr;
        int omegas_len = -1;
        if(args.is_spectral){
            netcdf_file_t file(args.files_in[0], NETCDF_READ);
            netcdf_var_t var_id = file.get_var(args.freq_name);
            netcdf_dim_t dim_id = file.get_var_dim(var_id, 0);
            omegas_len = file.get_dim_len(dim_id);
            omegas = new float[omegas_len];
            omegas = file.get_var_vals<float>(var_id);
        }

        time_t rend = time(nullptr);
        double rtime = difftime(rend,rstart);
        cout << "nc_in: " << rtime << "s; ";

        // Calculate the eofs with n cores using PLASMA
        complex_eof_t<float> eof;
        eof.set_svd(new SVD_TYPE<float>(args.ncores_in));
        //eof.set_svd(new mkl_svd_t<float>(args.ncores_in));
        vector<complex_variable_t<float>*> vars_out = eof.calculate(vars_in, args.dim_in, args.ncores_in, args.is_circular, args.is_spectral, omegas_len, omegas);


        time_t wstart = time(nullptr); // writing time

        size_t i = 0;
        for (string filename : args.files_out) {
            netcdf_file_t file(filename, NETCDF_OVERWRITE);

            for (int i = 0; i < args.vars_out.size(); i++) {

                if (args.do_hilbert) {
                    vars_out[i]->write_complex(args.vars_out[i] + "_re", args.vars_out[i] + "_im", &file);
                } else {
                    vars_out[i]->write_complex(args.vars_out[i], args.cvars_out[i], &file);
                }

                // Clean up
                delete vars_in[i];
                delete vars_out[i];

                i++;
            }
        }

        time_t wend = time(nullptr);
        double wtime = difftime(wend,wstart);
        cout << "nc_out: " << wtime << "s; ";
    }

    time_t end = time(nullptr);
    double time = difftime(end,start);
    cout << "main: " << time << "s" << endl;

    return 0;
}

