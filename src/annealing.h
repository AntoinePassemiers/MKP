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

#ifndef __ANNEALING_H__
#define __ANNEALING_H__

#include "mkpalgotypes.h"
#include "constructive.h"
#include "perturbative.h"
#include "mkpsolution.h"
#include "mkpneighbourhood.h"

#include <stdint.h>


/*
 * Metropolis acceptance criterion. Returns 1 if newly created
 * solution is accepted, 0 otherwise.
 *
 * new_f            : objective value of the newly created solution
 * current_f        : objective value of current solution
 * t                : current temperature of the system
 */
int metropolis_criterion(double new_f, double current_f, double t);

/*
 * Generate a feasible solution with simulated annealing.
 *
 * p                : instance of MKP
 * max_time         : Maximum execution time
 */
solution *simulated_annealing(const problem const *p, float max_time);

#endif // __ANNEALING_H__