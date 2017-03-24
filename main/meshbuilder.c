#include "global_header.h"

int main(/*int argc, char *argv[]*/){

    // FILE *infile;

    // if (argc != 2){
    //     throw_error("Error! The program needs exactly 1 command line arguement as the input, viz. the input file name.");
    // }
    // else{
    //     infile = fopen(argv[1],'r');
    //     if (infile ==NULL) throw_error("Error! Input file not found. Please check the command line argument.");
    // }

    int nrows = 3;
    int ncols = 5;
    VECT3D cornerxyz[NCORNERS];    /* NCORNERS = 4 */
    int ord[NCORNERS] = {3, 0, 1, 2}; /* Order of nodes */
    cornerxyz[ord[0]].x = -10000.; cornerxyz[ord[0]].y =     0.; cornerxyz[ord[0]].z = -40.0;
    cornerxyz[ord[1]].x =      0.; cornerxyz[ord[1]].y =     0.; cornerxyz[ord[1]].z = -30.0;
    cornerxyz[ord[2]].x =      0.; cornerxyz[ord[2]].y = 10000.; cornerxyz[ord[2]].z = -20.0;
    cornerxyz[ord[3]].x = -10000.; cornerxyz[ord[3]].y = 10000.; cornerxyz[ord[3]].z = -30.0;


    // int i;

    MESH *mesh = NULL;
    //int nmeshes = 1;
    //MESH *mesh = (MESH *) malloc(sizeof(MESH)  * nmeshes);
    //for (i=0; i<nmeshes; i++){
    //    char meshname[10];
    //    mesh.name = strcpy(meshname, mesh.name);
    //}

    mesh_alloc_init(&mesh, nrows, ncols, cornerxyz);

    generate_nodes(mesh);

    generate_elements(mesh);

    write_mesh_file(mesh);

//    write_bc_file(mesh);

    write_hotstart_file(mesh);

    mesh_free(&mesh);

    printf("\nProject run successful. Exiting...\n\n");

    return 1;
}
