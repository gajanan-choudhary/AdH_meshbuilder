#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_INT_LENGTH 9
#define infilename "infile.inp"
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
        *outstr = NULL;
    }
}

int read_int_field(char **outstr, int *status){
    int outint = NO;
    int numlength=0;
    char *i = *outstr;
    remove_spaces(&i);
    *outstr = i;
    if (*i=='-' || *i=='+') {
        i++;
    }
    if ((!*i) || *i==0 || *i=='\n'){
        printf("\nError: No number encountered in the string");
        *status = READ_FAILURE;
        return NO;
    }
    while (*i!=' ' && *i!='\t' && *i!='\n' && *i!=0){
        if (*i>='0' && *i<='9'){
            numlength++;
        }
        else{
            printf("\nError: Encountered a non-numeric character while reading the number.");
            *outstr = i;
            *status = READ_FAILURE;
            return NO;
        }
        i++;
    }
    if (numlength > MAX_INT_LENGTH){
        printf("\nError: Integer too long! only upto %i digits allowed.", MAX_INT_LENGTH);
        *outstr = i;
        *status = READ_FAILURE;
        return NO;
    }

    /* If we're here, we can finally read the string! */
    outint = (int) strtol(*outstr, &i, 10);
    if (DEBUG) printf("\n outstr : %s      i : %s", *outstr, i);
    *outstr = i;
    *status = READ_SUCCESS;
    return outint;
}

int main() {
    char line[MAXLINE];
    char *data;
    int status, integer;
    FILE *infile = fopen(infilename, "r");

    while (fgets(line, MAXLINE, infile)!=NULL){
        printf("\n**************************\nLine: %s", line);
        data = line;
        integer = read_int_field(&data,&status);
        if (DEBUG){
            if (status == READ_SUCCESS){
                printf("\nSuccess! Integer read: %d",integer);
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
