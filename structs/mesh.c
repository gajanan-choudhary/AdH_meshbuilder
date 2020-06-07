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

void mesh_defaults(MESH *mesh, int nmeshes){
    int i;
    for (i=0; i<nmeshes; i++){
        mesh[i].supID       = NO;
        mesh[i].subID       = NO;
        mesh[i].nrows       = 0;
        mesh[i].ncols       = 0;
        mesh[i].nnodes      = 0;
        mesh[i].nelems2d    = 0;
        mesh[i].nelems1d    = 0;
        mesh[i].ncorners    = 0;
        mesh[i].nboundaries = 0;
        mesh[i].nseries     = 0;

        mesh[i].trn    = 0;
        mesh[i].tem    = NO;
        mesh[i].tpg    = -3.0; /* UNSET_FLT */
        mesh[i].nit    = 20;
        mesh[i].mit    = 500;
        mesh[i].ntl    = 1.000E-12;
        mesh[i].itl    = 1.000E-12;
        mesh[i].mng    = 0.000E+00;
        mesh[i].t0     = 0.000E+00;
        mesh[i].tf     = 1.000E+04;
        mesh[i].dt     = 1.000E+02;
        mesh[i].awrite = 1.000E+02;
        mesh[i].dtl    = 0.000E+00;

        mesh[i].cornerwse   = NULL;
        mesh[i].wse         = NULL;
        mesh[i].series      = NULL;
        mesh[i].cornernodes = NULL;
        mesh[i].xyz         = NULL;
#ifdef _PARABOLIC_BOWL
        mesh[i].u           = NULL;
#endif
        mesh[i].elem2d      = NULL;
        mesh[i].boundary    = NULL;
        mesh[i].elem1d      = NULL;
    }
}

void mesh_init(MESH *mesh){
    int i, j; /* Loop counters */

    assert(mesh->nrows>1);
    assert(mesh->ncols>1);
    assert(mesh->ncorners==NCORNERS);
    assert(mesh->nseries>0);

    //mesh->name = (char *) malloc(sizeof(char) * MAXLINE);
    //sprintf(mesh->name, "mesh");
    mesh->nnodes = mesh->nrows*mesh->ncols;
    mesh->nelems2d = 2* (mesh->nrows-1)*(mesh->ncols-1);
    mesh->nelems1d = 2*((mesh->nrows-1)+(mesh->ncols-1));
    mesh->nboundaries = mesh->ncorners; /* Quadrilateral domain, by choice */

    mesh->cornerwse = (double *) malloc(sizeof(double) * mesh->ncorners);
    checkmem(mesh->cornerwse);

    mesh->wse = (double *) malloc(sizeof(double) * mesh->nnodes);
    checkmem(mesh->wse);

    mesh->series = (double *) malloc(sizeof(double) * mesh->nseries);
    checkmem(mesh->series);

    mesh->cornernodes = (VECT3D *) malloc(sizeof(VECT3D) * mesh->ncorners);
    checkmem(mesh->cornernodes);

    mesh->xyz = (VECT3D *) malloc(sizeof(VECT3D) * mesh->nnodes);
    checkmem(mesh->xyz);

#ifdef _PARABOLIC_BOWL
    mesh->u = (VECT3D *) malloc(sizeof(VECT3D) * mesh->nnodes);
    checkmem(mesh->u);
#endif

    mesh->elem2d = (ELEM2D *) malloc(sizeof(ELEM2D) * mesh->nelems2d);
    checkmem(mesh->elem2d);

    mesh->boundary = (ELEM1D *) malloc(sizeof(ELEM1D) * mesh->nboundaries);
    checkmem(mesh->boundary);

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
#ifdef _PARABOLIC_BOWL
        mesh->u[i].x = 0.0;
        mesh->u[i].y = 0.0;
        mesh->u[i].z = 0.0;
#endif
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
        free((*mesh_ptr)[i].cornerwse);
        free((*mesh_ptr)[i].wse);
        free((*mesh_ptr)[i].series);
        free((*mesh_ptr)[i].cornernodes);
        free((*mesh_ptr)[i].xyz);
#ifdef _PARABOLIC_BOWL
        free((*mesh_ptr)[i].u);
#endif
        free((*mesh_ptr)[i].elem2d);
        free((*mesh_ptr)[i].boundary);
        free((*mesh_ptr)[i].elem1d);
    }
    free((*mesh_ptr));
}
