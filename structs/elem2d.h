#ifndef _ELEM2D
#define _ELEM2D

typedef struct{
    int type;                 // Type = BOUNDARY or INTERIOR
    int nodes[NDONTRI];       // Node ID's
    VECT2D coord[NDONTRI];    // Co-ordinates of the nodes
    double area;              // 
} ELEM2D;

#endif
