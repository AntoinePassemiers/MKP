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

#include "utils.h"


timer_t create_timer(float max_time) {
  timer_t timer;
  timer.start = clock();
  timer.max_time = max_time;
  timer.active = (max_time > 0.0);
  return(timer);
}

int finished(const timer_t *timer) {
  if (!timer->active) return 0;
  return((float)(clock() - timer->start) / CLOCKS_PER_SEC >= timer->max_time);
}

void set_seed(int seed) {
  srand(seed);
}

int *create_shuffled(int n) {
  int i, *v = (int *)malloc(n * sizeof(int));
  for (i = 0; i < n; i++) v[i] = i;
  shuffle_int(v, n);
  return(v);
}

void shuffle_int(int *v, int n) {
  int j, tmp;
  for (int i = n-1; i >= 1; i--) {
    j = rand() % i;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
  }
}

params *create_parse_error(params *pars) {
  printf("Error. Calls to MKP Solver must be of the form:\n\n \
    \tmkp <path_to_instance>  <random|greedy|toyoda|sa|ga> \
    [--fi|--bi|--vnd] [--max-time value] [--seed value] [--terse]\n");
  pars->has_parse_error = 1;
  return(pars);
}

params *read_params(int argc, char *argv[]) {
  params *pars = (params *)malloc(sizeof(params));
  memset(pars, 0x00, sizeof(params));
  pars->instance_file = argv[1];
  pars->max_time = -1;

  if (argc < 3) return(create_parse_error(pars));
  if (strcmp(argv[2], "random") == 0) pars->h = RANDOM;
  else if (strcmp(argv[2], "greedy") == 0) pars->h = GREEDY;
  else if (strcmp(argv[2], "toyoda") == 0) pars->h = TOYODA;
  else if (strcmp(argv[2], "sa") == 0) pars->h = SA;
  else if (strcmp(argv[2], "ga") == 0) pars->h = GA;
  else return(create_parse_error(pars));

  for (int i = 3 ; i < argc ; i++) {
    if (strcmp(argv[i], "--max-time") == 0) {
      pars->max_time = atof(argv[++i]);
    } else if (strcmp(argv[i], "--seed") == 0) {
      pars->seed = atoi(argv[++i]);
      pars->use_seed = 1;
    } else if (strcmp(argv[i], "--terse") == 0) {
      pars->terse = 1;
    } else if (strcmp(argv[i], "--fi") == 0) {
      if (pars->use_nm + pars->use_vns) return(create_parse_error(pars));
      pars->use_nm = 1;
      pars->nm = FI;
    } else if (strcmp(argv[i], "--bi") == 0) {
      if (pars->use_nm + pars->use_vns) return(create_parse_error(pars));
      pars->use_nm = 1;
      pars->nm = BI;
    } else if (strcmp(argv[i], "--vnd") == 0) {
      if (pars->use_nm + pars->use_vns) return(create_parse_error(pars));
      pars->use_vns = 1;
      pars->nm = VND;
    }
  }
  return(pars);
}

FILE *open_file(char *filename) {
  FILE *input_file;
  printf("\nOpening instance file %s\n\n",filename);
  if( (input_file = fopen(filename, "r")) == NULL) {
     fprintf(stderr, "error opening input file %s\n",filename);
     exit(1);
   }
  return(input_file);
}

void close_file(FILE *input_file) {
  fclose(input_file);
}

int *read_problem_data(FILE *input_file){
  int *problem_data = (int *) malloc(3 * sizeof(int));
  fscanf(input_file, "%d", &problem_data[0]);
  fscanf(input_file, "%d", &problem_data[1]);
  fscanf(input_file, "%d", &problem_data[2]);
  return(problem_data);
}

int *read_profits(FILE *input_file, int n) {
  int *profits = (int *) malloc(n * sizeof(int));
  for (int i = 0 ; i < n ; i++) {
    fscanf(input_file, "%d", &profits[i]);
  }
  return(profits);
}

int **read_constraints(FILE *input_file, int m, int n) {
  int **constraints = (int **) malloc(m * sizeof(int *));
  for (int i = 0 ; i < m ; i++) {
    constraints[i] = (int *) malloc(n * sizeof(int));
  }
  for (int i = 0 ; i < m ; i++) {
    for (int j = 0 ; j < n ; j++) {
      fscanf(input_file, "%d", &constraints[i][j]);
    }
  }
  return(constraints);
}

int *read_capacities(FILE *input_file, int m) {
  int *capacities = (int *) malloc(m * sizeof(int));
  for (int i = 0 ; i < m ; i++) {
    fscanf(input_file, "%d", &capacities[i]);
  }
  return(capacities);
}