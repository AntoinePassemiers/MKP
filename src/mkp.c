/***************************************************************************
 *   Copyright (C) 2018 Antoine Passemiers                                 *
 *   apassemi@ulb.ac.be                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#include "mkpdata.h"
#include "utils.h"
#include "mkpsolution.h"
#include "mkpproblem.h"
#include "mkpneighbourhood.h"

#include "constructive.h"
#include "perturbative.h"
#include "genetic.h"
#include "annealing.h"


int main(int argc, char *argv[]) {
  // Parse arguments. If they do not respect the expected form,
  // an error message is displayed and the program finishes.
  params *pars = read_params(argc, argv);
  if (pars->has_parse_error) return(1);
  
  // Set seed with pars->seed if it has been provided,
  // otherwise use current time.
  if (pars->use_seed) set_seed(pars->seed);
  else set_seed(time(NULL));
  float max_time = pars->max_time;
  
  problem *p = read_problem(argv[1]);

  // Print instance if argument --terse is provided
  if (!pars->terse) print_problem(p);

  clock_t start_time = clock();

  // Find an initial solution using a constructive heuristic
  solution *s;
  if (pars->h == RANDOM) s = random_insertion(p);
  else if (pars->h == GREEDY) s = greedy_insertion(p);
  else if (pars->h == TOYODA) s = toyoda_algorithm(p);
  else if (pars->h == SA) s = simulated_annealing(p, max_time);
  else s = genetic_algorithm(p, max_time);

  // If one the arguments [--fi, --bi, --vnd] is provided,
  // improve the initial solution using the appropriate perturbative algorithm
  if (pars->use_vns) {
    if (pars->nm == VND) variable_neighbourhood_descent(s, p, pars->h, 3, max_time);
  } else if (pars->use_nm){
    improve_with_1_moves(s, p, pars->h, pars->nm, max_time);
  }
  
  // Measure execution time
  clock_t end_time = clock();
  float exec_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;

  // Display solution if argument --terse is not provided
  // Otherwise display only execution time and value of the objective
  printf("Solution value: %d\n", s->value);
  if (is_feasible(s, p)) {
    printf("Solution is feasible.\n");
  } else {
    printf("Solution is infeasible.\n");
  }
  printf("Execution time in seconds: %.6f\n", exec_time);
  if (!pars->terse) print_solution(s);

  destroy_solution(s);
  destroy_problem(p);
  free(pars);

  return(0);
}

