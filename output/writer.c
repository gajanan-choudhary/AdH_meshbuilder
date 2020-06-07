#include "global_header.h"

static int DEBUG = OFF;

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

void write_mesh_file(MESH *mesh){
    int i;
    VECT3D *xyz = mesh->xyz;       /* alias */
    ELEM2D *elem2d = mesh->elem2d; /* alias */

    char meshfilename[MAXLINE];
    sprintf(meshfilename,"%s.3dm",mesh->name);
    FILE *outfile=NULL;
    outfile = fopen(meshfilename,"w");
    assert(outfile!=NULL);


    fprintf(outfile, "MESH2D");
    fprintf(outfile, "\nMESHNAME \"%s\"", mesh->name);
    fprintf(outfile, "\n#Elements = %i", mesh->nelems2d);
    fprintf(outfile, "\n#Nodes    = %i", mesh->nnodes);
    for (i=0; i<mesh->nelems2d; i++){
        fprintf(outfile, "\nE3T %10i %10i %10i %10i %10i", i+1,
            elem2d[i].nodes[0]+1,elem2d[i].nodes[1]+1, elem2d[i].nodes[2]+1, 1);
    }
    for (i=0; i<mesh->nnodes; i++){
        fprintf(outfile, "\nND  % 6i  % 23.14e % 23.14e % 23.14e", i+1,
            xyz[i].x, xyz[i].y, xyz[i].z);
    }
    fprintf(outfile, "\n");
    fclose(outfile);
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

void write_bc_file(MESH *mesh){
    int i;
    ELEM1D *elem1d = mesh->elem1d; /* alias */

    char bcfilename[MAXLINE];
    sprintf(bcfilename,"%s.bc",mesh->name);
    FILE *outfile=NULL;
    outfile = fopen(bcfilename,"w");
    assert(outfile!=NULL);

    fprintf(outfile,"OP SW2\n");
    fprintf(outfile,"OP TRN  %i\n", mesh->trn);
    fprintf(outfile,"OP BLK  1\n");
    fprintf(outfile,"OP INC  40\n");
    fprintf(outfile,"OP PRE  1\n");
    if (mesh->tem!=NO) fprintf(outfile,"OP TEM  %i\n", mesh->tem);
    if (mesh->tpg>=0.0) fprintf(outfile,"OP TPG  %i\n", mesh->tpg);

    fprintf(outfile,"\n#Screen output\nSOUT ALL\n");

    fprintf(outfile,"\nIP NTL    % 9.3E\n", mesh->ntl);
    fprintf(outfile,"IP ITL    % 9.3E\n", mesh->itl);
    fprintf(outfile,"IP NIT     %i\n", mesh->nit);
    fprintf(outfile,"IP MIT     %i\n", mesh->mit);

    fprintf(outfile,"\nMTS        1  1\n");
    fprintf(outfile,"MP ML      1  0\n");
    fprintf(outfile,"MP SRT     1  100\n");

    fprintf(outfile,"\nMP EVS     1  0.00  0.00  0.00  0.00  0.00  0.00\n");
    fprintf(outfile,"MP MUC     1.0\n");
    fprintf(outfile,"MP MU      0.0\n");
    fprintf(outfile,"MP RHO     990\n");
    fprintf(outfile,"MP G       9.81\n");
    if (mesh->dtl>0.0) fprintf(outfile, "MP DTL     %23.15E",mesh->dtl);

    fprintf(outfile,"\nFR MNG     1  % 9.6E\n\n", mesh->mng);

    for (i=0; i<mesh->nseries; i++){
        fprintf(outfile,"#\nSERIES BC  %i  2  0  0  0  0\n", i+1);
        fprintf(outfile,"    % 14.6E    % 14.6E\n",-mesh->tf*100,   mesh->series[i]);
        fprintf(outfile,"    % 14.6E    % 14.6E\n", mesh->tf*100, mesh->series[i]);
    }
    ///* GKC temporary */
    //int j, ntsteps = (int)((mesh->tf-mesh->t0)/mesh->dt) + 3;
    //double curt = 0.;
    //fprintf(outfile,"#\nSERIES BC  %i  %i  0  0  0  0\n", i+1, ntsteps);
    //for (j=0, curt=0.0; j<ntsteps; j++, curt+=mesh->dt){
    //    fprintf(outfile,"    % 14.6E    % 14.6E\n", curt, 3.0*(sin(4.0*PI*curt/mesh->tf)));
    //}

    fprintf(outfile,"\n#Output\nSERIES AWRITE  %i  1  0  0  0  0\n", mesh->nseries+1);
    fprintf(outfile,"% 14.6E     % 14.6E     % 14.6E     0\n",-mesh->tf*100, mesh->tf*100, mesh->awrite);

    fprintf(outfile,"\n#Time step\nSERIES DT  %i  2  0  0  0  0\n", mesh->nseries+2);
        fprintf(outfile,"    % 14.6E    % 14.6E\n",-mesh->tf*100,   mesh->dt);
        fprintf(outfile,"    % 14.6E    % 14.6E\n", mesh->tf*100, mesh->dt);

    fprintf(outfile,"\nTC T0  % 14.6E  0\n",mesh->t0);
    fprintf(outfile,"TC TF  % 14.6E  0\n",mesh->tf);

    fprintf(outfile,"\n#Enable XDMF output\n");
    fprintf(outfile,"PC XDF\n");
    
    fprintf(outfile,"\n#Shallow water boundary Conditions");
    for (i=0; i<mesh->nboundaries; i++){
        switch (mesh->boundary[i].type){
            case FLOW:
                fprintf(outfile,"\nNB VEL  %3i  %3i", mesh->boundary[i].str+1, mesh->boundary[i].bc_series+1);
                if (abs(mesh->boundary[i].bc_value) < SMALL) {
                    fprintf(outfile,"        ! No Flow");
                }
                else if (mesh->boundary[i].bc_value > SMALL) {
                    fprintf(outfile,"        ! Inflow");
                }
                else if (mesh->boundary[i].bc_value < -SMALL) {
                    fprintf(outfile,"        ! Outflow");
                }
                break;
            case DISCHARGE:
                fprintf(outfile,"\nNB DIS  %3i  %3i", mesh->boundary[i].str+1, mesh->boundary[i].bc_series+1);
                if (abs(mesh->boundary[i].bc_value) < SMALL) {
                    fprintf(outfile,"        ! No Flow");
                }
                else if (mesh->boundary[i].bc_value > SMALL) {
                    fprintf(outfile,"        ! Inflow");
                }
                else if (mesh->boundary[i].bc_value < -SMALL) {
                    fprintf(outfile,"        ! Outflow");
                }
                break;
            case TAILWATER:
                fprintf(outfile,"\nNB OTW  %3i  %3i        ! Tail Water", mesh->boundary[i].str+1,mesh->boundary[i].bc_series+1);
                break;
            case STRONG_INTERFACE:
                fprintf(outfile,"\nHY INT  %3i             ! Strongly coupled interface", mesh->boundary[i].str+1);
                break;
            case WEAK_INTERFACE:
                fprintf(outfile,"\nHY EXT  %3i             ! Weakly coupled interface", mesh->boundary[i].str+1);
                break;
            default:
                throw_error("Only VEL, DIS, OTW, INT, and EXT boundaries supported for now.");
                break;
        }
    }

    fprintf(outfile,"\n\n#Edge list below:\n");
    for (i=0; i<mesh->nelems1d; i++){
        fprintf(outfile,"EGS %10i %10i %10i\n", elem1d[i].nodes[0]+1, elem1d[i].nodes[1]+1, elem1d[i].str+1);
    }

    fprintf(outfile, "\nEND");
    fclose(outfile);
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

void write_hotstart_file(MESH *mesh){
    int i;

    char hotstartfilename[MAXLINE];
    sprintf(hotstartfilename,"%s.hot",mesh->name);
    FILE *outfile=NULL;
    outfile = fopen(hotstartfilename,"w");
    assert(outfile!=NULL);

    /* Write depths. */
    fprintf(outfile,"DATASET\n");
    fprintf(outfile,"OBJTYPE \"mesh2D\"\n");
    fprintf(outfile,"BEGSCL\n");
    fprintf(outfile,"ND %10i\n", mesh->nnodes);
    fprintf(outfile,"NC %10i\n", mesh->nelems2d);
    fprintf(outfile,"NAME IOH\n");
    fprintf(outfile,"TS 0 0\n");
    for (i=0; i<mesh->nnodes; i++){
        double depth = (mesh->wse[i] - mesh->xyz[i].z);
        depth = max2(-SMALL, depth);
        fprintf(outfile,"    % 20.15f\n", depth);
    }
    fprintf(outfile, "ENDDS");

    ///* Write concentration. */
    //fprintf(outfile,"\nDATASET\n");
    //fprintf(outfile,"OBJTYPE \"mesh2D\"\n");
    //fprintf(outfile,"BEGSCL\n");
    //fprintf(outfile,"ND %10i\n", mesh->nnodes);
    //fprintf(outfile,"NC %10i\n", mesh->nelems2d);
    //fprintf(outfile,"NAME ICON 1\n");
    //fprintf(outfile,"TS 0 0\n");
    //for (i=0; i<mesh->nnodes; i++){
    //    double conc = 35.0*(mesh->xyz[i].x - mesh->cornernodes[1].x)
    //                /      (mesh->cornernodes[0].x - mesh->cornernodes[1].x);
    //    conc = max2(0.0, conc);
    //    fprintf(outfile,"    % 20.15f\n", conc);
    //}
    //fprintf(outfile, "ENDDS");
#ifdef _PARABOLIC_BOWL
    /* Write velocity. */
    fprintf(outfile,"\nDATASET\n");
    fprintf(outfile,"OBJTYPE \"mesh2D\"\n");
    fprintf(outfile,"BEGVEC\n");
    fprintf(outfile,"ND %10i\n", mesh->nnodes);
    fprintf(outfile,"NC %10i\n", mesh->nelems2d);
    fprintf(outfile,"NAME IV\n");
    fprintf(outfile,"TS 0 0\n");
    for (i=0; i<mesh->nnodes; i++){
        fprintf(outfile,"    % 23.15e % 23.15e %23.15e\n", mesh->u[i].x, mesh->u[i].y, mesh->u[i].z);
    }
    fprintf(outfile, "ENDDS");
#endif
    fclose(outfile);
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

void write_superfile(MESH *mesh, int nmeshes){
    int i, j, k;
    FILE *outfile=NULL;
    outfile = fopen("superfile.in","w");
    assert(outfile!=NULL);

    int nsupmod=NO;
    int *nsubmod=NULL;
    for (i=0; i<nmeshes; i++){
        nsupmod=max2(nsupmod, mesh[i].supID);
    }
    nsupmod++;
    nsubmod = (int *) malloc(sizeof(int) * nsupmod);
    checkmem(nsubmod);
    for (i=0; i<nmeshes; i++){
        nsubmod[mesh[i].supID]=max2(nsubmod[mesh[i].supID], mesh[i].subID);
    }
    for (i=0; i<nsupmod; i++) nsubmod[i]++;
    
    //int nsuperinterfaces=0;
    for (i=0; i<nsupmod; i++){
        fprintf(outfile, "################################################################# SUPERMODEL %i", i+1);
        fprintf(outfile, "\nSMODEL  %i", nsubmod[i]);
        int order = 0;
        for (j=0; j<nmeshes; j++){
            if (mesh[j].supID==i && mesh[j].subID == order){
                fprintf(outfile, "\n    %s", mesh[j].name);
                order++;
            }
        }
        int nsubinterfaces=0;
        for (j=0; j<nmeshes; j++){
            if (mesh[j].supID!=i) continue;
            for (k=0; k<mesh[j].nboundaries; k++){
                if(mesh[j].boundary[k].type == STRONG_INTERFACE){
                    nsubinterfaces++;
                }
                //if(mesh[j].boundary[k].type == WEAK_INTERFACE){
                //    nsuperinterfaces++;
                //}
            }
        }
        if (nsubinterfaces%2!=0) throw_error("There have to be an even number of HY INT cards in the input file.");
        nsubinterfaces/=2;

        for (j=0; j<nmeshes; j++){
            if (mesh[j].supID==i) break;
        }
        /* Note : using the value of j below! */
        fprintf(outfile, "\n\n    OP BLK  1");
        fprintf(outfile, "\n    OP INC  40");
        fprintf(outfile, "\n    OP PRE  1");
        fprintf(outfile, "\n    IP NTL  % 23.14E", mesh[j].ntl);
        fprintf(outfile, "\n    IP ITL  % 23.14E", mesh[j].itl);
        fprintf(outfile, "\n    IP NIT  %3i", mesh[j].nit);
        fprintf(outfile, "\n    IP MIT  %3i", mesh[j].mit);


        fprintf(outfile, "\n\n    #Overriding timing information:");
        fprintf(outfile, "\n    TC T0  % 9.1f", mesh[j].t0);
        fprintf(outfile, "\n    TC TF  % 9.1f", mesh[j].tf);
        fprintf(outfile, "\n    TC DT  % 9.1f", mesh[j].dt);
        /* Note : stopped using the value of j now. */

        if (nsubinterfaces>0) {
            fprintf(outfile, "\n\n    NUMIFC  %i", nsubinterfaces);
            for (j=0; j<nmeshes; j++){
                if (mesh[j].supID!=i) continue;
                for (k=0; k<mesh[j].nboundaries; k++){
                    if(mesh[j].boundary[k].type == STRONG_INTERFACE){
                        ELEM1D *b1 = &(mesh[j].boundary[k]);
                        int /* othermodelfound = NO,*/ jj, kk;
                        for (jj=j+1; jj<nmeshes; jj++){
                            for (kk=0; kk<mesh[jj].nboundaries; kk++){
                                if (mesh[jj].boundary[kk].type == STRONG_INTERFACE){
                                    ELEM1D *b2 = &(mesh[jj].boundary[kk]);
                                    double dist1 = PROJECTED_DISTANCE(b1->coord[0].x, b1->coord[0].y, b2->coord[0].x, b2->coord[0].y);
                                    double dist2 = PROJECTED_DISTANCE(b1->coord[1].x, b1->coord[1].y, b2->coord[1].x, b2->coord[1].y);
                                    double dist3 = PROJECTED_DISTANCE(b1->coord[0].x, b1->coord[0].y, b2->coord[1].x, b2->coord[1].y);
                                    double dist4 = PROJECTED_DISTANCE(b1->coord[1].x, b1->coord[1].y, b2->coord[0].x, b2->coord[0].y);
                                    if ((dist1<SMALL && dist2<SMALL) || (dist3<SMALL && dist4<SMALL)){
                                        int nnodes1 = count_nodes(mesh[j].elem1d, mesh[j].nelems1d,  b1->str);
                                        int nnodes2 = count_nodes(mesh[jj].elem1d, mesh[jj].nelems1d, b2->str);
#ifdef _DEBUG
                                        if (DEBUG) {
                                            printf("\ndist1 = %23.14e, dist2 = %23.14e\ndist3 = %23.14e, dist4 = %23.14e", dist1, dist2, dist3, dist4);
                                            printf_elem1d(mesh[j].boundary, k);
                                            printf_elem1d(mesh[jj].boundary, kk);
                                            printf("\nnnodes1 = %i, nnodes2 = %i\n", nnodes1, nnodes2);
                                        }
#endif
                                        if (nnodes1!=nnodes2) throw_error("Number of nodes along the interface should be equal on both the models which share it. Check the cards NROWS and NCOLS in the input file.");
                                        fprintf(outfile, "\n    INTFCE  %i  %i  %i", mesh[j].subID+1, mesh[jj].subID+1, nnodes1);
                                        fprintf(outfile, "    !  Interface between submodel %i and %i contains %i coupled node columns.", mesh[j].subID+1, mesh[jj].subID+1, nnodes1);
                                        // othermodelfound=YES;
                                    }
                                }
                            }
                        }
                        //if (othermodelfound==NO) throw_error("Please check the model node corners to make sure there are overlapping interface edges. Could not find the coupled model counterpart of at least one model.")
                    }
                }
            }
        }
        // fprintf(outfile, "");
        // fprintf(outfile, "");
        // fprintf(outfile, "");
        // fprintf(outfile, "");
        fprintf(outfile, "\n\n");
    }
    //if (nsuperinterfaces%2!=0) throw_error("There have to be an even number of HY EXT or HY CPL cards in the input file.");
    //nsuperinterfaces/=2;
    //if (nsuperinterfaces>0){
        fprintf(outfile, "\n\n################################################################# Superinterface data");
        for (j=0; j<nmeshes; j++){
            for (k=0; k<mesh[j].nboundaries; k++){
                if(mesh[j].boundary[k].type != STRONG_INTERFACE){
                    ELEM1D *b1 = &(mesh[j].boundary[k]);
                    int /* othermodelfound = NO,*/ jj, kk;
                    for (jj=j+1; jj<nmeshes; jj++){
                        for (kk=0; kk<mesh[jj].nboundaries; kk++){
                            if (mesh[jj].boundary[kk].type != STRONG_INTERFACE){
                                ELEM1D *b2 = &(mesh[jj].boundary[kk]);
                                double dist1 = PROJECTED_DISTANCE(b1->coord[0].x, b1->coord[0].y, b2->coord[0].x, b2->coord[0].y);
                                double dist2 = PROJECTED_DISTANCE(b1->coord[1].x, b1->coord[1].y, b2->coord[1].x, b2->coord[1].y);
                                double dist3 = PROJECTED_DISTANCE(b1->coord[0].x, b1->coord[0].y, b2->coord[1].x, b2->coord[1].y);
                                double dist4 = PROJECTED_DISTANCE(b1->coord[1].x, b1->coord[1].y, b2->coord[0].x, b2->coord[0].y);
                                if ((dist1<SMALL && dist2<SMALL) || (dist3<SMALL && dist4<SMALL)){
                                    int nedges1 = count_nodes(mesh[j].elem1d, mesh[j].nelems1d,  b1->str);
                                    int nedges2 = count_nodes(mesh[jj].elem1d, mesh[jj].nelems1d, b2->str);
                                    nedges1--; nedges2--; // #edges = #nodes-1 for 'open' interfaces
#ifdef _DEBUG
                                    if (DEBUG) {
                                        printf("\ndist1 = %23.14e, dist2 = %23.14e\ndist3 = %23.14e, dist4 = %23.14e", dist1, dist2, dist3, dist4);
                                        printf_elem1d(mesh[j].boundary, k);
                                        printf_elem1d(mesh[jj].boundary, kk);
                                        printf("\nnedges1 = %i, nedges2 = %i\n", nedges1, nedges2);
                                    }
#endif
                                    if (nedges1!=nedges2) throw_error("Number of nodes along the interface should be equal on both the models which share it. Check the cards NROWS and NCOLS in the input file.");
                                    fprintf(outfile, "\nSUPIFC  %i  %i", mesh[j].supID+1, mesh[jj].supID+1);
                                    fprintf(outfile, "    !  Interface between supermodels %i and %i", mesh[j].supID+1, mesh[jj].supID+1);
                                    fprintf(outfile, "\n    IFCSM  %i  %i  %i", mesh[j].subID+1, mesh[jj].subID+1, nedges1);
                                    fprintf(outfile, "    !  Interface between supermodel %i, submodel %i and supermodel %i, submodel %i contains %i coupled edge/face columns.",
                                                                           mesh[j].supID+1, mesh[j].subID+1, mesh[jj].supID+1, mesh[jj].subID+1, nedges1);
                                    fprintf(outfile, "\n    CPL    %i  %i", b1->str+1, b2->str+1);
                                    fprintf(outfile, "       !  Edge/Face strings %i and %i are weakly coupled.",b1->str+1, b2->str+1);
                                    // othermodelfound=YES;
                                }
                            }
                        }
                    }
                    //if (othermodelfound==NO) throw_error("Please check the model node corners to make sure there are overlapping interface edges. Could not find the coupled model counterpart of at least one model.")
                }
            }
        }

    //}

    free(nsubmod);
    fclose(outfile);
}

int count_nodes(ELEM1D *elem1d, int nelems1d, int str /* Edge string ID */){
    int i;
    int nnodes = 0;
    for (i=0; i<nelems1d; i++){
        if (elem1d[i].str == str){
            nnodes++;
        }
    }
    nnodes++; // Number of nodes = number of #edges + 1; // For 'open' interfaces
    return nnodes;
}
