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

#ifndef __MKPDATA_H__
#define __MKPDATA_H__

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mkpalgotypes.h"

/*
 * generic solution for a MKP problem
 * 
 * n              : number of objects
 * m              : number of knapsacks
 * value          : objective function value
 * sol            : 0/1 vector (item j discarded/selected)
 * resources_used : vector of length m containing the amount of resources used
 *                  by the current solution
 */
typedef struct _solution {
  int n;
  int m;
  int value;
  int *sol;
  int *resources_used;
} solution;


/*
 * storage for command-line parameters and supplementary information
 * 
 * instance_file   : path to the instance file
 * h               : Type of constructive heuristic to use
 * nm              : Type of perturbative algorithm to use (if use_nm is set to true)
 * use_nm          : Whether to use best-improvement or best-improvement algorithm
 * use_vns         : Whether to use Variable Neighbourhood Descent (VND) or
 *                   Relaxation-Guided Variable Neighbourhood Search (RGVNS).
 *                   use_nm and use_vnd cannot be both true but can be both false
 * use_seed        : Whether to use a seed (set to true if argument --seed is provided)
 * seed            : Value of the seed for the RNG
 * terse           : Whether to print brief messages to the standard output
 *                   If set to true, only execution time and value of the objective will be printed
 * max_time        : Maximum execution time for local search algorithms
 * has_parse_error : Whether the command-line parser failed
 */
typedef struct _params {
  char *instance_file;
  enum constructive h;
  enum perturbative nm;
  int use_nm;
  int use_vns;
  int use_seed;
  int seed;
  int terse;
  float max_time;
  int has_parse_error;
} params;


// could be used for both profit and cost
typedef struct _item_val {
  int id;
  double value;
} item_val;

#endif // __MKPDATA_H__
