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

#include "k_move.h"

k_move *create_empty_k_move(int k, const solution *s) {
  // Create k-move
  k_move *move = (k_move *)malloc(sizeof(k_move));
  move->k = k;
  move->n_selected = get_num_selected(s);
  move->n = s->n;

  // Create an array of zeros of length n.
  // This array is of the form: 1...10...0
  // where the number of 1's sums to one.
  // deselected[i] tells whether item i must be removed
  // from the current solution.
  // This is the lexicographically first combination 
  // of elements to remove from the current solution.
  move->deselected = (int *)calloc(move->n_selected, sizeof(int));
  move->removed = (int *)calloc(move->n, sizeof(int));
  move->indexes = (int *)malloc(move->n_selected * sizeof(int));

  int current = 0;
  for (int i = 0 ; i < s->n ; i++) {
    if (s->sol[i]) {
      move->indexes[current++] = i;
    }
  }
  for (int i = 0 ; i < k ; i++) {
    move->deselected[i] = 1;
    move->removed[move->indexes[i]] = 1;
  }
  return(move);
}

int iterate_k_move(k_move *move) {
  int *x = move->deselected; // Give shorter var name
  int n_deselected = 0;
  memset(move->removed, 0x00, move->n * sizeof(int));
  // Get lexicographically next combination of 1's that
  // still sums to one by moving the 1's to the right without 
  // collision. For each 1, move it to the right only if the element
  // to its right is not a 1 itself. Once one of the 1's has been
  // moved, stop. If no move was possible, then we have a combination
  // of the form 0...01...1 and the perturbative algorithm can stop iterating.
  int i;
  for (i = 0 ; i < move->n_selected ; i++) {
    if ((i == move->n_selected - 1) && x[i]) {
      return 0; // Stop iterating
    } else if (x[i] && (!x[i+1])) {
      // Move the current 1
      x[i] = 0;
      x[i+1] = 1;
      move->removed[move->indexes[i+1]] = 1;
      break;
    } else if (x[i]) {
      // Count the number of 1's to the left of the current position
      n_deselected++;
    }
  }
  // Efficiently move all left 1's to the leftmost positions
  for (int j = 0 ; j < i ; j++) {
    if (j < n_deselected) {
      x[j] = 1;
      move->removed[move->indexes[j]] = 1;
    } else {
      x[j] = 0;
    }
  }
  return(1);
}

void destroy_k_move(k_move *move) {
  // Deallocate k-move
  if (move->deselected != NULL) free(move->deselected);
  if (move->removed != NULL) free(move->removed);
  if (move->indexes != NULL) free(move->indexes);
  free(move);
}