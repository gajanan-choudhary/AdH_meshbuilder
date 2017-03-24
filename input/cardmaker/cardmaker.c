#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 250
#define OFF 0
#define NO -3
#define infilename "cardmaker_input.inp"
#define outfilename "cards.h"
static int DEBUG = 1;

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
        if (DEBUG) printf("\nLine = %s", line);
        cardvalue = parse_card(data, &subdata);
        if (DEBUG) printf("Card Value = %14u", cardvalue);
        fprintf(outfile,",\n    ");
        len = strlen(line);
        if (cardvalue != NO){
            if (DEBUG) printf("\nString Length = %i", len);
            for (i=0; i<len-1; i++) fprintf(outfile,"%c",line[i]);
            fprintf(outfile," = %14u", cardvalue);
        }
        else {
            for (i=0; i<len-1; i++) fprintf(outfile,"%c",line[i]);
        }
        data = subdata;
    }

    fprintf(outfile,"\n} CARD;");
    fprintf(outfile,"\n\n#endif");
    fclose(infile);
    fclose(outfile);
    return 0;
}
