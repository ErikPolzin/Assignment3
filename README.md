# PGM Flood Fill Analysis

Runs a flood fill algorithm on a PGM file to determine the connected foreground regions above a certain threshold.

## Build & launch instructions

Run `make` inside the extracted folder. An executable should be built, to be run as `./findcomp`.
Run `make clean` to remove the generated object files.
Raun `./test` to run unit tests.


## Command-line interface

The CLI has the format `findcomp <FLAGS> <input_file>.pgm` where FLAGS are:

- s: minComponentSize, maxComponentSize
- t: threshold
- p: print out analysis results
- w: output file name


## Files

- `PGMMetadata.h/cpp`: Utilities for reading and writing PGM files
- `PGMImageProcessor.h/cpp`: Stores connected components.
- `ConnectedComponent.h/cpp`: Flood fill algorithm.
- `driver.cpp`: Implements command line parsing.
- `Makefile`: Build instructions for `make`
- `README.md`: Build instructions for you :)