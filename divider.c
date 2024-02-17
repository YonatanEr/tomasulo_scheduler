#include "divider.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


Dividers* init_dividers(){
    Dividers* dividers = (Dividers*) calloc (1, sizeof(Dividers));
    assert(dividers);
    return dividers;
}


void set_up_divider_units(Dividers* dividers){
    dividers->busy = (bool*) calloc (dividers->div_nr_units, sizeof(bool));
    assert(dividers->busy);
    for (int i=0; i<dividers->div_nr_units; i++){
        dividers->busy[i] = false;
    }
}


void print_dividers(Dividers* dividers){
    printf("print_dividers\n");
    for (int i=0; i<dividers->div_nr_units; i++){
        printf("    busy[%d] = %s\n", i, dividers->busy[i]?"True":"False");
    }
    printf("    div_nr_units = %d\n", dividers->div_nr_units);
    printf("    div_nr_reservation = %d\n", dividers->div_nr_reservation);
    printf("    div_delay = %d\n", dividers->div_delay);
    printf("\n");
}


void free_dividers(Dividers* dividers){
    free(dividers->busy);
    dividers->busy = NULL;
    free(dividers);
    dividers = NULL;
}

