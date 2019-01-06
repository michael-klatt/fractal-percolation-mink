/*
 * init.h
 *
 *  Created on: Dec 10, 2018
 *      Author: mklatt
 */

#ifndef INIT_H_
#define INIT_H_

#include <string>
//#include <algorithm>

#include <boost/program_options.hpp>
namespace progopt = boost::program_options;

const std::string VERSION = "19.01";

/* Initialize parameters reading from command line or configuration file
 *
 * Default values of parameters are overridden if provided in a
 * configuration file or via the command line
 *
 * parameter: config_file       Configuration file to read parameters from
 * parameter: prefix_of         Folder for output, i.e. prefix for output files
 * parameter: p                 Fractal percolation: survival probability
 * parameter: subdivision       Fractal percolation: Parameter M of subdivisions
 * parameter: n_approximations  Fractal percolation: Level of approximation
 * parameter: N_runs            Fractal percolation: Number of simulation runs
 * parameter: imageout          Flag whether a pgm image shall be created
 * parameter: seed              Seed of the random number generator
 */
void initialize(int clc, char* clv[],
               std::string &config_file,
               std::string &prefix_of,
               double &p,
               unsigned &subdivision,
               unsigned &n_approximations,
               unsigned &N_runs,
               bool &imageout,
               unsigned &seed);


#endif /* INIT_H_ */
