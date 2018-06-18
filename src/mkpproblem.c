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

#include "mkpproblem.h"

problem *create_problem(int n,
                        int m,
                        int b,
                        int *profits,
                        int **constraints,
                        int *capacities) {
  problem *p = (problem *)malloc(sizeof(problem));
  p->n = n;
  p->m = m;
  p->best_known = b;
  p->profits = profits;
  p->constraints = constraints;
  p->capacities = capacities;
  return (p);
}

void destroy_problem(problem *p) {
  free(p->profits);
  for (int i = 0 ; i < p->m ; i++) free(p->constraints[i]);
  free(p->constraints);
  free(p->capacities);
  free(p);
}

void print_problem(problem *p) {
  int i, j;

  printf("Problem data:\n");
  printf("%d objects, %d knapsacks", p->n, p->m);
  if (p->best_known != 0) printf(", best known value: %d", p->best_known);
  printf("\n\n");

  printf("Object profits:\n");
  for (i = 0 ; i < p->n ; i++) {
    printf("%d ", p->profits[i]);
  }
  printf("\n\n");

  printf("Constraint matrix:\n");
  for (i = 0 ; i < p->m ; i++) {
    for (j = 0 ; j < p->n - 1 ; j++) {
      printf("%d * x%d + ", p->constraints[i][j], j);
    }
    printf("%d * x%d <= %d\n", p->constraints[i][p->n - 1],  p->n - 1, p->capacities[i]);
  }
  printf("\n");
}

problem *read_problem(char *filename) {
  FILE *input_file = open_file(filename);

  int *problem_data = read_problem_data(input_file);
  int n = problem_data[0],
      m = problem_data[1],
      b = problem_data[2];

  int *profits = read_profits(input_file, n);
  int **constraints = read_constraints(input_file, m, n);
  int *capacities = read_capacities(input_file, m);

  close_file(input_file);

  problem *p = create_problem(n, m, b, profits, constraints, capacities);

  free(problem_data);

  return(p);
}
