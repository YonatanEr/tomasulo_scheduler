#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include <stdbool.h>


typedef struct Multipliers {
    int mul_nr_units;
    int mul_nr_reservation;
    int mul_delay;
    bool* busy;
} Multipliers;



Multipliers* init_multipliers();
void set_up_multiplier_units(Multipliers* multipliers);
void print_multipliers(Multipliers* multipliers);
void free_multipliers(Multipliers* multipliers);


#endif