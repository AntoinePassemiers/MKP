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

#ifndef __PERTURBATIVE_H__
#define __PERTURBATIVE_H__

#include "mkpsolution.h"
#include "mkpalgotypes.h"
#include "constructive.h"
#include "k_move.h"

/*
 * If nm is FI, apply first-improvement algorithm with respect to
 * the constructive heuristic h that was used to find initial_sol.
 * Otherwise, apply best-improvement algorithm in the same way.
 *
 * initial_sol    : Initial solution found with heuristic h
 * p              : Problem to solve
 * h              : Construction heuristic that was used to find
 *                  the initial solution
 * initial_sol    : Initial solution found with heuristic h
 * nm             : Perturbative algorithm to apply
 * max_time       : Limit for the execution time
 */
void improve_with_1_moves(solution *initial_sol, 
                          const problem *p, 
                          enum constructive h, 
                          enum perturbative nm,
                          float max_time);

/*
 * Apply Variable Neighbourhood Descent algorithm with respect to
 * the constructive heuristic h that was used to find initial_sol.
 * The algorithm is implemented with a Best-Improvement heuristic.
 * That is, all k-moves will be evaluated for k in {1, ..., k_max},
 * and the best k-move will be performed to find the next solution.
 *
 * initial_sol    : Initial solution found with heuristic h
 * p              : Problem to solve
 * h              : Construction heuristic that was used to find
 *                  the initial solution
 * k_max          : Maximum value k to perform a k-move.
 *                  If k_max == 3, then all k-moves will be
 *                  evaluated, where k is in {1, 2, 3}.
 * max_time       : Limit for the execution time
 */
void variable_neighbourhood_descent(solution *initial_sol, 
                                    const problem *p, 
                                    enum constructive h, 
                                    int k_max,
                                    float max_time);

#endif // __PERTURBATIVE_H__
