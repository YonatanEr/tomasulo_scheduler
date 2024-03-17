#ifndef CDB_STATE_H
#define CDB_STATE_H

#include <stdbool.h>
#include "tag.h"

typedef struct CdbState {
    bool cdb_used;
    float cdb_value;
    Tag res_sta_tag;
    int dst_reg;
    bool update_reg_file;
} CdbState;

#endif