#include "global_header.h"

static int DEBUG = ON;

void generate_elements(MESH *mesh){
    int i, j;
    int count=0, refnode;
    int nodes[NCORNERS];
    int index[NCORNERS];

    // The first leftmost node will always be the reference corner - our choice
    double leftx = leftmostx(mesh->cornernodes);
#ifdef _DEBUG
    if (DEBUG) printf("\nleftmost point x = % 23.14e",leftx);
#endif
    for (i=0; i<NCORNERS; i++) {
#ifdef _DEBUG
         if (DEBUG) printf("\nNode %i (% 23.14e, % 23.14e) distance = % 23.14e", i,
             mesh->cornernodes[i].x, mesh->cornernodes[i].y, (mesh->cornernodes[i].x-leftx));
#endif
        if ( (abs(mesh->cornernodes[i].x-leftx)) < SMALL) {
           index[count] = i;
           count++;
        }
    }
    assert(count>0); /* Otherwise, we need to rewrite the if condition */
    assert(count<3); /* Otherwise, there are at least 3 collinear points! */
    if (count == 1){
        refnode = index[0];
    }
    else /*if (count == 2)*/ {
        refnode = mesh->cornernodes[index[0]].y < mesh->cornernodes[index[1]].y ? index[0] : index[1];
    }
    for (i=0; i<NCORNERS; i++){
        index[i] = (refnode + i) % NCORNERS;
#ifdef _DEBUG
        if (DEBUG){
            printf("\nIndex[%i] = %i", i, index[i]);
        }
#endif
    }

    count=0;
    ELEM2D *elem2d = mesh->elem2d;
    for (i=0; i<mesh->ncols-1; i++){
        for (j=0; j<mesh->nrows-1; j++){ /* Column-first numbering */
            /* Note the indices of nodes[] below! */
            nodes[0] = i*mesh->nrows+j;
            nodes[1] = nodes[0]+1;
            nodes[3] = nodes[0]+mesh->nrows;
            nodes[2] = nodes[3]+1;
            
            CREATE_ELEM2D(elem2d[count],mesh->xyz,nodes[index[3]], nodes[index[0]], nodes[index[1]]);
            count++;
            CREATE_ELEM2D(elem2d[count],mesh->xyz,nodes[index[1]], nodes[index[2]], nodes[index[3]]);
            count++;
        }
    }
    assert(count==mesh->nelems2d);
#ifdef _DEBUG
    if (DEBUG){
        printf("\n");
        for (i=0; i<mesh->nelems2d; i++){
            printf("\nE3T %10i %10i %10i %10i %10i % 23.14e", i+1,
                elem2d[i].nodes[0]+1,elem2d[i].nodes[1]+1, elem2d[i].nodes[2]+1, 1, elem2d[i].area);
        }
    }
#endif
}
