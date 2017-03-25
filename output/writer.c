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

    double endtime = 21600.0;
    double writeinterval = 300.0;
    double dt = 50.0;

    char bcfilename[MAXLINE];
    sprintf(bcfilename,"%s.bc",mesh->name);
    FILE *outfile=NULL;
    outfile = fopen(bcfilename,"w");
    assert(outfile!=NULL);

    fprintf(outfile,"OP SW2\n");
    fprintf(outfile,"OP TRN 0\n");
    fprintf(outfile,"OP BLK 1\n");
    fprintf(outfile,"OP INC 40\n");
    fprintf(outfile,"OP PRE 1\n");
    fprintf(outfile,"OP TEM 1\n");

    fprintf(outfile,"\n#Screen output\nSOUT ALL\n");

    fprintf(outfile,"\nIP NTL     1.0e-06\n");
    fprintf(outfile,"IP ITL     1.0e-06\n");
    fprintf(outfile,"IP NIT     10\n");
    fprintf(outfile,"IP MIT     500\n");

    fprintf(outfile,"\nMTS        1  1\n");
    fprintf(outfile,"MP ML      1  0\n");
    fprintf(outfile,"MP SRT     1  1.0\n");

    fprintf(outfile,"\nMP EVS     1  0.0  0.0  0.0\n");
    fprintf(outfile,"MP MUC     1.0\n");
    fprintf(outfile,"MP MU      0.0\n");
    fprintf(outfile,"MP RHO     990\n");
    fprintf(outfile,"MP G       9.81\n");

    fprintf(outfile,"\nFR MNG     1  0.0\n");

    fprintf(outfile,"\n#No flow\nSERIES BC  1  2  0  0  0  0\n");
    fprintf(outfile,"%15.3e %15.3e\n", 0.0, 0.0);
    fprintf(outfile,"%15.3e %15.3e\n", endtime*5, 0.0);

    fprintf(outfile,"\n#Output\nSERIES AWRITE  2  1  0  0  0  0\n");
    fprintf(outfile,"0 %15.3e %15.3e 0\n", endtime*5, writeinterval);

    fprintf(outfile,"\n#Tail water\nSERIES DT  3  2  0  0  0  0\n");
    fprintf(outfile,"%15.3e %15.3e\n", 0.0, dt);
    fprintf(outfile,"%15.3e %15.3e\n", endtime*5, dt);

    fprintf(outfile,"\nTC T0 %10.3e 0\n",2*dt);
    fprintf(outfile,"TC TF %10.3e 0\n",endtime);

    fprintf(outfile,"\n#BCS\n");
    for (i=0; i<NCORNERS; i++){
        switch (mesh->boundary[i].str){
            case VEL:
                fprintf(outfile,"\nNB VEL %3i %3i", i+2,i+1);
                if (abs(mesh->boundary[i].bc_value) < SMALL) {
                    fprintf(outfile,"        ! No Flow");
                }
                else if (mesh->boundary[i].bc_value > SMALL) {
                    fprintf(outfile,"        ! Inflow");
                }
                else {
                    fprintf(outfile,"        ! Outflow");
                }
                break;
            case DIS:
                fprintf(outfile,"\nNB DIS %3i %3i", i+2,i+1);
                if (abs(mesh->boundary[i].bc_value) < SMALL) {
                    fprintf(outfile,"        ! No Flow");
                }
                else if (mesh->boundary[i].bc_value > SMALL) {
                    fprintf(outfile,"        ! Inflow");
                }
                else {
                    fprintf(outfile,"        ! Outflow");
                }
                break;
            case OTW:
                fprintf(outfile,"\nNB OTW %3i %3i        ! Tail Water", i+2,i+1);
                break;
            case INT:
                fprintf(outfile,"\nHY INT %3i %3i        ! Interface", i+2,i+1);
                break;
            default:
                throw_error("Only VEL, DIS, OTW, and INT boundaries supported for now.");
                break;
        }
    }

    fprintf(outfile,"\n#Edge list below:\n");
    for (i=0; i<mesh->nelems1d; i++){
        fprintf(outfile,"EGS %10i %10i %10i\n", elem1d[i].nodes[0], elem1d[i].nodes[1], elem1d[i].str);
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

