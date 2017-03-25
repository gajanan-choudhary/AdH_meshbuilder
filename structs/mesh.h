#ifndef _MESH_H
#define _MESH_H

typedef struct{
    int supID, subID;         // Supermodel and submodel ID's, starting from 0
    char name[MAXLINE];       // Name of the mesh for writing mesh files
    int nrows;                // Number of nodes in the a column
    int ncols;                // Number of nodes in the a row
    int nnodes;
    int nelems2d;
    int nelems1d;
    int ncorners;             // Number of corners in the domain. #defined to 4 right now.
    int nboundaries;          // Number of edges/boundaries of the domain. 4 for quadrilateral.


    double t0, tf, dt;        // Starting and ending times, and time step
    double awrite_interval;   // Used for creating SERIES AWRITE
    double *wse;              // Initial condition for the water surface elevation, size = nnodes

    VECT3D *cornernodes;      // Corner nodes' co-ordinates, starting bottom left, going anticlockwise
    VECT3D *xyz;              // Co-ordinates of all nodes, bathymetry in the third co-ordinate

    ELEM2D *elem2d;           // Triangular elements
    ELEM1D *boundary;         // Boundary edges
    ELEM1D *elem1d;           // Edge elements for the boundary

} MESH;

void mesh_init(MESH *mesh);
void mesh_free(MESH **mesh_ptr, int nmeshes);

#endif
