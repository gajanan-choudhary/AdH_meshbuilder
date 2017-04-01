#include "global_header.h"

//static int DEBUG = OFF;

void printf_elem1d(ELEM1D *elem1d, int id){
    printf("\n********************************************************************");
    printf("\nElement        : %i", id+1);
    printf("\nLength         :% 22.15e", elem1d[id].length);
    printf("\nNode 1         : %i", elem1d[id].nodes[0]+1);
    printf(" (% 22.15e,% 22.15e)", elem1d[id].coord[0].x, elem1d[id].coord[0].y);
    printf("\nNode 2         : %i", elem1d[id].nodes[1]+1);
    printf(" (% 22.15e,% 22.15e)", elem1d[id].coord[1].x, elem1d[id].coord[1].y);
    printf("\nEdge string ID : %i", elem1d[id].str);
    printf("\nType           : ");
    switch (elem1d[id].type){
        case FLOW:
            printf("Flow");
            printf("\nBC Series ID   : %i", elem1d[id].bc_series);
            printf("\nBC Value       :% 22.15e", elem1d[id].bc_value);
        break;
        case DISCHARGE:
            printf("Discharge");
            printf("\nBC Series ID   : %i", elem1d[id].bc_series);
            printf("\nBC Value       :% 22.15e", elem1d[id].bc_value);
        break;
        case TAILWATER:
            printf("Tail water");
            printf("\nBC Series ID   : %i", elem1d[id].bc_series);
            printf("\nBC Value       :% 22.15e", elem1d[id].bc_value);
        break;
        case STRONG_INTERFACE:
            printf("Strongly coupled interface");
        break;
        case WEAK_INTERFACE:
            printf("Weakly coupled interface");
        break;
    }
}
