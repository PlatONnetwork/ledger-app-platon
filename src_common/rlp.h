#ifndef _RLP_H_
#define _RLP_H_

#include <stdint.h>
#include "utils.h"

uint64_t get_length(uint8_t **data_ptr);

uint16_t get_func_type(const uint8_t *data, uint32_t length);

void get_amount(uint8_t *data, uint32_t length, uint256_t *result);

#endif