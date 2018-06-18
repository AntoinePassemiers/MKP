# How to use MKP heuristic solver

Compile the program:

```sh
  $ make release
  $ make clean
```

Each call to the heuristic solver must be of the form:

```sh
  $ mkp <path_to_instance>  <random|greedy|toyoda|ga|sa> \
      [--max-time value] [--fi|--bi|--vnd] [--seed value] [--terse]
```

If no seed is provided, current time is used as seed to the RNG.
*--terse* is an optional argument for helping the R script to
accelerate the parsing of standard output by displaying only necessary
information. Mind it only if you wish to hide the objective, constraints,
selected items and discarded items in the output messages.
Let's explicit how to call each one of the twelve algorithms:

```sh
  $ # Genetic algorithm
  $ mkp <path_to_instance>  ga
  $ # Simulated annealing
  $ mkp <path_to_instance>  sa
  $ # Set time limit to 60 seconds
  $ mkp <path_to_instance>  <random|greedy|toyoda|ga|sa> --max-time 60
```