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

#ifndef __MKPSOLUTION_H__
#define __MKPSOLUTION_H__

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mkpdata.h"
#include "mkpneighbourhood.h"
#include "utils.h"
#include "mkpproblem.h"

/*
 * create an empty solution for a MKP problem
 *
 * no items selected
 */
solution *create_empty_solution(const problem *p);

/*
 * tell whether a solution satisfies all capacity constraints of p
 */
int is_feasible(const solution *s, const problem *p);

/*
 * copy a solution
 */
void copy_solution(const solution *src, solution *dest, const problem *p);

/*
 * Update a solution, based on its "sol" field
 */
 void update_solution(solution *s, const problem const *p);

/*
 * duplicate a solution
 */
solution *clone_solution(const solution *s, const problem *p);

/*
 * determine whether two solutions are identical
 */
int are_identical(const solution const *s1, const solution const *s2);

/*
 * eliminate a solution
 */
void destroy_solution(solution *s);

/*
 * get number of items selected by solution
 */
int get_num_selected(const solution *s);

/*
 * print a solution
 */
void print_solution(const solution *s);

#endif // __MKPSOLUTION_H__
