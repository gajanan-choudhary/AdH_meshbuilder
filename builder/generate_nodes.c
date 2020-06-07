#include "global_header.h"

#ifdef _PARABOLIC_BOWL
#define A  1600.0
#define H0    1.58984830714
#else
#define H0   1.0e-9 
#endif

static int DEBUG = ON;

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
            //mesh->xyz[count].z = -H0*mesh->xyz[count].x*mesh->xyz[count].x;
            //double tempx = mesh->xyz[count].x;
            //double tempy = mesh->xyz[count].y;
            //mesh->xyz[count].x = tempx/sqrt(2) - tempy/sqrt(2);
            //mesh->xyz[count].y = tempx/sqrt(2) + tempy/sqrt(2);

#ifdef _PARABOLIC_BOWL
            double r = sqrt(mesh->xyz[count].x*mesh->xyz[count].x+mesh->xyz[count].y*mesh->xyz[count].y);
            mesh->wse[count] = H0*2*mesh->xyz[count].x/A - H0;
            mesh->xyz[count].z = -H0*(1-(r*r)/(A*A));

            double omega = sqrt(2*G*H0)/A;
            mesh->u[count].x = 0.0;
            mesh->u[count].y = sqrt(2*G*H0);//A*omega;
            mesh->u[count].z = 0.0;

            if (mesh->wse[count] - mesh->xyz[count].z < 0) {
                mesh->wse[count] = mesh->xyz[count].z;
                mesh->u[count].y = 0.0;
            }
#endif


#ifdef _TRAPEZOID
            // Side slope: 2
            // Trapezoid width is mapped to reference element.
            double slope = 2.0;
            VECT3D quarter; // centerline values
            if (eta>0.5){
                VECT3D_EVAL_SHP_FNCTN(quarter, ksi, 0.5, mesh->cornernodes);
                mesh->xyz[count].z += fabs(mesh->xyz[count].y - quarter.y)*slope;
            }
            else if (eta<-0.5){
                VECT3D_EVAL_SHP_FNCTN(quarter, ksi,-0.5, mesh->cornernodes);
                mesh->xyz[count].z += fabs(mesh->xyz[count].y - quarter.y)*slope;
            }

            if (mesh->wse[count] - mesh->xyz[count].z < 0) {
                mesh->wse[count] = mesh->xyz[count].z;
            }
#endif


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
