#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
//#include <float.h>
//#include <limits.h>
//#include <time.h>

/* Constants */
#define ON    1
#define OFF   0
#define YES   1
#define NO   -3
#define TRUE  1
#define FALSE 0

#define MAXLINE 250

#define NDIM     3 // Problem dimension  // Used mainly for x-y vs x-y-z
#define NDONTRI  3 // Nodes per triangle
#define NDONSEG  2 // Nodes per segment
#define EGPRTRI  3 // Edges per triangle
