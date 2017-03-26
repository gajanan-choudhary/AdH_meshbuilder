#include "global_header.h"

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
    if (mesh->trn!=NO) fprintf(outfile,"OP TRN  %i\n", mesh->trn);
    fprintf(outfile,"OP BLK  1\n");
    fprintf(outfile,"OP INC  40\n");
    fprintf(outfile,"OP PRE  1\n");
    if (mesh->tem!=NO) fprintf(outfile,"OP TEM  %i\n", mesh->tem);

    fprintf(outfile,"\n#Screen output\nSOUT ALL\n");

    fprintf(outfile,"\nIP NTL    % 9.3E\n", mesh->ntl);
    fprintf(outfile,"IP ITL    % 9.3E\n", mesh->itl);
    fprintf(outfile,"IP NIT     %i\n", mesh->nit);
    fprintf(outfile,"IP MIT     %i\n", mesh->mit);

    fprintf(outfile,"\nMTS        1  1\n");
    fprintf(outfile,"MP ML      1  0\n");
    fprintf(outfile,"MP SRT     1  1.0\n");

    fprintf(outfile,"\nMP EVS     1  0.0  0.0  0.0\n");
    fprintf(outfile,"MP MUC     1.0\n");
    fprintf(outfile,"MP MU      0.0\n");
    fprintf(outfile,"MP RHO     990\n");
    fprintf(outfile,"MP G       9.81\n");

    fprintf(outfile,"\nFR MNG     1  % 9.6E\n\n", mesh->mng);

    for (i=0; i<mesh->nseries; i++){
        fprintf(outfile,"#\nSERIES BC  %i  2  0  0  0  0\n", i);
        fprintf(outfile,"    % 23.15E    % 23.15E\n", mesh->t0,   mesh->series[i]);
        fprintf(outfile,"    % 23.15E    % 23.15E\n", mesh->tf*5, mesh->series[i]);
    }

    fprintf(outfile,"\n#Output\nSERIES AWRITE  %i  1  0  0  0  0\n", mesh->nseries+1);
    fprintf(outfile,"0     % 23.15E     % 23.15E     0\n", mesh->tf*5, mesh->awrite);

    fprintf(outfile,"\n#Time step\nSERIES DT  %i  2  0  0  0  0\n", mesh->nseries+2);
        fprintf(outfile,"    % 23.15E    % 23.15E\n", mesh->t0,   mesh->dt);
        fprintf(outfile,"    % 23.15E    % 23.15E\n", mesh->tf*5, mesh->dt);

    fprintf(outfile,"\nTC T0  % 23.15E  0\n",mesh->t0);
    fprintf(outfile,"TC TF  % 23.15E  0\n",mesh->tf);

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

    fprintf(outfile,"DATASET\n");
    fprintf(outfile,"OBJTYPE \"mesh2D\"\n");
    fprintf(outfile,"BEGSCL\n");
    fprintf(outfile,"ND %10i\n", mesh->nnodes);
    fprintf(outfile,"NC %10i\n", mesh->nelems2d);
    fprintf(outfile,"NAME IOH\n");
    fprintf(outfile,"TS 0 0\n");
    for (i=0; i<mesh->nnodes; i++){
        fprintf(outfile,"    % 23.14e\n", (mesh->xyz[i].z - mesh->wse[i]));
    }

    fprintf(outfile, "ENDDS");
    fclose(outfile);
}

