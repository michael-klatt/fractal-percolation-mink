/*
 * init.cpp
 *
 *  Created on: Dec 10, 2018
 *      Author: mklatt
 */

#include <iostream>
#include <fstream>

#include "init.h"

void initialize(int clc, char* clv[],
               std::string &config_file,
               std::string &prefix_of,
               double &p,
               unsigned &subdivision,
               unsigned &n_approximations,
               unsigned &N_runs,
               bool &imageout,
               unsigned &seed)
{
  try{
      progopt::options_description generic("Generic options");
      generic.add_options()
          ("help,h", "Print options")
          ("version", "Print version number")
          ("config", progopt::value<std::string>(&config_file)->default_value(config_file),
              "Configuration file")
              ;
      progopt::options_description config("Configuration");
      config.add_options()
          ("prefix_of,o",        progopt::value<std::string>(&prefix_of)->default_value(prefix_of),            "Set prefix for output files")
          ("survival_prob,p",    progopt::value<double>(&p)->default_value(p),                                 "Probability of survival of each cell in each iteration")
          ("subdivision,M",      progopt::value<unsigned>(&subdivision)->default_value(subdivision),           "Number of subdivisions")
          ("n_approximations,n", progopt::value<unsigned>(&n_approximations)->default_value(n_approximations), "Number of approximations")
          ("Nruns,R",            progopt::value<unsigned>(&N_runs)->default_value(N_runs),                     "Number of simulation runs")
          ("image,i",            progopt::value<bool>(&imageout)->default_value(imageout),                     "Set whether or not to print an image")
          ("seed,s",             progopt::value<unsigned>(&seed)->default_value(seed),                         "Set seed of random number generators")
          ;

      progopt::options_description cmdlineopt("\nMandelbrot Percolation - Options");
      cmdlineopt.add(generic).add(config);
      progopt::options_description confopt;
      confopt.add(config);

      progopt::variables_map varmap;
      progopt::store(progopt::parse_command_line(clc, clv, cmdlineopt), varmap);
      progopt::notify(varmap);
      // Reading config file
      std::ifstream readconf( config_file.c_str() );
      if(readconf.fail()){
          std::cerr << "ERROR: ifstream failed to read the configuration file " << config_file << ";" << std::endl;
          exit(-1);
      }
      else{
          progopt::store(parse_config_file(readconf, confopt), varmap);
          progopt::notify(varmap);
      }
      readconf.close();

      if(varmap.count("help")){
          std::cout << cmdlineopt << std::endl;
          exit(0);
      }
      if(varmap.count("version")) {
          std::cout << std::endl
              << "# ------------------------------------------------ " << std::endl
              << "# - Mandelbrot Percolation: Euler Characteristic - " << std::endl
              << "# ------------------------------------------------ " << std::endl << std::endl;

          std::cout << "Version: " << VERSION << std::endl << std::endl;
          exit(0);
      }

      std::cout << std::endl
          << "# ------------------------------------------------ " << std::endl
          << "# - Mandelbrot Percolation: Euler Characteristic - " << std::endl
          << "# ------------------------------------------------ " << std::endl << std::endl;

      std::cout << "# Version: " << VERSION << std::endl
          << std::endl
          << "# Probability of survival of each cell in each iteration: p = " << p << std::endl
          << "# Number of subdivisions:                                 subdivision = " << subdivision << std::endl
          << "# Number of approximations:                               n_approximations = " << n_approximations << std::endl
          << "# Number of simulation runs:                              N_runs = " << N_runs << std::endl
          << "# Print an image to a pgm-file:                           imageout = " << imageout << std::endl
          << "# Seed of random number generators:                       seed = " << seed << std::endl
          << "# Configuration file:                                     config = " << config_file << std::endl
          << "# Prefix for output files:                                prefix_of = " << prefix_of << std::endl
          << std::endl;

  }
  catch(std::exception& error){
      std::cerr << "# ERROR: " << error.what() << std::endl;
      exit(-1);
  }
  catch(...) {
      std::cerr << "# Exception of unknown type!\n";
      exit(-1);
  }

}


