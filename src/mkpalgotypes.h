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

#ifndef __MKPALGOTYPES_H__
#define __MKPALGOTYPES_H__

/*
 * Passed to heuristic algorithms to inform them about which constructive
 * heuristic to use / which heuristic has been used to find the
 * initial solution.
 *
 * RANDOM       : Random item selection
 * GREEDY       : Greedy heuristic (items sorted by profit)
 * TOYODA       : Toyoda algorithm (items sorted by pseudo-utilities)
 * SA           : Simulated Annealing
 * GA           : Genetic Algorithm
 */
enum constructive {
    RANDOM,
    GREEDY,
    TOYODA,
    SA,
    GA
};

/*
 * Passed to perturbative search algorithms to inform them 
 * about which heuristic to use.
 *
 * FI           : First-Improvement
 * BI           : Best-Improvement
 * VND          : Variable Neighbourhood Descent
 */
enum perturbative {
    FI,
    BI,
    VND
};

#endif // __MKPALGOTYPES_H__