#include "global_header.h"

static int DEBUG = ON;

/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
void read_mesh_data(MESH **mesh_ptr, int *nummeshes, char infilename[MAXLINE]) {
    char line[MAXLINE];  /* For storing file lines */
    char *data;          /* Pointer moving along the string stored in 'line[]' */
    int tempint, status; /* For using read_int_field */
    double tempdouble;   /* For using read_dbl_filed */
    CARD card;

    int i;
    int nsupmod = NO;
    *nummeshes = 0;
    MESH *mesh; /* Alias */
    int imesh=0;

    FILE *infile = fopen(infilename, "r");
    *mesh_ptr = NULL; /* Just making sure. */

    /* Reading input file to determine the total number of meshes to allocate memory for. */
    while (fgets(line, MAXLINE, infile)!=NULL && *mesh_ptr==NULL){
        data = line;
        card = parse_card(&data);
#ifdef _DEBUG
        // if (DEBUG) printf("\nCard Value = %10u", (unsigned int) card);
#endif
        switch (card){
            case CARD_NSUPMOD:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) nsupmod = tempint;
                //else if (status==READ_FAILURE) throw_error("\nThere was some error reading the int field in the input file.");
                if (DEBUG) printf("\nNumber of supermodels = %i", nsupmod);
                break;
            case CARD_NSUBMOD:
                if (nsupmod==NO) throw_error("NSUPMOD card must be supplied before the NSUBMOD card in the input file.");
                for (i=0; i<nsupmod; i++){
                    tempint = read_int_field(&data,&status);
                    if (tempint<1) throw_error("Expected positive int field");
                    if (status==READ_SUCCESS) *nummeshes += tempint;
                    //else if (status==READ_FAILURE) throw_error("\nThere was some error reading the int field in the input file.");
                }
                if (DEBUG) printf("\nNumber of submodels = %i", *nummeshes);
                *mesh_ptr = (MESH *) malloc(sizeof(MESH) * (*nummeshes));
                break;
            case CARD_NO:
#ifdef _DEBUG
                // if (DEBUG) printf("\nNo card in this line!");
#endif
                continue;
                break;
           default:
                throw_error("Cards NSUPMOD and NSUBMOD must appear first in the input file.");
                break;
        }
    }


    /* Reading input file to allocate memory of variables within each MESH struct. */
    /* Need supID, subID, name, nrows, ncols, and ncorners. */
    rewind(infile);
    imesh = -1;
    mesh = NULL;// &((*mesh_ptr)[0]);
    while (fgets(line, MAXLINE, infile)!=NULL){
        data = line;
        card = parse_card(&data);
#ifdef _DEBUG
        // if (DEBUG) printf("\nCard Value = %10u", (unsigned int) card);
#endif
        switch (card){
            case CARD_SUPMOD:
                imesh++; /* Storing data in a new mesh. */
                mesh = &((*mesh_ptr)[imesh]);
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) mesh->supID = tempint;
                if (DEBUG) printf("\nisupmod = %i", mesh->supID);
                mesh->supID--;
                card = parse_card(&data);
                switch (card){
                    case CARD_SUBMOD:
                        tempint = read_int_field(&data,&status);
                        if (tempint<1) throw_error("Expected positive int field");
                        if (status==READ_SUCCESS) mesh->subID = tempint;
                        if (DEBUG) printf("\nisubmod = %i", mesh->subID);
                        mesh->subID--;
                        switch (parse_card(&data)){
                             case CARD_NAME:
                                 remove_spaces(&data);
                                 char *i = data;
                                 printf("\nExtracting name from string = %s", i);
                                 //read_word_field(&data);
                                 int namelength=0;
                                 while (*i!=' ' && *i!='\t' && *i!='\n' && *i!=0){ /* Caution: Temporary fix only! */
                                     if ((*i>='0' && *i<='9') || (*i>='A' && *i<='z') || (*i=='_' || *i=='.' || *i=='-')){
                                         namelength++;
                                         i++;
                                     }
                                     else{
                                        throw_error("There is some problem in the mesh name supplied by the name card. Only A-Z, a-z, 0-9 and special characters _ . - are allowed. No spaces allowed.");
                                     }
                                 }
                                 strncpy(mesh->name, data, (size_t) namelength);
                                 //sprintf(i,"");
                                 //sprintf(mesh->name, data);
                                 printf("\nMesh name = |%s|", mesh->name);
                                 break;
                             default:
                                 throw_error("Expected card NAME with cards SUBMOD and SUPMOD.");
                                 break;
                        }
                        break;
                    default:
                        throw_error("Expected card SUBMOD with card SUPMOD.");
                        break;
                }
                break;
            case CARD_NROWS:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) mesh->nrows = tempint;
                if (DEBUG) printf("\nmesh->nrows = %i", mesh->nrows);
                break;
            case CARD_NCOLS:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) mesh->ncols = tempint;
                if (DEBUG) printf("\nmesh->ncols = %i", mesh->ncols);
                break;
            case CARD_NCORND:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) mesh->ncorners = tempint;
                if (DEBUG) printf("\nmesh->ncorners = %i", mesh->ncorners);
                assert (mesh->ncorners==NCORNERS);
                break;
            case CARD_NO:
