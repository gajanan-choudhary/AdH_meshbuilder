#include "global_header.h"

// void mesh_alloc_init(MESH **mesh_ptr, int nrows, int ncols, VECT3D *cornernodes /* length 4 */){
//     MESH *mesh = NULL; /* Alias */
//     *mesh_ptr = (MESH *) malloc(sizeof(MESH) * 1);
//     if (*mesh_ptr != NULL){
//         mesh = *mesh_ptr;        
//     }
//     else{
//         throw_error("Error in memory allocation for mesh. Could not allocate memory");
//     }
void mesh_init(MESH *mesh){
    int i, j; /* Loop counters */

    assert(mesh->nrows>1);
    assert(mesh->ncols>1);
    assert(mesh->ncorners==NCORNERS);

    //mesh->name = (char *) malloc(sizeof(char) * MAXLINE);
    //sprintf(mesh->name, "mesh");
    mesh->nnodes = mesh->nrows*mesh->ncols;
    mesh->nelems2d = 2* (mesh->nrows-1)*(mesh->ncols-1);
    mesh->nelems1d = 2*((mesh->nrows-1)+(mesh->ncols-1));
    mesh->nboundaries = mesh->ncorners; /* Quadrilateral domain, by choice */

    mesh->cornernodes = (VECT3D *) malloc(sizeof(VECT3D) * mesh->ncorners);
    checkmem(mesh->cornernodes);

    mesh->boundary = (ELEM1D *) malloc(sizeof(ELEM1D) * mesh->nboundaries);
    checkmem(mesh->boundary);

    mesh->xyz = (VECT3D *) malloc(sizeof(VECT3D) * mesh->nnodes);
    checkmem(mesh->xyz);

    mesh->wse = (double *) malloc(sizeof(double) * mesh->nnodes);
    checkmem(mesh->wse);

    mesh->elem2d = (ELEM2D *) malloc(sizeof(ELEM2D) * mesh->nelems2d);
    checkmem(mesh->elem2d);

    mesh->elem1d = (ELEM1D *) malloc(sizeof(ELEM1D) * mesh->nelems1d);
    checkmem(mesh->elem1d);
    
    /* Iniitialize arrays */
    for (i=0; i<mesh->ncorners; i++){ /* Length of cornernodes */
        mesh->cornernodes[i].x = 0.0; // cornernodes[i].x;
        mesh->cornernodes[i].y = 0.0; // cornernodes[i].y;
        mesh->cornernodes[i].z = 0.0; // cornernodes[i].z;
    }
    for (i=0; i<mesh->nnodes; i++){
        mesh->xyz[i].x = 0.0;
        mesh->xyz[i].y = 0.0;
        mesh->xyz[i].z = 0.0;
        mesh->wse[i] = 0.0;
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

void mesh_free(MESH **mesh_ptr, int nmeshes){
    int i;
    for (i=0; i<nmeshes; i++){
        //free((*mesh_ptr)[i].name);
        free((*mesh_ptr)[i].cornernodes);
        free((*mesh_ptr)[i].xyz);
        free((*mesh_ptr)[i].wse);
        free((*mesh_ptr)[i].elem2d);
        free((*mesh_ptr)[i].boundary);
        free((*mesh_ptr)[i].elem1d);
    }
    free((*mesh_ptr));
}
