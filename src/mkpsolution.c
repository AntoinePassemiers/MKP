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

#include "mkpsolution.h"

solution *create_empty_solution(const problem *p) {
  int n = p->n;
  int m = p->m;

  solution *s = (solution *)malloc(sizeof(solution));

  // see mkpdata.h for what the fields contain
  s->n = n;
  s->m = m;
  s->sol = (int *)calloc(n, sizeof(int));
  s->value = 0;

  s->resources_used = (int *)calloc(m, sizeof(int));
  return(s);
}

int is_feasible(const solution *s, const problem *p) {
  for (int i = 0 ; i < s->m ; i++) {
    if (s->resources_used[i] > p->capacities[i]) {
      return(0);
    }
  }
  return(1);
}

void copy_solution(const solution *src, solution *dest, const problem *p) {
  for (int i = 0 ; i < src->n ; i++) {
    dest->sol[i] = src->sol[i];
  }
  dest->value = src->value;
  for (int i = 0 ; i < src->m ; i++) {
    dest->resources_used[i] = src->resources_used[i];
  }
}

void update_solution(solution *s, const problem const *p) {
 s->value = 0;
 memset(s->resources_used, 0x00, p->m * sizeof(int));
 for (int item = 0; item < s->n; item++) {
   if (s->sol[item]) {
     s->value += p->profits[item];
     for (int i = 0 ; i < s->m ; i++) {
       s->resources_used[i] += p->constraints[i][item];
     }
   }
 }
}

int get_num_selected(const solution *s) {
  int num_selected = 0;
  for (int i = 0 ; i < s->n ; i++) {
    if (s->sol[i]) num_selected++;
  }
  return(num_selected);
}

solution *clone_solution(const solution *s, const problem *p) {
  solution *ns = create_empty_solution(p);
  copy_solution(s, ns, p);
  return(ns);
}

int are_identical(const solution const *s1, const solution const *s2) {
  return(memcmp(s1->sol, s2->sol, s1->n) == 0);
}

void destroy_solution(solution *s) {
  if (s->sol != NULL) free(s->sol);
  if (s->resources_used != NULL) free(s->resources_used);
  free(s);
}

void print_solution(const solution *s) {
  if (get_num_selected(s) == 0) printf("No items selected in the solution!\n");
  else {
    printf("Items in solution:\n[ ");
    for (int i = 0 ; i < s->n ; i++) {
      if (s->sol[i] == 1) printf("%d ",i);
    }
    printf("]\n");
  }

  printf("[ ");
  for (int i = 0 ; i < s->n ; i++) {
    if (s->sol[i]) {
      printf("%d ", i);
    }
  }
  printf("] :: [ ");
  for (int i = 0 ; i < s->n ; i++) {
    if (!s->sol[i]) {
      printf("%d ", i);
    }
  }
  printf("]\n\n");
}