#ifdef _DEBUG
                // if (DEBUG) printf("\nNo card in this line!");
#endif
                continue;
                break;
           default:
                break;
        }
    }

    /* Allocating and initializing memory for variables within each mesh. */
    for (i=0; i<*nummeshes; i++){
        mesh = &((*mesh_ptr)[i]);
        mesh_init(mesh);
    }
    

    /* Reading input file for taking in all the remaining data. */
    rewind(infile);
    imesh = -1;
    mesh = NULL;// &((*mesh_ptr)[0]);
    while (fgets(line, MAXLINE, infile)!=NULL){
        data = line;
        card = parse_card(&data);
#ifdef _DEBUG
        // if (DEBUG) printf("\nCard Value = %10u", (unsigned int) card);
#endif
        switch (card){
            case CARD_SUPMOD:
                imesh++; /* Storing data in a new mesh. */
                mesh = &((*mesh_ptr)[imesh]);
                break;
            case CARD_ND:
                tempint = read_int_field(&data,&status);
                if (tempint<1 || tempint>mesh->ncorners)
                    throw_error("Node ID need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    tempint--;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->cornernodes[tempint].x = tempdouble;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->cornernodes[tempint].y = tempdouble;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->cornernodes[tempint].z = tempdouble;
                }
                if (DEBUG) printf("\nmesh->cornernodes[%i] = (% 23.16e,% 23.16e,% 23.16e)", tempint+1,
                               mesh->cornernodes[tempint].x, mesh->cornernodes[tempint].y, mesh->cornernodes[tempint].z);
                break; 
            case CARD_WSE:
                tempint = read_int_field(&data,&status);
                if (tempint<1 || tempint>mesh->ncorners)
                    throw_error("Node ID need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    tempint--;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->wse[tempint] = tempdouble;
                }
                if (DEBUG) printf("\nmesh->wse[%i] = % 23.16e", tempint+1, mesh->wse[tempint]);
                break;
            case CARD_EGS:
                tempint = read_int_field(&data,&status);
                if (tempint<1 || tempint>mesh->ncorners)
                    throw_error("Node ID need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    int ib = tempint-1;
                    for (i=0; i<NDONSEG; i++){
                        tempint = read_int_field(&data, &status);
                        if (status == READ_SUCCESS) mesh->boundary[ib].nodes[i] = tempint;
                    }
                    if (DEBUG) printf("\nmesh->boundary[%i].nodes = (% 2i, % 2i)", ib+1,
                                   mesh->boundary[ib].nodes[0], mesh->boundary[ib].nodes[1]);
                }
                break; 
            case CARD_NO:
#ifdef _DEBUG
                // if (DEBUG) printf("\nNo card in this line!");
#endif
                continue;
                break;
           default:
                break;
        }
    }



    printf("\n\n\n\nReached EOF\n\n\n\n");
    fclose(infile);
}
