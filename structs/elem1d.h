#ifndef _ELEM1D_H
#define _ELEM1D_H

typedef struct{
    int str;                  // Edgestring ID
    int type;                 // Type = INFLOW/NOFLOW or DISCHARGE or TAILWATER or INTERFACE
    int nodes[NDONSEG];       // Node ID's
    VECT2D coord[NDONSEG];    // Co-ordinates of the nodes
    double length;            // 
    double bc_value;          // Value to be supplied
} ELEM1D;

#endif
