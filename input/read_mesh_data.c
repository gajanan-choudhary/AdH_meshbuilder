#include "global_header.h"

static int DEBUG = OFF;

/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
void read_mesh_data(MESH **mesh_ptr, int *nummeshes, char infilename[MAXLINE]) {
    char line[MAXLINE];  /* For storing file lines */
    char *data;          /* Pointer moving along the string stored in 'line[]' */
    int tempint, status; /* For using read_int_field */
    double tempdouble;   /* For using read_dbl_filed */

    int i=0, imesh=-1;   /* Counters */
    int iscommon = 0;    /* Used to make sure there's one each of COMMON START and COMMON END cards in the input file. */
    int nsupmod = NO;
    CARD card;
    MESH *mesh = NULL;   /* Alias */

    FILE *infile = fopen(infilename, "r");

    /* Reading input file to determine the total number of meshes to allocate memory for. */
    *mesh_ptr = NULL; /* Just making sure to start off with. */
    *nummeshes = 0;
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

    mesh_defaults(*mesh_ptr, *nummeshes);

    /* Reading input file to allocate memory of variables within each MESH struct. */
    /* Need supID, subID, name, nrows, ncols, and ncorners. */
    rewind(infile);
    imesh = -1;
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
                if (DEBUG) printf("\nmesh->supID = %i", mesh->supID);
                mesh->supID--;
                card = parse_card(&data);
                switch (card){
                    case CARD_SUBMOD:
                        tempint = read_int_field(&data,&status);
                        if (tempint<1) throw_error("Expected positive int field");
                        if (status==READ_SUCCESS) mesh->subID = tempint;
                        if (DEBUG) printf("\nmesh->subID = %i", mesh->subID);
                        mesh->subID--;
                        switch (parse_card(&data)){
                             case CARD_NAME:
                                 remove_spaces(&data);
                                 char *i = data;
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
                                 if (DEBUG) printf("\nMesh name = |%s|", mesh->name);
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
            case CARD_BC:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected positive int field");
                if (status==READ_SUCCESS) mesh->nseries = max2(tempint,mesh->nseries);
                if (DEBUG) printf("\nmesh->nseries = %i", mesh->nseries);
                break;
            case CARD_COMMON:
                card = parse_card(&data);
                if (card == CARD_START) iscommon++;
                if ((card == CARD_END) && (iscommon==1)) iscommon++;
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
    if (iscommon != 2) throw_error("There must be exactly one each of COMMON START and COMMON END cards in the input file, with the cards in between indicating data common between all supermodel and submodel meshes");

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
                    throw_error("Node IDs need to lie between 1 and NCORND <ncorners>, inclusive");
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
                    throw_error("Node IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    tempint--;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->cornerwse[tempint] = tempdouble;
                }
                if (DEBUG) printf("\nmesh->cornerwse[%i] = % 23.16e", tempint+1, mesh->cornerwse[tempint]);
                break;
            case CARD_EGS:
                tempint = read_int_field(&data,&status);
                if (tempint<1 || tempint>mesh->ncorners)
                    throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    int ib = tempint-1;
                    for (i=0; i<NDONSEG; i++){
                        tempint = read_int_field(&data, &status);
                        if (status == READ_SUCCESS) mesh->boundary[ib].nodes[i] = tempint-1;
                    }
                    CREATE_ELEM1D(mesh->boundary[ib], mesh->cornernodes, mesh->boundary[ib].nodes[0], mesh->boundary[ib].nodes[1]);
                    if (DEBUG) printf_elem1d(mesh->boundary, ib);
                    // if (DEBUG) printf("\nmesh->boundary[%i].nodes = (% 2i, % 2i)", ib+1,
                    //                mesh->boundary[ib].nodes[0], mesh->boundary[ib].nodes[1]);
                }
                break;
            case CARD_BC:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Expected a positive int field");
                if (status==READ_SUCCESS){
                    tempint--;
                    tempdouble = read_dbl_field(&data, &status);
                    if (status == READ_SUCCESS) mesh->series[tempint] = tempdouble;
                }
                break;
            case CARD_INT:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS) tempint--;
                mesh->boundary[tempint].str = tempint + SHIFT_EDGESTRING_IDS; /* Numbering starting 0 for storage. 1 will be added while writing output. */
                mesh->boundary[tempint].type = STRONG_INTERFACE;
                break;
            case CARD_EXT:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS) tempint--;
                mesh->boundary[tempint].str = tempint + SHIFT_EDGESTRING_IDS; /* Numbering starting 0 for storage. 1 will be added while writing output. */
                mesh->boundary[tempint].type = WEAK_INTERFACE;
                break;
            case CARD_VEL:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    int ib = tempint-1;
                    mesh->boundary[ib].str = ib + SHIFT_EDGESTRING_IDS; /* Numbering starting 0 for storage. 1 will be added while writing output. */
                    tempint = read_int_field(&data, &status);
                    if (status == READ_SUCCESS) tempint--;
                    mesh->boundary[ib].type = FLOW;
                    mesh->boundary[ib].bc_series = tempint;
                }
                break;
            case CARD_OTW:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    int ib = tempint-1;
                    mesh->boundary[ib].str = ib + SHIFT_EDGESTRING_IDS; /* Numbering starting 0 for storage. 1 will be added while writing output. */
                    tempint = read_int_field(&data, &status);
                    if (status == READ_SUCCESS) tempint--;
                    mesh->boundary[ib].type = TAILWATER;
                    mesh->boundary[ib].bc_series = tempint;
                }
                break;
            case CARD_DIS:
                tempint = read_int_field(&data,&status);
                if (tempint<1) throw_error("Boundary IDs need to lie between 1 and NCORND <ncorners>, inclusive");
                if (status==READ_SUCCESS){
                    int ib = tempint-1;
                    mesh->boundary[ib].str = ib + SHIFT_EDGESTRING_IDS; /* Numbering starting 0 for storage. 1 will be added while writing output. */
                    tempint = read_int_field(&data, &status);
                    if (status == READ_SUCCESS) tempint--;
                    mesh->boundary[ib].type = DISCHARGE;
                    mesh->boundary[ib].bc_series = tempint;
                }
                break;
            case CARD_DTL:
                tempdouble = read_dbl_field(&data, &status);
                if (status == READ_SUCCESS) mesh->dtl = tempdouble;
                if (DEBUG) printf("\nmesh->dtl = % 23.16e", mesh->dtl);
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

    for (imesh=0; imesh<*nummeshes; imesh++){
         mesh = &((*mesh_ptr)[imesh]);
         for (i=0; i<mesh->nboundaries; i++){
             mesh->boundary[i].bc_value = mesh->series[mesh->boundary[i].bc_series];
         }
    }



    /* Reading input file for taking in all the remaining data. */
    rewind(infile);
    imesh = -1;
    mesh = NULL;
    iscommon = 0;
    while (iscommon!=1){ /* No need to check till EOF since we took care of that in the beginning of the file. */
        fgets(line, MAXLINE, infile);
        data = line;
        if (parse_card(&data)==CARD_COMMON && parse_card(&data)==CARD_START) iscommon++;
    }
    while (iscommon!=2){ /* No need to check till EOF since we took care of that in the beginning of the file. */
        fgets(line, MAXLINE, infile);
        data = line;
        switch (parse_card(&data)){
            case CARD_TRN:
                tempint = read_int_field(&data, &status);
                if (tempint!=0 && tempint !=1) throw_error("Only 0 or 1 allowed for transport card TRN");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].trn = tempint;
                break;
            case CARD_TEM:
                tempint = read_int_field(&data, &status);
                if (tempint!=0 && tempint !=1) throw_error("Only 0 or 1 allowed for timestepping card TEM");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].tem = tempint;
                break;
            case CARD_TPG:
                tempdouble = read_int_field(&data, &status);
                if (tempdouble<0.0 || tempdouble>0.5) throw_error("Value between 0 and 1 allowed for Petrov Galerkin card TPG");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].tpg = tempdouble;
                break;
            case CARD_NTL:
                tempdouble = read_dbl_field(&data, &status);
                if (tempdouble<=0. || tempdouble>1.) throw_error("0 < Tolerance (NTL) < 1.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].ntl = tempdouble;
                break;
            case CARD_ITL:
                tempdouble = read_dbl_field(&data, &status);
                if (tempdouble<=0. || tempdouble>1.) throw_error("0 < Tolerance (ITL) < 1.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].itl = tempdouble;
                break;
            case CARD_NIT:
                tempint = read_int_field(&data, &status);
                if (tempint<1) throw_error("Number of nonlinear iterations, integer NIT >= 1 required.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].nit = tempint;
                break;
            case CARD_MIT:
                tempint = read_int_field(&data, &status);
                if (tempint<1) throw_error("Number of solver iterations, integer MIT >= 1 required.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].mit = tempint;
                break;
            case CARD_TSTART:
                tempdouble = read_dbl_field(&data, &status);
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].t0 = tempdouble;
                break;
            case CARD_TEND:
                tempdouble = read_dbl_field(&data, &status);
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].tf = tempdouble;
                break;
            case CARD_DT:
                tempdouble = read_dbl_field(&data, &status);
                if (tempdouble<=0.0) throw_error("Time step > 0 required.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].dt = tempdouble;
                break;
            case CARD_AWRITE:
                tempdouble = read_dbl_field(&data, &status);
                if (tempdouble<=0.0) throw_error("Result-writing interval > 0 required.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].awrite = tempdouble;
                break;
            case CARD_MNG:
                tempdouble = read_dbl_field(&data, &status);
                if (tempdouble<0.0) throw_error("Manning's Friction (MNG) >= 0  required.");
                if (status==READ_SUCCESS)
                    for (imesh=0; imesh<*nummeshes; imesh++) (*mesh_ptr)[imesh].mng = tempdouble;
                break;
            case CARD_COMMON:
                if (parse_card(&data)==CARD_END) iscommon++;
                break;
                default:
                break;
        }
    }

    fclose(infile);
}
