#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_INT_LENGTH 9
#define infilename "infiledbl.inp"
#define MAXLINE 250
#define NO -3
#define READ_SUCCESS 1
#define READ_FAILURE 0

static int DEBUG = 1;

void remove_spaces(char **outstr){
    char *i = *outstr;
    while ((*i == ' ' || *i == '\t') && (*i!=0) && (*i!='\n')){
        i++;
    }
    if (*i!=0 /* && *i!='\n'*/) {
        *outstr = i;
    }
    else{
        *outstr = i-1;
    }
}

double read_dbl_field(char **outstr, int *status){
    double outdouble = NO;
    int pcount = 0, ecount=0;
    int numlength=0;
    char *i = *outstr;
    remove_spaces(&i);
    *outstr = i;
    if (*i=='-' || *i=='+') {
        i++;
    }
    if ((!*i) || *i==0 || *i=='\n' || *i==' ' || *i == '\t'){
        printf("\nError: No number encountered in the string");
        *status = READ_FAILURE;
        return NO;
    }
    while (*i!=' ' && *i!='\t' && *i!='\n' && *i!=0){
        if (*i>='0' && *i<='9'){
            numlength++;
        }
        else if (pcount<1 && *i=='.' && numlength>0){
            pcount++;
        }
        else if (ecount<1 && (*i=='e' || *i=='E') && numlength>0){
            ecount++;
        }
        else if (((*(i-1))=='E' || (*(i-1))=='e') && (*i=='-' || *i=='+')){
            /* Do nothing, this one's allowed. */
        }
        else{
            printf("\nError: Encountered a non-numeric character while reading the number.");
            *outstr = i;
            *status = READ_FAILURE;
            return NO;
        }
        i++;
    }
    if (numlength ==0){
        printf("\nError: No number encountered in the beginning of the string.");
    }
    
    /* If we're here, we can finally read the string! */
    outdouble = (double) strtod(*outstr, &i);
    if (DEBUG) printf("\n outstr : %s      i : %s", *outstr, i);
    *outstr = i;
    *status = READ_SUCCESS;
    return outdouble;
}

int main() {
    char line[MAXLINE];
    char *data;
    int status;
    double number;
    FILE *infile = fopen(infilename, "r");

    while (fgets(line, MAXLINE, infile)!=NULL){
        printf("\n**************************\nLine: %s", line);
        data = line;
        number = read_dbl_field(&data,&status);
        if (DEBUG){
            if (status == READ_SUCCESS){
                printf("\nSuccess! Double read: %30.15f",number);
                printf("\nRemaining string : %s",data);
            }
            else{
                printf("\nFailure!", line);
                printf("\nPossible failure location: %s",data);
            }
        }
    }    
    fclose(infile);
    return 0;
}
