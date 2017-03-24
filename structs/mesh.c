#include "global_header.h"

void mesh_alloc_init(MESH **mesh_ptr, int nrows, int ncols, VECT3D *cornernodes /* length 4 */){
    MESH *mesh = NULL; /* Alias */
    *mesh_ptr = (MESH *) malloc(sizeof(MESH) * 1);
    if (*mesh_ptr != NULL){
        mesh = *mesh_ptr;        
    }
    else{
        throw_error("Error in memory allocation for mesh. Could not allocate memory");
    }
    int i, j; /* Loop counters */

    mesh->name = (char *) malloc(sizeof(char) * MAXLINE);
    sprintf(mesh->name, "mesh");
    mesh->nrows = nrows;
    mesh->ncols = ncols;
    mesh->nnodes = nrows*ncols;
    mesh->nelems2d = 2* (nrows-1)*(ncols-1);
    mesh->nelems1d = 2*((nrows-1)+(ncols-1));

    mesh->ncorners = NCORNERS; /* Quadrilateral domain, by choice */
    mesh->cornernodes = (VECT3D *) malloc(sizeof(VECT3D) * mesh->ncorners);
    checkmem(mesh->cornernodes);

    mesh->nedges = NCORNERS; /* Quadrilateral domain, by choice */
    mesh->edge = (ELEM1D *) malloc(sizeof(ELEM1D) * mesh->nedges);
    checkmem(mesh->edge);

    mesh->xyz = (VECT3D *) malloc(sizeof(VECT3D) * mesh->nnodes);
    checkmem(mesh->xyz);

    mesh->depth = (double *) malloc(sizeof(double) * mesh->nnodes);
    checkmem(mesh->depth);

    mesh->elem2d = (ELEM2D *) malloc(sizeof(ELEM2D) * mesh->nelems2d);
    checkmem(mesh->elem2d);

    mesh->elem1d = (ELEM1D *) malloc(sizeof(ELEM1D) * mesh->nelems1d);
    checkmem(mesh->elem1d);
    
    /* Iniitialize arrays */
    for (i=0; i<mesh->ncorners; i++){ /* Length of cornernodes */
        mesh->cornernodes[i].x = cornernodes[i].x;
        mesh->cornernodes[i].y = cornernodes[i].y;
        mesh->cornernodes[i].z = cornernodes[i].z;
    }
    for (i=0; i<mesh->nnodes; i++){
        mesh->xyz[i].x = 0.0;
        mesh->xyz[i].y = 0.0;
        mesh->xyz[i].z = 0.0;
        mesh->depth[i] = 0.0;
    }
    for (i=0; i<mesh->nelems2d; i++){
        for (j=0; j<NDONTRI; j++){
            mesh->elem2d[i].nodes[j] = NO;
            mesh->elem2d[i].coord[j].x = 0.0;
            mesh->elem2d[i].coord[j].y = 0.0;
        }
    }
    for (i=0; i<mesh->nelems1d; i++){
        for (j=0; j<NDONSEG; j++){
            mesh->elem1d[i].nodes[j] = NO;
            mesh->elem1d[i].coord[j].x = 0.0;
            mesh->elem1d[i].coord[j].y = 0.0;
        }
    }
}

void mesh_free(MESH **mesh_ptr){
    free((*mesh_ptr)->name);
    free((*mesh_ptr)->cornernodes);
    free((*mesh_ptr)->edge);
    free((*mesh_ptr)->xyz);
    free((*mesh_ptr)->depth);
    free((*mesh_ptr)->elem2d);
    free((*mesh_ptr)->elem1d);
    free((*mesh_ptr));
}
