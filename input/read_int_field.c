#include "global_header.h"

#define MAX_INT_LENGTH  9
static int DEBUG = OFF;

int read_int_field(char **outstr, int *status){
    int outint = NO;
    int numlength=0;
    char *i = *outstr;
    remove_spaces(&i);
    *outstr = i;
    if (*i=='-' || *i=='+') {
        i++;
    }
    if ((!*i) || *i==0 || *i=='\n' || *i==' ' || *i == '\t'){
#ifdef _DEBUG
        if (DEBUG) printf("\nError: No number encountered in the string");
        else throw_error("\nError: No number encountered in the string");
#else
        throw_error("\nError: No number encountered in the string");
#endif
        *status = READ_FAILURE;
        return NO;
    }
    while (*i!=' ' && *i!='\t' && *i!='\n' && *i!=0){
        if (*i>='0' && *i<='9'){
            numlength++;
        }
        else{
#ifdef _DEBUG
            if(DEBUG) printf("\nError: Encountered a non-numeric character while reading the number.");
            else throw_error("\nError: Encountered a non-numeric character while reading the number.");
#else
            throw_error("\nError: Encountered a non-numeric character while reading the number.");
#endif
            *outstr = i;
            *status = READ_FAILURE;
            return NO;
        }
        i++;
    }
    if (numlength > MAX_INT_LENGTH){
#ifdef _DEBUG
        if (DEBUG) printf("\nError: Integer too long. Only upto %i digits allowed.", MAX_INT_LENGTH);
        else throw_error("\nError: Integer too long.");
#else
        throw_error("\nError: Integer too long.");
#endif
        *outstr = i;
        *status = READ_FAILURE;
        return NO;
    }

    /* If we're here, we can finally read the string! */
    outint = (int) strtol(*outstr, &i, 10);
#ifdef _DEBUG
    if (DEBUG) printf("\n outstr : %s      i : %s", *outstr, i);
#endif
    *outstr = i;
    *status = READ_SUCCESS;
    return outint;
}
