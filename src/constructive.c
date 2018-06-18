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

#include "constructive.h"


int indexed_cmp_decreasing(const void *a, const void *b) {
  // a and b are pointers to indexed_val structs. They contain
  // the indexes of the values to be sorted in the original array.
  indexed_val *val1 = (indexed_val *)a;
  indexed_val *val2 = (indexed_val *)b;
  if (val1->value > val2->value) return(-1);
  else if (val1->value < val2->value) return(1);
  else return(0);
}

void indexes_of_sorted_array(int *indexes, const float *arr, int arr_size) {
  indexed_val elements[arr_size];
  for(int i = 0 ; i < arr_size ; i++) {
    elements[i].index = i; // Index of the element in the original array
    elements[i].value = arr[i]; // Value of the element
  }
  qsort(elements, arr_size, sizeof(indexed_val), indexed_cmp_decreasing);
  // Write the sorted indexes in a separated array
  for(int i = 0 ; i < arr_size ; i++) {
    indexes[i] = elements[i].index;
  }
}

float euclidean_norm(const float *vec, int vec_size) {
  // norm = sqrt(sum_k vec[k]**2)
  float norm = 0.0;
  for (int i = 0 ; i < vec_size ; i++) {
    norm += vec[i] * vec[i];
  }
  return(sqrt(norm));
}

solution *random_insertion(const problem *p) {
  // Implementation identical to create_random_solution(const problem *p)
  // from mkpsolution.c
  solution *s = create_empty_solution(p);
  int *indexes = create_shuffled(p->n);
  for (int i = 0 ; i < p->n ; i++) {
    check_and_add_item(s, indexes[i], p);
  }
  free(indexes);
  return(s);
}

solution *greedy_insertion(const problem *p) {
  solution *s = create_empty_solution(p);
  
  // Since indexes_of_sorted_array requires arrays of floats,
  // we need to cast the integer item profits
  float *rv_profits = (float *)malloc(p->n * sizeof(float));
  for (int j = 0 ; j < p->n ; j++) rv_profits[j] = (float)p->profits[j];

  // Sort the items by profit
  int *indexes = (int *)malloc(p->n * sizeof(int));
  indexes_of_sorted_array(indexes, rv_profits, p->n);
  // Add items in the given order
  for (int i = 0 ; i < p->n ; i++) {
    check_and_add_item(s, indexes[i], p);
  }
  free(indexes);
  free(rv_profits);
  return(s);
}

void compute_pseudo_utilities(float *u,
                              float *v,
                              float *pseudo_utilities,
                              const solution *s,
                              const problem *p) {
  // Giving shorter var names
  int **w = p->constraints;
  int *l = p->capacities;
  int *x = s->sol;

  // Compute vector U
  for (int i = 0 ; i < p->m ; i++) {
    u[i] = 0.0;
    for (int j = 0 ; j < p->n ; j++) {
      u[i] += ((float)w[i][j] / l[i]) * x[j];
    }
  }
  // Normalize U
  float norm_u = euclidean_norm(u, p->m);
  for (int i = 0 ; i < p->m ; i++) {
    u[i] = (norm_u == 0) ? 1 : (u[i] / norm_u);
  }

  // Compute vector V
  for (int j = 0 ; j < p->n ; j++) {
    v[j] = 0;
    for (int i = 0 ; i < p->m ; i++) {
      v[j] += ((float)w[i][j] / l[i]) * u[i];
    }
  }

  // Sort items according to the pseudo-utilities
  for (int j = 0 ; j < p->n ; j++) {
    pseudo_utilities[j] = (float)p->profits[j] / v[j];
  }
}

solution *toyoda_algorithm(const problem *p) {
  solution *s = create_empty_solution(p);
  int improving = 1;

  // Allocate temporary buffers
  float u[p->m];
  float v[p->n];
  float pseudo_utilities[p->n];
  int *indexes = (int *)malloc(p->n * sizeof(int));

  while (improving) {
    // Re-compute pseudo-utilities at each iteration
    compute_pseudo_utilities(u, v, pseudo_utilities, s, p);

    // Sort items by pseudo-utilities
    indexes_of_sorted_array(indexes, pseudo_utilities, p->n);
    int viol = 1;
    // Add items (if possible) in the given order
    for (int j = 0 ; j < p->n ; j++) {
      if (!s->sol[indexes[j]]) {
        viol = check_and_add_item(s, indexes[j], p);
        if (!viol) break;
      }
    }
    if (viol) improving = 0;
  }
  free(indexes);
  return(s);
}
