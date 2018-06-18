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

#include "genetic.h"


int find_worst_member(solution **population,
                      int population_size) {
  int worst_value = INT_MAX;
  int worst_index = 0;
  // Basically an argmin function applied on
  // the objective value of the members
  for (int i = 0; i < population_size; i++) {
    if (population[i]->value < worst_value) {
      worst_value = population[i]->value;
      worst_index = i; // Index of the current worst solution
    }
  }
  return worst_index;
}

int tournament(solution **population, const int const *indices,
               int partition_start,
               int partition_end) {
  int best_value = INT_MIN;
  int best_index = 0;
  // Argmax function applied on the objective value
  // of the members in the pool.
  // The identifiers of the members to evaluate are given by:
  // indices[partition_start], indices[partition_start+1], ...
  // indices[partition_end].
  for (int i = partition_start; i < partition_end; i++) {
    if (population[indices[i]]->value > best_value) {
      best_value = population[indices[i]]->value;
      // Index of the best member of the pool
      best_index = indices[i];
    }
  }
  return best_index;
}

solution *crossover(const solution const *parent1,
                    const solution const *parent2,
                    const problem const *p) {
  // Create new empty solution
  solution *child = create_empty_solution(p);
  for (int item = 0; item < parent1->n; item++) {
    // Take next binary character either from parent 1
    // or from parent 2 with 0.5/0.5 probabilities.
    if (((double)rand() / (double)RAND_MAX) < 0.5) {
      child->sol[item] = parent1->sol[item];
    } else {
      child->sol[item] = parent2->sol[item];
    }
  }
  // Because we modified the binary representation
  // of the child "by hand", the quantities of
  // resources used need to be updated, as well as the
  // value of the objective function. This update is done
  // here for speed purposes.
  update_solution(child, p);
  return child;
}

void mutate(solution *individual,
            const problem const *p,
            int *indices,
            int mutation_rate) {
  // Shuffle the item indices
  shuffle_int(indices, individual->n);
  // Select mutation_rate items without replacement
  for (int i = 0; i < mutation_rate; i++) {
    int item = indices[i];
    // Flip binary character
    individual->sol[item] = 1 - individual->sol[item];
  }
  // We modified the binary representation of the 
  // child "by hand" -> solution update
  // (objective and resources).
  update_solution(individual, p);
}

void repair(solution *individual,
            const int const *indices,
            const problem const *p) {

  // DROP phase:
  // While one of the capacity constraints is not satisfied,
  // items are removed by reversed order of pseudo-utility.
  for (int j = individual->n - 1; j > -1; j--) {
    int item = indices[j];
    if (individual->sol[item]) {
      for (int i = 0; i < individual->m; i++) {
        if (individual->resources_used[i] > p->capacities[i]) {
          remove_item(individual, item, p);
          // The item has been removed,
          // it cannot be removed twice.
          break;
        }
      }
    }
  }
  // ADD phase:
  // While solution remains feasible by adding items,
  // items are inserted by order of pseudo-utility.
  for (int j = 0; j < individual->n; j++) {
    int item = indices[j];
    check_and_add_item(individual, item, p);
  }
}

solution *genetic_algorithm(const problem const *p,
                            float max_time) {
                      
  int population_size = 100;
  int mutation_rate = 2;
  int pool_size = 2;
  int t_max = 1000000 * p->n;

  timer_t timer = create_timer(max_time);

  // Currently best solution
  solution *best_solution = create_empty_solution(p);

  // Necessary arrays for computing pseudo-utilities
  // using Toyoda algorithm
  float *u = (float *)malloc(p->m * sizeof(float));
  float *v = (float *)malloc(p->n * sizeof(float));
  float *pseudo_utilities = (float *)malloc(p->n * sizeof(float));

  // Necessary arrays for sorting items, randomly picking items
  // and randomly sampling the population
  int *member_indices = create_shuffled(population_size);
  int *item_indices = create_shuffled(p->n);

  // Initialize population by creating many solutions
  // with the random insertion constructive heuristic
  solution *population[population_size];
  for (int i = 0; i < population_size; i++) {
    population[i] = random_insertion(p);
  }

  // Store currently best solution
  int best = tournament(population, member_indices, 0, population_size);
  copy_solution(population[best], best_solution, p);

  int t = 0;
  while ((t < t_max) && !finished(&timer)) {
    // Create two pools of individuals
    // First pool: from member_indices[0] to member_indices[pool_size]
    // Second pool: from member_indices[pool_size]
    //              to member_indices[2*pool_size]
    shuffle_int(member_indices, population_size);
    int p1 = tournament(population, member_indices, 0, pool_size);
    int p2 = tournament(
      population, member_indices, pool_size, 2*pool_size);

    // Apply crossover operator between parents p1 and p2
    solution *child = crossover(population[p1], population[p2], p);

    // Mutate newly created child solution
    mutate(child, p, item_indices, mutation_rate);

    // Apply repair operator on child solution if infeasible
    if (!is_feasible(child, p)) {
      compute_pseudo_utilities(u, v, pseudo_utilities, child, p);
      indexes_of_sorted_array(item_indices, pseudo_utilities, p->n);
      repair(child, item_indices, p);
    }

    // Improve newly created solution with iterative best-fit
    // The neighbourhood is of size 1 to avoid slowing down GA
    improve_with_1_moves(child, p, GREEDY, BI, max_time);

    // Determine whether the new feasible solution is identical
    // to one of the members in the population
    int is_duplicate = 0;
    for (int i = 0; i < population_size; i++) {
      if (are_identical(child, population[i])) {
        is_duplicate = 1;
        break;
      }
    }
    if (is_duplicate) {
      // Destroy new solution if duplicate
      destroy_solution(child);
    } else {
      // If new solution is unique, destroy and replace the worst
      // member of the population (the one with lowest fitness)
      int worst = find_worst_member(population, population_size);
      destroy_solution(population[worst]);
      population[worst] = child;

      // Steady-state replacement
      if (child->value > best_solution->value) {
        copy_solution(child, best_solution, p);
      }
    }
    t++;
  }

  // Deallocate memory
  for (int i = 0; i < population_size; i++) {
    free(population[i]);
  }
  free(population);
  free(member_indices);
  free(item_indices);
  free(u);
  free(v);
  free(pseudo_utilities);
  return best_solution;
}

