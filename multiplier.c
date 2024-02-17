#include "multiplier.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


Multipliers* init_multipliers(){
    Multipliers* multipliers = (Multipliers*) calloc (1, sizeof(Multipliers));
    assert(multipliers);
    return multipliers;
}


void set_up_multiplier_units(Multipliers* multipliers){
    multipliers->busy = (bool*) calloc (multipliers->mul_nr_units, sizeof(bool));
    assert(multipliers->busy);
    for (int i=0; i<multipliers->mul_nr_units; i++){
        multipliers->busy[i] = false;
    }
}


void print_multipliers(Multipliers* multipliers){
    for (int i=0; i<multipliers->mul_nr_units; i++){
        printf("busy[%d] = %s\n", i, multipliers->busy[i]?"True":"False");
    }
    printf("mul_nr_units = %d\n", multipliers->mul_nr_units);
    printf("mul_nr_reservation = %d\n", multipliers->mul_nr_reservation);
    printf("mul_delay = %d\n", multipliers->mul_delay);
}


void free_multipliers(Multipliers* multipliers){
    free(multipliers->busy);
    multipliers->busy = NULL;
    free(multipliers);
    multipliers = NULL;
}

