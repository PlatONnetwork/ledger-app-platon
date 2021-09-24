#ifndef _UI_FLOW_PPOS_H_
#define _UI_FLOW_PPOS_H_

#include <stdint.h>
#include "uint256.h"

typedef struct ppos_data_t {
    uint8_t *begin;
    uint8_t *end;
} ppos_data_t;

extern ppos_data_t ppos_data;

bool node_id_to_string(const uint8_t *nodeID, char *out, uint32_t outLength);

void ux_confirm_create_staking();
void ux_confirm_increase_staking();
void ux_confirm_withdrew_staking();
void ux_confirm_delegate();
void ux_confirm_withdrew_delegate();
void ux_confirm_get_delegate_reward();

#endif