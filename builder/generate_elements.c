#include "global_header.h"

static int DEBUG = OFF;

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



    count=0;
    ELEM1D *boundary = mesh->boundary;
    ELEM1D *elem1d = mesh->elem1d;
    for (i=0; i<mesh->nboundaries; i++){
        int nedges = NO, nodegap = NO, nodestart = NO;
        switch (boundary[i].nodes[0]){
            case 0:
                nodestart = 0;
                if (boundary[i].nodes[1] == 1){
                    nodegap = 1;
                    nedges  = mesh->nrows-1;
                }
                else if (boundary[i].nodes[1] == 3){
                    nodegap = mesh->nrows;
                    nedges  = mesh->ncols-1;
                }
                break;
            case 1:
                nodestart = mesh->nrows-1;
                if (boundary[i].nodes[1] == 2){
                    nodegap = mesh->nrows;
                    nedges  = mesh->ncols-1;
                }
                else if (boundary[i].nodes[1] == 0){
                    nodegap = -1;
                    nedges  = mesh->nrows-1;
                }
                break;
            case 2:
                nodestart = mesh->nnodes-1;
                if (boundary[i].nodes[1] == 3){
                    nodegap = -1;
                    nedges  = mesh->nrows-1;
                }
                else if (boundary[i].nodes[1] == 1){
                    nodegap = -mesh->nrows;
                    nedges  = mesh->ncols-1;
                }
                break;
            case 3:
                nodestart = mesh->nnodes-mesh->nrows;
                if (boundary[i].nodes[1] == 0){
                    nodegap = -mesh->nrows;
                    nedges  = mesh->ncols-1;
                }
                else if (boundary[i].nodes[1] == 2){
                    nodegap = 1;
                    nedges  = mesh->nrows-1;
                }
                break;
        }
#ifdef _DEBUG
        if (DEBUG) {
            printf("\nnode0 = %i, node1 = %i",boundary[i].nodes[0], boundary[i].nodes[1]);
            printf("\nnodestart = %i, nodegap = %i, nedges = %i", nodestart, nodegap, nedges);
        }
#endif
        for (j=0; j<nedges; j++){
            int node0 = nodestart+j*nodegap;
            int node1 = nodestart+(j+1)*nodegap;
            CREATE_ELEM1D(elem1d[count], mesh->xyz, node0, node1);
            elem1d[count].str = boundary[i].str;
            elem1d[count].type = boundary[i].type;
            elem1d[count].bc_series = boundary[i].bc_series;
            elem1d[count].bc_value = boundary[i].bc_value;
            count++;
        }
    }
    assert(count==mesh->nelems1d);



#ifdef _DEBUG
    if (DEBUG){
        printf("\n");
        for (i=0; i<mesh->nelems2d; i++){
            printf("\nE3T %10i %10i %10i %10i %10i % 23.14e", i+1,
                elem2d[i].nodes[0]+1,elem2d[i].nodes[1]+1, elem2d[i].nodes[2]+1, 1, elem2d[i].area);
        }
    }
    if (DEBUG){
        printf("\n");
        for (i=0; i<mesh->nelems1d; i++){
            printf("\nEGS %10i %10i %10i % 23.14e", i+1,
                elem1d[i].nodes[0]+1,elem1d[i].nodes[1]+1, elem1d[i].length);
        }
    }
#endif
}
