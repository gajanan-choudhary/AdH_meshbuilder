#include "global_header.h"


static int DEBUG = OFF;

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
    if (numlength ==0){
#ifdef _DEBUG
        if (DEBUG) printf("\nError: No number encountered in the string");
        else throw_error("\nError: No number encountered in the string");
#else
        throw_error("\nError: No number encountered in the string");
#endif
        *status = READ_FAILURE;
        return NO;
    }
    
    /* If we're here, we can finally read the string! */
    outdouble = (double) strtod(*outstr, &i);
#ifdef _DEBUG
    if (DEBUG) printf("\n outstr : %s      i : %s", *outstr, i);
#endif
    *outstr = i;
    *status = READ_SUCCESS;
    return outdouble;
}
