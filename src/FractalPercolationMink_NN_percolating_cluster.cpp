/* FractalPercolationMink_NN_percolating_cluster
 *
 * Percolating cluster connecting only nearest neighbors
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

// input is a black-and-white binfield
// keep only that black cluster, which spans the system both horizontally and vertically
bool only_keep_percolating_cluster(BinField<bool> &bw, const bool &print_sample);

// Iterate over all pixels "before" xi,yi and if the labels is "replace" replace it by "by"
void replace_labels(const int &replace, const int &by, BinField<int> &lbls, const unsigned &xi, const unsigned &yi, const unsigned &Ny);

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
  double fraction_of_percolating_samples = 0;

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

    /* only keep percolating cluster */
    bool living_cells_percolate = only_keep_percolating_cluster(final_approximation, imageout);
    if(living_cells_percolate){
      std::cout << "Run " << run << " found  _a_ percolating cluster ...\n";
      fraction_of_percolating_samples++;
    }
    else
      std::cout << "Run " << run << " found _no_ percolating cluster ...\n";

    // compute euler characteristic of dead cells
    // we apply white boundary conditions, that is surrounding is alive
    // we connect dead cells
    int chi_dead_times_eight = 0;
    if(living_cells_percolate)
      chi_dead_times_eight = euler_wbc_pix(final_approximation);
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
  fraction_of_percolating_samples /= N_runs;

  std_error_actual_chi = sqrt((std_error_actual_chi - pow(mean_actual_chi,2)/N_runs)/N_runs/(N_runs-1));
  mean_actual_chi /= N_runs;
  
  std::stringstream outputstst;
  outputstst << prefix_of << "frac-perc-mink-val-NN-percolatingcluster-" << subdivision << "x" << subdivision << "-n-" << n_approximations << "-p-" << std::setprecision(2) << p << ".dat";
  std::ofstream output(outputstst.str().c_str());
  output << p << " " << mean_actual_chi*pow(1./pow(subdivision,2)/p,n_approximations) << " " << std_error_actual_chi*pow(1./pow(subdivision,2)/p,n_approximations) << " " << n_approximations << " " << fraction_of_percolating_samples << " " << N_runs << std::endl;
  output.close();

  return 0;
}



// input is a black-and-white binfield
// keep only that surviving (white) cluster, which spans the system both horizontally and vertically
bool only_keep_percolating_cluster(BinField<bool> &bw, const bool &print_sample){
  // black = true = death
  // white = false = no death = survival

  // extract
  unsigned Nx = bw.call_Nx();
  unsigned Ny = bw.call_Ny();

  // define a BinField of labels (label 0 = "dead/black/true")
  // following pseudo code by Tobin Fricke's implementation in Wikipedia
  BinField<int> labels(Nx,Ny,0);
  int largest_label = 0;
  for(unsigned xi = 0; xi < Nx; xi++)
    for(unsigned yi = 0; yi < Ny; yi++)
      if(!bw.call(xi,yi)){
	// BOUNDARY CONDITION: DEAD
  	bool left = true;
  	bool below = true;
	if(xi > 0)
	  left = bw.call(xi-1,yi);
	if(yi > 0)
	  below = bw.call(xi,yi-1);
	if (left && below){ // Neither alive to left or below => new label
	  largest_label++;
	  labels.assign(xi,yi, largest_label);
	}
	else if (!left && below){ // alive to left but not below
	  labels.assign(xi,yi, labels.call(xi-1,yi));
	}
	else if (left && !below){ // alive below but not to the left
	  labels.assign(xi,yi, labels.call(xi,yi-1));
	}
	else{ // alive both below and to the left
	  int label_left  = labels.call(xi-1,yi);
	  int label_below = labels.call(xi,yi-1);
	  if (label_left == label_below)
	    labels.assign(xi,yi, label_left);
	  else if(label_left < label_below){ // replace label_below by label_left
	    replace_labels(label_below, label_left, labels, xi, yi, Ny);
	    labels.assign(xi,yi, label_left);
	  }
	  else if(label_below < label_left){ // replace label_left by label_below
	    replace_labels(label_left, label_below, labels, xi, yi, Ny);
	    labels.assign(xi,yi, label_below);
	  }
	}
      }
  // Now all pixels have a label according to their clusters

  // Find for each label the maximum and minimum xi
  std::vector<int> min_xi_per_label(largest_label,Nx);
  std::vector<int> max_xi_per_label(largest_label,0);
  std::vector<int> min_yi_per_label(largest_label,Ny);
  std::vector<int> max_yi_per_label(largest_label,0);

  for(int xi = 0; xi < int(Nx); xi++)
    for(int yi = 0; yi < int(Ny); yi++){
      int current_label = labels.call(xi,yi);
      if(current_label > 0){
	int index = current_label - 1;
  	if(min_xi_per_label[index] > xi)
  	  min_xi_per_label[index] = xi;
  	if(max_xi_per_label[index] < xi)
  	  max_xi_per_label[index] = xi;
  	if(min_yi_per_label[index] > yi)
  	  min_yi_per_label[index] = yi;
  	if(max_yi_per_label[index] < yi)
  	  max_yi_per_label[index] = yi;
      }
    }

  int percolating_label = 0;
  // Is there a percolating cluster?
  for(int li = 0; li < largest_label; li++)
    if (min_xi_per_label[li] == 0 && max_xi_per_label[li] == int(Nx-1) && min_yi_per_label[li] == 0 && max_yi_per_label[li] == int(Ny-1)){
      percolating_label = li + 1;
      break; // there can only be one percolating cluster
    }

  std::stringstream pgmoutstst;
  pgmoutstst << prefix_of << "frac-perc-mink-val-NN-percolatingcluster-" << subdivision << "x" << subdivision << "-n-" << n_approximations << "-p-" << std::setprecision(2) << p << ".pgm";
  std::ofstream pgmout;
  if(print_sample){
    pgmout.open(pgmoutstst.str().c_str());
    pgmout << "P2" << std::endl;
    pgmout << Ny << " " << Nx << std::endl;
    pgmout << "255" << std::endl; // max value (white)
  }

  // set all pixels to "dead=true" if they do not belong to the percolating cluster
  for(unsigned yi = Ny-1; yi < Ny; yi--)
    for(unsigned xi = 0; xi < Nx; xi++){
      if(print_sample){
	if( bw.call(xi,yi) == false && labels.call(xi,yi) == percolating_label )
	  pgmout << "0" << std::endl; //black (percolating)
  	else if( bw.call(xi,yi) == false )
  	  pgmout << "125" << std::endl; //gray (alive)
  	else
  	  pgmout << "255" << std::endl; //white (dead)
      }

      if ( labels.call(xi,yi) != percolating_label )
	bw.assign(xi,yi,true);
    }
  if(print_sample)
    pgmout.close();

  if(percolating_label)
    return true;
  else
    return false;
}


// Iterate over all pixels "before" xi,yi and if the labels is "replace" replace it by "by"
void replace_labels(const int &replace, const int &by, BinField<int> &lbls, const unsigned &xi, const unsigned &yi, const unsigned &Ny){
  for(unsigned x = 0; x < xi; x++)
    for(unsigned y = 0; y < Ny; y++){
      if(lbls.call(x,y) == replace)
	lbls.assign(x,y,by);
    }
  // unsigned x = xi;
  for(unsigned y = 0; y < yi; y++)
    if(lbls.call(xi,y) == replace)
      lbls.assign(xi,y,by);
}
