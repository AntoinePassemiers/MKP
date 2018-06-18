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

#include "mkpneighbourhood.h"


void add_item(solution *s, int item, const problem *p) {
  // if item was already in the solution, do nothing
  if (s->sol[item] == 0) {
    s->sol[item] = 1;
    s->value += p->profits[item]; // delta evaluation

    // update list of resources used by the current solution
    for (int i = 0 ; i < s->m ; i++) {
      s->resources_used[i] += p->constraints[i][item];
    }
  }
}

int check_item(solution *s, int item, const problem *p) {
  // check whether adding the item would lead to violate some contraints
  // and count how many
  if (s->sol[item] == 1) return(1);
  for (int i = 0 ; i < s->m ; i++) {
    if (s->resources_used[i] + p->constraints[i][item] > p->capacities[i]) {
      return(1);
    }
  }
  return(0);
}

int check_and_add_item(solution *s, int item, const problem *p) {
  if (check_item(s, item, p)) {
    return(1);
  } else {
    // if no constraint is violated, add the item to the solution
    add_item(s, item, p);
    return(0);
  }
}

void remove_item(solution *s, int item, const problem *p) {
  // if item is not in the solution, do nothing
  if (s->sol[item] == 1) {
    s->sol[item] = 0;
    s->value -= p->profits[item]; // delta evaluation

    // update list of resources used by the current solution
    for (int i = 0 ; i < s->m ; i++) {
      s->resources_used[i] -= p->constraints[i][item];
    }
  }
}