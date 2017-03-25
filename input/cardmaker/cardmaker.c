#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 250
#define OFF 0
#define NO -3
#define infilename "cardmaker_input.inp"
#define outfilename "cards.h"
static int DEBUG = 0;

void remove_spaces(char *instr, char **outstr){
    char *i = instr;
    while ((*i == ' ' || *i == '\t') && *i != 0){
        i++;
    }
    *outstr = i;
}

unsigned int parse_card(char *instr, char **outstr){
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

int main(/*int argc, char **argv*/) {
    char line[MAXLINE];
    char *data;
    char *subdata;
    unsigned int cardvalue = OFF;
    int i, len;

    FILE *outfile = fopen(outfilename, "w");
    if (outfile == NULL){
        printf("\n\nCould not open output file %s for writing\n\n", outfilename);
        return -1;
    }
    fprintf(outfile,"#ifndef _CARDS_H\n#define _CARDS_H\n");
    fprintf(outfile,"\n// cardvalue = card[0] + card[1]*26 + card[2]*26^2 + ...");
    fprintf(outfile,"\n\ntypedef enum {");
    fprintf(outfile,"\n    // CARD_<name> = <cardvalue>");

    FILE *infile = fopen(infilename, "r");
    if (infile == NULL){
        printf("\n\nCould not open input file %s for reading\n\n", infilename);
        fclose(outfile);
        return -1;
    }
    while (fgets(line, MAXLINE, infile)!=NULL){
        data = line;
        cardvalue = OFF;
        cardvalue = parse_card(data, &subdata);
        remove_spaces(data,&subdata);
        len = strlen(subdata);

        if (DEBUG){
            printf("\nLine = %s", line);
            printf("Card Value = %14u", cardvalue);
            printf("\nString Length = %i", len);
        }

        if (cardvalue != NO){
            fprintf(outfile,",\n    CARD_");
            for (i=0; i<len-1; i++){
//                if (line[i]!=' ' && line[i]!='\t'){
                    fprintf(outfile,"%c",subdata[i]);
//                }
            }
            fprintf(outfile," = %14u", cardvalue);
        }
        else {
            fprintf(outfile,",\n    ");
            for (i=0; i<len-1; i++) fprintf(outfile,"%c",subdata[i]);
        }
    }

    fprintf(outfile,"\n} CARD;");
    fprintf(outfile,"\n\n#endif");
    fclose(infile);
    fclose(outfile);
    return 0;
}
