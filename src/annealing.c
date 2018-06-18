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

#include "annealing.h"


int metropolis_criterion(double new_f, double current_f, double t) {
  if (new_f - current_f >= 0) {
    return 1; // Accept new solution if higher value of the objective
  } else {
    double proba = exp((new_f - current_f) / t);
    // Randomly decide if new solution is accepted
    return (((double)rand() / (double)RAND_MAX) < proba);
  }
}

solution *simulated_annealing(const problem const *p, float max_time) {
  timer_t timer = create_timer(max_time);

  // Generate initial solution with Toyoda algorithm
  solution *best_solution = toyoda_algorithm(p);

  // Allocate memory for storing solution at step t and
  // solution at step t+1
  solution *current_solution = clone_solution(best_solution, p);
  solution *new_solution = clone_solution(best_solution, p);

  // Use best parameters.
  // If a maximum execution time is provided,
  // the Markov chain length is set dynamically
  int markov_chain_length = (max_time <= 0) ? p->n : INT_MAX;
  float t0 = 500;
  double e = 1e-05;
  float alpha = 0.95;

  // Number of iterations of th eoutermost loop required in order
  // to get a frozen system.
  int k_star = (int)floor(log(e / t0) / log(alpha));
  
  for (float t = t0; (t >= e) && !finished(&timer); t *= alpha) {
    // (max_time / k_star) is the maximum running time
    // of current iteration
    timer_t iter_timer = create_timer(max_time / (double)k_star);
    int m = 0;
    while ((m < markov_chain_length) && (!finished(&iter_timer)) && \
        (!finished(&timer))) {
      copy_solution(current_solution, new_solution, p);
      int i = rand() % p->n; // Randomly select bit to flip
      if (new_solution->sol[i] == 0) {
        // Add item if not present in solution
        new_solution->sol[i] = 1;
      } else {
        // Remove item if present in solution
        new_solution->sol[i] = 0;
        new_solution->sol[rand() % p->n] = 1;
      }
      // Repair solution if not feasible
      update_solution(new_solution, p);
      while (!is_feasible(new_solution, p)) {
        int j = rand() % p->n; // Randomly select item to remove
        if (new_solution->sol[j] == 1) {
          new_solution->sol[j] = 0;
          update_solution(new_solution, p);
        }
      }

      // Improve newly created solution with iterative best-fit
      // The neighbourhood is of size 1 to avoid slowing down GA
      improve_with_1_moves(new_solution, p, TOYODA, BI, max_time);

      // Randomly decide whether to accept new solution as current solution
      if (metropolis_criterion(new_solution->value, current_solution->value, t)) {
        copy_solution(new_solution, current_solution, p);
        m++;
      }

      // Store current solution if higher value of the objective
      if (current_solution->value > best_solution->value) {
        copy_solution(current_solution, best_solution, p);
      }
    }
  }
  return best_solution;
}