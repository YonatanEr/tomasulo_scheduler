#include "adder.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


Adders* init_adders(){
    Adders* adders = (Adders*) calloc (1, sizeof(Adders));
    assert(adders);
    return adders;
}


void set_up_adder_units(Adders* adders){
    adders->busy = (bool*) calloc (adders->add_nr_units, sizeof(bool));
    assert(adders->busy);
    for (int i=0; i<adders->add_nr_units; i++){
        adders->busy[i] = false;
    }
}


void print_adders(Adders* adders){
    for (int i=0; i<adders->add_nr_units; i++){
        printf("busy[%d] = %s\n", i, adders->busy[i]?"True":"False");
    }
    printf("add_nr_units = %d\n", adders->add_nr_units);
    printf("add_nr_reservation = %d\n", adders->add_nr_reservation);
    printf("add_delay = %d\n", adders->add_delay);
}


void free_adders(Adders* adders){
    free(adders->busy);
    adders->busy = NULL;
    free(adders);
    adders = NULL;
}

