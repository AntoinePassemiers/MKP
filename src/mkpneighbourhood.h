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

#ifndef __MKPNEIGHBOURHOOD_H__
#define __MKPNEIGHBOURHOOD_H__

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mkpdata.h"
#include "mkpsolution.h"
#include "utils.h"
#include "mkpproblem.h"

// add item to the solution without feasibility checks
void add_item(solution *s, int item, const problem *p);

// check whether adding item would lead to some constraints being violated
int check_item(solution *s, int item, const problem *p);

/*
 * check whether adding item would lead to some constraints being violated
 *
 * if yes, don't add, and return the number of constraints that
 * would be violated by adding item
 *
 * if no, add and return 0
 */
int check_and_add_item(solution *s, int item, const problem *p);

// Remove item from solution (no need for feasability check)
void remove_item(solution *s, int item, const problem *p);

#endif // __MKPNEIGHBOURHOOD_H__
