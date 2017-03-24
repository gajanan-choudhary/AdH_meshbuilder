#include "global_header.h"

static int DEBUG = ON;

void remove_spaces(char *instr, char **outstr){
    char *i = instr;
    while ((*i == ' ' || *i == '\t') && *i != 0){
        i++;
    }
    *outstr = i;
}

unsigned int read_word(char *instr, char **outstr){
    char *i = instr;
    unsigned int cardvalue = 0, pow26 = 1, value;
    remove_spaces(instr, &i);
    *outstr = i;
    while ((*i != ' ' && *i != '\t' && *i != '\n') && *i != 0){
        if ((*i >='A') && (*i <='z')) {
            value = toupper(*i)-'A';
#ifdef _DEBUG
            if (DEBUG) printf("\nAlphabet %c, Value = %i - %i = %i", toupper(*i), toupper(*i), 'A', (toupper(*i)-'A'));
#endif
            cardvalue+=value*pow26;
            pow26 *= 26;
        }
        else {
           cardvalue=NO;
           break;
        }
        i++;
    }
    remove_spaces(i, outstr);
    return cardvalue;
}

void read_mesh_data(MESH **mesh, int *nummeshes, char *infilename) {
    char line[MAXLINE];
    char *data;
    char *subdata;
    unsigned int cardvalue = OFF;

    // char infilename[MAXLINE];
    // sprintf(infilename, "%s", filename);
    FILE *infile = fopen(filename, "r");
    while (fgets(line, MAXLINE, infile)!=NULL){
        cardvalue = read_word(data, &subdata);
        printf("\n\n Card Value = %10u\n", cardvalue);
        switch (cardvalue){
            case OTW:
                break;
            case NO:
#ifdef _DEBUG
                if (DEBUG) printf("\nNo card remaining to be read in this line!");
#endif
                continue;
                break;
           default:
                break;
        }
        if (cardvalue==(unsigned int) NO){
        }
        data = subdata;
    }
    fclose(infile);
}
