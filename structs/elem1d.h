#ifndef _ELEM1D
#define _ELEM1D

typedef struct{
    int type;                 // Type = INFLOW/NOFLOW or TAILWATER or INTERFACE
    int nodes[NDONSEG];       // Node ID's
    VECT2D coord[NDONSEG];    // Co-ordinates of the nodes
    double length;            // 
} ELEM1D;

#endif
