#ifndef _UI_FLOW_PPOS_H_
#define _UI_FLOW_PPOS_H_

#include <stdint.h>
#include "uint256.h"

extern uint8_t ppos_data[1024];

bool node_id_to_string(const uint8_t *nodeID, char *out, uint32_t outLength);

void ux_confirm_create_staking();
void ux_confirm_increase_staking();
void ux_confirm_withdrew_staking();
void ux_confirm_delegate();
void ux_confirm_withdrew_delegate();
void ux_confirm_get_delegate_reward();

#endif