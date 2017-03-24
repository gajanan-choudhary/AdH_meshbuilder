#ifndef _MESH
#define _MESH

typedef struct{
    char *name;               // Name of the mesh for writing mesh files
    int nrows;                // Number of nodes in the y direction
    int ncols;                // Number of nodes in the x direction
    int nnodes;
    int nelems2d;
    int nelems1d;

    int ncorners;             // Number of corners in the domain. #defined to 4 right now.
    VECT3D *cornernodes;      // Corner nodes' co-ordinates, starting bottom left, going anticlockwise
    int nedges;               // Number of edges/boundaries of the domain. 4 for quadrilateral.
    ELEM1D *edge;             // Boundary edges

    VECT3D *xyz;              // Co-ordinates of all nodes, bathymetry in the third co-ordinate
    double *depth;            // Initial condition for the depth

    ELEM2D *elem2d;           // Triangular elements
    ELEM1D *elem1d;           // Edge elements for the boundary

} MESH;

void mesh_alloc_init(MESH **mesh_ptr, int nrows, int ncols, VECT3D *cornernodes);
void mesh_free(MESH **mesh_ptr);

#endif
