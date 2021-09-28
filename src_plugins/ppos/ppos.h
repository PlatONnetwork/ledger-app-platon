#ifndef _PPOS_H_
#define _PPOS_H_

#include <stdint.h>
typedef struct ppos_data_t {
    uint8_t *begin;
    uint8_t *end;
} ppos_data_t;

extern ppos_data_t ppos_data;

uint64_t get_length(uint8_t **data_ptr);

uint16_t get_func_type(const uint8_t *data, uint32_t length);

void get_amount(uint8_t *data, uint32_t length);

bool node_id_to_string(const uint8_t *nodeID, char *out, uint32_t outLength);

#endif