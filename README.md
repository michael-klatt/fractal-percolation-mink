# FractalPercolationMink
**Approximate fractal percolation and compute its Euler characteristic**  
C++ code to simulate finite approximations of fractal percolation and to
analyze their geometry by estimating the expected Euler characteristic, see [arXiv:1812.06305](http://arxiv.org/abs/1812.06305)

                       M.A. Klatt and S. Winter (2018) "Geometric functionals of fractal percolation"

More generally the code can simulate pixelated random sets and compute the
Minkowski functionals or intrinsic volumes (area, perimeter, and Euler
characteristic) as well as their generalization to Minkowski tensors to
quantify anisotropy.

Author: Michael Andreas Klatt
Contributor: Steffen Winter
Maintainer: Michael Andreas Klatt (<software@mklatt.org>)  
License: GNU GPLv3  
Version: 19.01

Example
=======

First compile the code using the command

                       make

Note that this package uses boost and gsl libraries.

The mean Euler characteristic of fractal percolation can simply be
computed by the command

                       ./FractalPercolationMink_NN -n 8 -M 2 -p 0.5 -R 100

for an approximation level n=8 with subdivisions M=2 for a probability
of survival p=0.5, where the mean value and standard error of the mean
(based on 100 runs) is written to a file in the subfolder "./output/".

Columns in outputfile:
 * 1st column: probability of survival p
 * 2nd column: mean Euler characteristic
 * 3rd column: standard error of the mean
 * 4th column: level n of approximation

Parameters
==========

The parameters can either by chosen via the command line or via a
configuration file (default file name 'FractalPercolationMink.conf')

 * config_file ---       Configuration file to read parameters from
 * prefix_of ---         Folder for output, i.e. prefix for output files
 * p ---                 Fractal percolation: survival probability
 * subdivision ---       Fractal percolation: Parameter M of subdivisions
 * n_approximations ---  Fractal percolation: Level of approximation
 * N_runs ---            Fractal percolation: Number of simulation runs
 * imageout ---          Flag whether a pgm image shall be created
 * seed ---              Seed of the random number generator

Executables
===========

There are four executables from which the user can decide to compute the
Euler characteristic of
* of either all clusters or only the percolating cluster that spans the
  system both horizontally and vertically,
* connecting only nearest neighbors (maximum four-fold connected) or
  also next-to-nearest neighbors (maximum eight-fold connected graph).

FractalPercolationMink_NN
-------------------------

All clusters connecting only nearest neighbors

FractalPercolationMink_NN_percolating_cluster
---------------------------------------------

Percolating cluster connecting only nearest neighbors

FractalPercolationMink_NNN
--------------------------

All clusters connecting nearest neighbors and next-to-nearest
neighbors

FractalPercolationMink_NNN_percolating_cluster
----------------------------------------------

Percolating cluster connecting nearest neighbors and next-to-nearest
next-to-nearest neighbors 

