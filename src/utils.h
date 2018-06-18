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

#ifndef __MKPUTIL_H__
#define __MKPUTIL_H__

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mkpdata.h"

typedef struct _timer_t {
  clock_t start;
  float max_time;
  int active;
} timer_t;

// initialize custom timer with current time
timer_t create_timer(float max_time);

// check whether timer reached max_time (if it is active)
int finished(const timer_t *timer);

// set the random seed
void set_seed(int seed);

// create a vector of n shuffled integers (values from 0 to n-1)
int *create_shuffled(int n);

// shuffle vector of n integers
void shuffle_int(int *vector, int n);

// print error message, show cmd syntax and set error flag in pars
params *create_parse_error(params *pars);

// read command line parameters: TO BE EXTENDED
params *read_params(int argc, char *argv[]);


FILE *open_file(char *filename);
void close_file(FILE *input_file);

int *read_problem_data(FILE *input_file);
int *read_profits(FILE *input_file, int n);
int **read_constraints(FILE *input_file, int m, int n);
int *read_capacities(FILE *input_file, int m);

#endif
