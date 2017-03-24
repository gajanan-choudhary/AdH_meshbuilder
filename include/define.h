#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <math.h>
//#include <float.h>
//#include <limits.h>
//#include <time.h>

/* Constants */
#define SMALL  1.0e-6
#define MAXLINE 250
#define ON     1
#define OFF    0
#define YES    1
#define NO    -3
#define TRUE   1
#define FALSE  0

/* Problem */
#define NDIM     3 /* Problem dimension, Just ignore this! */
#define NCORNERS 4 /* This program is only for quadrilateral mesh having 4 corners.*/ 
#define NDONTRI  3 /* Nodes per triangle */
#define NDONSEG  2 /* Nodes per segment */
#define EGPRTRI  3 /* Edges per triangle */

/* */
#define VEL 10     /* NB VEL bc */
#define DIS 20     /* NB DIS bc */
#define OTW 30     /* NB OTW bc */
#define INT 40     /* HY INT bc */
