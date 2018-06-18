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

#include "perturbative.h"


void improve_with_1_moves(solution *initial_solution,
                          const problem *p,
                          enum constructive h,
                          enum perturbative nm,
                          float max_time) {
  // Temporary solutions
  solution *current_solution = create_empty_solution(p);
  solution *best_solution = create_empty_solution(p);

  // Keeping control flow consistent
  int best_value = initial_solution->value;
  int improving = 1;

  // Temporary arrays for shuffling sequences by index
  int *random_indexes = create_shuffled(p->n);
  int *indexes = create_shuffled(p->n); // Used only by the random selection heuristic

  // Allocate temporary arrays for doing the computing in the framework
  // of the Toyoda algorithm and the greedy heuristic.
  // This is done even when none of those heuristics are used since the memory
  // footprint is negligible.
  float *rv_profits = (float *)malloc(p->n * sizeof(float));
  float *u = (float *)malloc(p->m * sizeof(float));
  float *v = (float *)malloc(p->n * sizeof(float));
  float *pseudo_utilities = (float *)malloc(p->n * sizeof(float));

  timer_t timer = create_timer(max_time);
  while (improving && !finished(&timer)) {
    improving = 0; // We still have to find a better solution
    // The order in which items are evaluated for removal is random.
    // In the case of Best-Improvement, shuffling the order does
    // not make a difference.
    shuffle_int(random_indexes, p->n);
    for (int i = 0; (i < p->n) && !finished(&timer); i++) {
      int old_item = random_indexes[i];
      // Incumbent solution
      copy_solution(initial_solution, current_solution, p);
      if (current_solution->sol[old_item]) {
        // Remove the selected item (only the present one since k = 1)
        remove_item(current_solution, old_item, p);

        if (h == RANDOM) {
          // In the case of random selection, the order is shuffled
          shuffle_int(indexes, p->n);
        } else if (h == GREEDY) {
          // In the case of greedy heuristic, items are sorted by profit
          for (int j = 0 ; j < p->n ; j++) rv_profits[j] = (float)p->profits[j];
          indexes_of_sorted_array(indexes, rv_profits, p->n);
        } else {
          // In the case of Toyoda heuristic, items are sorted by pseudo-utilities
          compute_pseudo_utilities(u, v, pseudo_utilities, current_solution, p);
          indexes_of_sorted_array(indexes, pseudo_utilities, p->n);
        }
        // Add new items (except the one that has just been removed) in the appropriate
        // order (given by the perturbative heuristic) while keeping the solution feasible.
        for (int r = 0 ; r < p->n ; r++) {
          int new_item = indexes[r];
          if (new_item != old_item) {
            check_and_add_item(current_solution, new_item, p);
          }
        }
      }
      // In the case of First-Improvement heuristic, the improved solution is
      // directly copied into the initial solution. Indeed, at this point
      // best_solution is a pointer to the initial solution.
      if (current_solution->value > best_value) {
        best_value = current_solution->value;
        improving = 1;
        copy_solution(current_solution, best_solution, p);
        if (nm == FI) break;
      }
    }
    // In case of Best-Improvement heuristic, the best solution is copied
    // into the initial solution.
    copy_solution(best_solution, initial_solution, p);
  }

  // Deallocate all temporary data
  destroy_solution(current_solution);
  free(indexes);
  free(random_indexes);
  free(rv_profits);
  free(u);
  free(v);
  free(pseudo_utilities);
}


void variable_neighbourhood_descent(solution *initial_solution,
                                    const problem *p,
                                    enum constructive h,
                                    int k_max,
                                    float max_time) {

  // We allocate a temporary solution called "best_solution" to store
  // the temporary best solution. Indeed, the VND algorithm has been
  // implemented in a Best-Improvement fashion.
  solution *current_solution = create_empty_solution(p);
  solution *best_solution = create_empty_solution(p);

  // Keeping control flow consistent
  int best_value = initial_solution->value;
  int improving = 1;

  // Temporary array for shuffling sequences by index
  int *indexes = create_shuffled(p->n);

  // Allocate temporary arrays for doing the computing in the framework
  // of the Toyoda algorithm and the greedy heuristic.
  // This is done even when none of those heuristics are used since the memory
  // footprint is negligible.
  float *rv_profits = (float *)malloc(p->n * sizeof(float));
  float *u = (float *)malloc(p->m * sizeof(float));
  float *v = (float *)malloc(p->n * sizeof(float));
  float *pseudo_utilities = (float *)malloc(p->n * sizeof(float));

  timer_t timer = create_timer(max_time);
  while (improving && !finished(&timer)) {
    improving = 0; // We still have to find a better solution
    // The size of the neighbourhood is increasing over time.
    // Since we are considering the Best-Improvement, the order does
    // not make a difference.
    for (int k = 1; (k <= k_max) && !finished(&timer); k++) {
      // Initialize the lexicographically first combinations of k 1's
      // in an array of num_selected binary values.
      k_move *move = create_empty_k_move(k, initial_solution);

      do {
        copy_solution(initial_solution, current_solution, p);
        // Remove the k deselected items from current solution
        for (int i = 0 ; i < move->n_selected ; i++) {
          int old_item = move->indexes[i];
          if (move->removed[old_item]) remove_item(current_solution, old_item, p);
        }
        if (h == RANDOM) {
          // In the case of random selection, the order is shuffled
          shuffle_int(indexes, p->n);
        } else if (h == GREEDY) {
          // In the case of greedy heuristic, items are sorted by profit
          for (int j = 0 ; j < p->n ; j++) rv_profits[j] = (float)p->profits[j];
          indexes_of_sorted_array(indexes, rv_profits, p->n);
        } else {
          // In the case of Toyoda heuristic, items are sorted by pseudo-utilities
          compute_pseudo_utilities(u, v, pseudo_utilities, current_solution, p);
          indexes_of_sorted_array(indexes, pseudo_utilities, p->n);
        }
        // Add new items (except the ones that have just been removed) in the appropriate
        // order (given by the perturbative heuristic) while keeping the solution feasible.
        for (int r = 0 ; r < p->n ; r++) {
          int new_item = indexes[r];
          if (!move->removed[new_item]) check_and_add_item(current_solution, new_item, p);
        }
        // Copy solution if there is an improvement
        if (current_solution->value > best_value) {
          best_value = current_solution->value;
          improving = 1;
          copy_solution(current_solution, best_solution, p);
        }
      // Stop iterating once we get the lexicographically last combination of 1's
      // contained in "move".
      } while (iterate_k_move(move) && !finished(&timer));
      destroy_k_move(move);
    }
    copy_solution(best_solution, initial_solution, p);
  }

  // Deallocate all temporary data
  destroy_solution(best_solution);
  destroy_solution(current_solution);
  free(indexes);
  free(rv_profits);
  free(u);
  free(v);
  free(pseudo_utilities);
}