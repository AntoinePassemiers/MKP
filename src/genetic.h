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

#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "mkpalgotypes.h"
#include "constructive.h"
#include "perturbative.h"
#include "mkpsolution.h"
#include "mkpneighbourhood.h"

#include <stdint.h>


/*
 * Return the identifier of the member with lowest fitness.
 *
 * population      : Array of pointers to the members
 *                   of the population
 * population_size : Population size / array size
 */
int find_worst_member(solution **population,
                      int population_size);

/*
 * Return the identifier of the member with highest fitness
 * in the given pool. The pool is composed of the members
 * whose identifiers are located by the following indices:
 * indices[partition_start], indices[partition_start+1], ...
 * indices[partition_end].
 *
 * population      : Array of pointers to the members
 *                   of the population
 * indices         : indirect indices of the members in the pool
 * partition_start : indices[partition_start] gives the index of
 *                   the first member in the pool
 * partition_end   : indices[partition_end] gives the index of
 *                   the last member in the pool
 */
int tournament(solution **population,
               const int const *indices,
               int partition_start,
               int partition_end);

/*
 * Create a new member/solution and add items to it
 * according to a uniform crossover operation.
 *
 * parent1         : First parent solution
 * parent2         : Second parent solution
 * p               : instance of MKP
 */
solution *crossover(const solution const *parent1,
                    const solution const *parent2,
                    const problem const *p);

/*
 * Apply multiple mutations to a solution.
 *
 * individual      : solution to mutate
 * p               : instance of MKP
 * indices         : indirect indices of the items.
 *                   This array is shuffled in order to
 *                   randomly select items in solution.
 * mutation_rate   : number of mutations to apply
 */
void mutate(solution *individual,
            const problem const *p,
            int *indices,
            int mutation_rate);

/*
 * Make a solution feasible by dropping the less profitable
 * items first (DROP phase). If the resulting feasible solution
 * leaves some sufficiently large gaps, the most profitable
 * items are added first (ADD phase). The order for
 * dding/removing is determined by item pseudo-utilities.
 *
 * individual      : infeasible solution
 * indices         : indirect indices of the items.
 *                   This array is used to sort the indices
 *                   according to item pseudo-utilities.
 * p               : instance of MKP
 */
void repair(solution *individual,
            const int const *indices,
            const problem const *p);

/*
 * Generate a feasible solution with a genetic algorithm.
 *
 * p               : instance of MKP
 * max_time        : maximum execution time. If max_time <= 0,
 *                   then time is not used as stopping criterion.
 */
solution *genetic_algorithm(const problem const *p,
                            float max_time);

#endif // __GENETIC_H__