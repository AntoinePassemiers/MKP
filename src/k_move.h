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

#ifndef __K_MOVE_H__
#define __K_MOVE_H__

#include <stdlib.h>

#include "mkpsolution.h"
#include "mkpalgotypes.h"

/*
 * k-move in the neighbourhood of a given solution.
 * Stores the identifiers of the items to remove from the current solution.
 * 
 * k              : number of items to remove when performing the move
 * n_selected     : number of items currently present in the solution
 * n              : total number of items considered in the problem instance
 * deselected     : array of length n_selected where deselected[i] is true if and
 *                  only if item i is currently selected and must be removed 
 *                  from current solution. Item i in "deselected" corresponds 
 *                  to item indexes[i] in removed.
 * removed        : array of length n where removed[i] is true if and
 *                  only if item i is currently selected and must be removed 
 *                  from current solution.
 * indexes        : array of length n_selected that maps indexes of "deselected"
 *                  to indexes of "removed"
 *                  
 */
typedef struct _k_move {
  int k;
  int n_selected;
  int n;
  int *deselected;
  int *removed;
  int *indexes;
} k_move;

/*
 * Create and initialize a k-move of the form:
 * 1...10...0 where there are k 1's
 *
 * k              : number of items to remove from current solution
 *                  when performing the move
 * n_selected     : Number of items currently added to the solution
 */
k_move *create_empty_k_move(int k, const solution *s);

/*
 * Go to lexicographically next permutation such
 * that the number of 1's still sums to one.
 * If the iteration is the last one, the function
 * returns 0, otherwise 1.
 *
 * move : k-move over which to iterate.
 *        The operation is done inplace.
 */
int iterate_k_move(k_move *move);

/*
 * Deallocate k-move.
 *
 * move : k-move to deallocate
 */
void destroy_k_move(k_move *move);

#endif // __K_MOVE_H__