#ifndef _MESH
#define _MESH

typedef struct{
    char name;                // Name of the mesh for writing mesh files
    int nrows;                // Number of nodes in the y direction
    int ncols;                // Number of nodes in the x direction
    int nnodes;
    int nelems2d;
    int nelems1d;

    VECT3D cornernodes[4]; // Corner nodes' co-ordinates, starting bottom left, going anticlockwise

    VECT3D *xyz;          // Co-ordinates of all nodes, bathymetry in the third co-ordinate
    double *depth;            // Initial condition for the depth

    ELEM2D *elem2d;           // Triangular elements
    ELEM1D *elem1d;           // Edge elements for the boundary

} MESH;

#endif
