#ifndef DIVIDER_H
#define DIVIDER_H

#include <stdbool.h>


typedef struct Dividers {
    int div_nr_units;
    int div_nr_reservation;
    int div_delay;
    bool* busy;
} Dividers;


Dividers* init_dividers();
void set_up_divider_units(Dividers* dividers);
void print_dividers(Dividers* dividers);
void free_dividers(Dividers* dividers);


#endif