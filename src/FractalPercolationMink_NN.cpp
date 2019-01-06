/* FractalPercolationMink_NN
 *
 * All clusters connecting only nearest neighbors
 *
 * Author Michael Andreas Klatt (software@mklatt.org)
 * Released under the GNU General Public License, version 3.
 *
 * Approximate fractal percolation and estimate its expected Euler
 * characteristic
 *
 * Description
 * C++ code to simulate finite approximations of fractal percolation and to
 * analyze their geometry by estimating the expected Euler characteristic, see
 * M.A. Klatt and S. Winter "Geometric functionals of fractal percolation".
 *
 * More generally the code can simulate pixelated random sets and compute the
 * Minkowski functionals or intrinsic volumes (area, perimeter, and Euler
 * characteristic) as well as their generalization to Minkowski tensors to
 * quantify anisotropy.
 */

#include "init.h"
#include "minkowski.h"

static std::string config_file = "FractalPercolationMink.conf";
static std::string prefix_of = "output/";
static double p = 0.5;
static unsigned subdivision = 3;
static unsigned n_approximations = 3;
static unsigned N_runs = 100;
static bool imageout = false;
static unsigned seed = 17;

int main(int clc, char* clv[]){
  // Read in parameters
  initialize(clc, clv, config_file, prefix_of, p, subdivision, n_approximations, N_runs, imageout, seed);
  // update_seed 
  seed *= int(p*100+1e-10);
  seed *= 1000;
  seed += subdivision*100;
  seed += n_approximations;
  seed *= 100000;
  seed += N_runs; 

  // black = true = death
  // white = false = no death = survival
  double p_turning_black = 1 - p;
  
  // linear size of approximation
  int final_Mx = pow(subdivision,n_approximations);

  double mean_actual_chi = 0;
  double std_error_actual_chi = 0;

  for(unsigned run = 0; run < N_runs; run++){
    BinField<bool> final_approximation(final_Mx, false);

    for(unsigned k = 1; k <= n_approximations; k++){
      int tmp_size = pow(subdivision,k);
      BinField<bool> tmp_Bernoulli(tmp_size,false);
      RandomBernoulliBinField_const_p(tmp_Bernoulli, p_turning_black, seed);

      for(int xi = 0; xi < tmp_size; xi++)
        for(int yi = 0; yi < tmp_size; yi++)
          if(tmp_Bernoulli.call(xi,yi)){ // if cell dies
            // how many small pixels in final approximation
            // correspond to one pixel in this k-th approximation?
            int h = pow(subdivision,n_approximations-k);

            // in the k-th approximation a square is set to false
            // all offsprings die in final_approximation
            for(int final_xi = xi*h; final_xi < (xi+1)*h; final_xi++)
              for(int final_yi = yi*h; final_yi < (yi+1)*h; final_yi++)
                final_approximation.assign(final_xi,final_yi,true);
          }
        
    }// for all approximations

    // compute euler characteristic of dead cells
    // we apply white boundary conditions, that is surrounding is alive
    // we connect dead cells
    int chi_dead_times_eight = euler_wbc_pix(final_approximation);
    if(chi_dead_times_eight%8 != 0)
      std::cerr << "Error: non-integer Euler characteristic" << std::endl;

    // The Euler characteristic of the surviving Mandelbrot cells
    int actual_chi = - chi_dead_times_eight/8;

    /*
    std::stringstream outputfigstst;
    outputfigstst << "example-mandelbrot-subdivision" << subdivision << "-p" << p << "-S" << n_approximations << ".dat";
    final_approximation.fout(outputfigstst.str().c_str(),prefix_of);
    */

    mean_actual_chi += actual_chi;
    std_error_actual_chi += pow(actual_chi,2);
  }

  std_error_actual_chi = sqrt((std_error_actual_chi - pow(mean_actual_chi,2)/N_runs)/N_runs/(N_runs-1));
  mean_actual_chi /= N_runs;

  std::stringstream outputstst;
  outputstst << prefix_of << "frac-perc-mink-val-NN-" << subdivision << "x" << subdivision << "-n-" << n_approximations << "-p-" << std::setprecision(2) << p << ".dat";
  std::ofstream output(outputstst.str().c_str());
  output << p << " " << mean_actual_chi*pow(1./pow(subdivision,2)/p,n_approximations) << " " << std_error_actual_chi*pow(1./pow(subdivision,2)/p,n_approximations) << " " << n_approximations << std::endl;
  output.close();

  return 0;
}

