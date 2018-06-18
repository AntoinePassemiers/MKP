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

#ifndef __CONSTRUCTIVE_H__
#define __CONSTRUCTIVE_H__

#include <math.h>

#include "mkpsolution.h"
#include "mkpalgotypes.h"


/*
 * Stores a value as well as the index where this value was stored
 * in the original array. It is used to sort an array
 * without actually modify it, only by returning the sorted indexes.
 * 
 * index          : Location of the element in the original array
 * value          : Value of the element
 */
typedef struct _indexed_val {
  int index;
  float value;
} indexed_val;

/*
 * Compare the values of a and b.
 * Return -1 if a  > b
 * Return  0 if a == b
 * Return  1 if a  < b
 *
 * a : Pointer to the first indexed_val struct
 * b : Pointer to the second indexed_val struct
 */
int indexed_cmp_decreasing(const void *a, const void *b);

/*
 * Take an array as input and place the indexes of its sorted version in
 * another array. The original array is not modified.
 *
 * indexes        : Array where to write the indexes of the sorted array
 * arr            : Array to be sorted
 * arr_size       : Length of arr
 */
void indexes_of_sorted_array(int* indexes, const float *arr, int arr_size);

/*
 * Compute the euclidean norm of an array.
 *
 * vec            : Array representing a vector
 * vec_size       : Number of components in the vector
 */
float euclidean_norm(const float *vec, int vec_size);

/*
 * Create an initial solution to MKP by adding items to it
 * in a random order.
 *
 * p : Instance of MKP to solve
 */
solution *random_insertion(const problem *p);

/*
 * Sort items given in a problem, sort them by profit, and add them in this
 * order to an empty solution in order to create an initial solution.
 *
 * p : Instance of MKP to solve
 */
solution *greedy_insertion(const problem *p);

/*
 * Make one step of the Toyoda algorithm by computing vectors U (normalized), V,
 * and the vector of pseudo-utilities. The solution is not modified.
 *
 * u              : Normalized vector U
 * v              : Vector v
 * v              : Vector of pseudo-utilities
 * s              : Current solution
 * p              : MKP instance of the problem to solve
 */
void compute_pseudo_utilities(float *u,
                              float *v,
                              float *pseudo_utilities,
                              const solution *s,
                              const problem *p);

/*
 * Execute Toyoda algorithm in order to find an initial solution to the problem.
 *
 * p : MKP instance of the problem to solve
 */
solution *toyoda_algorithm(const problem *p);

#endif // __CONSTRUCTIVE_H__
