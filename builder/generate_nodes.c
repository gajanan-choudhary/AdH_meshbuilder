#include "global_header.h"

static int DEBUG = OFF;

void generate_nodes(MESH *mesh){
    int i, j;
    double ksi, eta;
    int count = 0;

#ifdef _COLUMNFIRST
    for (i=0; i<mesh->ncols; i++){
        ksi = -1.0 + 2.0*(double)i/(double)(mesh->ncols-1);
        for (j=0; j<mesh->nrows; j++){ /* Column-first numbering */
            eta = -1.0 + 2.0*(double)j/(double)(mesh->nrows-1);
#else
    for (j=0; j<mesh->nrows; j++){
        eta = -1.0 + 2.0*(double)j/(double)(mesh->nrows-1);
        for (i=0; i<mesh->ncols; i++){ /* Row-first numbering */
            ksi = -1.0 + 2.0*(double)i/(double)(mesh->ncols-1);
#endif
            VECT3D_EVAL_SHP_FNCTN(mesh->xyz[count], ksi, eta, mesh->cornernodes);
            mesh->wse[count] = EVAL_SHP_FNCTN(ksi,eta,mesh->cornerwse);

#ifdef _DEBUG
            if (DEBUG) printf("\nND |% 6i| % 23.14e | % 23.14e | % 23.14e | % 23.14e", count+1,
                           mesh->xyz[count].x, mesh->xyz[count].y, mesh->xyz[count].z, mesh->wse[count]);
#endif
            count++;
        }
#ifdef _DEBUG
            if (DEBUG) printf("\nFormat: | NodeID | X | Y | Bathymetry Surf. El. | Water Surf. El. |");
            if (DEBUG) printf("\n");
#endif
    }
}
