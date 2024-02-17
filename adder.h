#ifndef ADDER_H
#define ADDER_H

#include <stdbool.h>


typedef struct Adders {
    int add_nr_units;
    int add_nr_reservation;
    int add_delay;
    bool* busy;
} Adders;


Adders* init_adders();
void set_up_adder_units(Adders* adders);
void print_adders(Adders* adders);
void free_adders(Adders* adders);


#endif