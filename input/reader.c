#include "global_header.h"

static int DEBUG = OFF;

/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
// void remove_spaces(char *instr, char **outstr){
//     char *i = instr;
//     while ((*i == ' ' || *i == '\t') && *i != 0){
//         i++;
//     }
//     *outstr = i;
// }
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


/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
// CARD parse_card(char *instr, char **outstr){
//     char *i = instr;
//     unsigned int cardvalue = 0, pow26 = 1, value;
//     remove_spaces(instr, &i);
CARD parse_card(char **outstr){
    char *i = *outstr;
    unsigned int cardvalue = 0, pow26 = 1, value;
    remove_spaces(&i);
    *outstr = i;
#ifdef _DEBUG
    if (DEBUG) printf("\n**********************************************");
#endif
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
           cardvalue=CARD_NO;
           break;
        }
        i++;
    }
    if (cardvalue==0) cardvalue=CARD_NO;
    *outstr = i;
    //remove_spaces(&i);
    return (CARD) cardvalue;
}
