#include "shared_context.h"
#include "ui_callbacks.h"
#include "chainConfig.h"
#include "utils.h"
#include "feature_signTx.h"
#include "network.h"

uint8_t ppos_data[1024] = {};

bool node_id_to_string(const uint8_t *nodeID, char *out, uint32_t outLength){
  if(outLength <= 130) return false;
  memset(out, 0, outLength);
  out[0] = '0';
  out[1] = 'x';
  for(int i = 0; i < 64; ++i){
    uint8_t digit = (nodeID[i] >> 4) & 0x0f;
    out[2*(i+1)] = HEXDIGITS[digit];

    digit = nodeID[i]  & 0x0f;
    out[2*(i+1)+1] = HEXDIGITS[digit];
  }

  return true;
}


// // clang-format off
// UX_STEP_NOCB(
//     ux_confirm_create_staking_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "create staking",
//     });

// UX_STEP_NOCB(
//     ux_confirm_increase_staking_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "increase staking",
//     });

// UX_STEP_NOCB(
//     ux_confirm_withdrew_staking_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "withdrew staking",
//     });

// UX_STEP_NOCB(
//     ux_confirm_delegate_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "delegate",
//     });

// UX_STEP_NOCB(
//     ux_confirm_withdrew_delegate_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "withdrew delegate",
//     });

//   UX_STEP_NOCB(
//     ux_confirm_get_delegate_reward_step,
//     pnn,
//     {
//       &C_icon_eye,
//       "TxType",
//       "get delegate reward",
//     });

// UX_STEP_NOCB(
//     ux_confirm_nodeID_step,
//     bnnn_paging,
//     {
//       .title = "nodeID",
//       .text = strings.common.nodeID
//     });


// UX_STEP_NOCB(
//     ux_confirm_amount_step,
//     bnnn_paging,
//     {
//       .title = "Amount",
//       .text = strings.common.fullAmount
//     });


// UX_STEP_CB(
//     ux_confirm_approve_step,
//     pb,
//     io_seproxyhal_touch_data_ok(NULL),
//     {
//       &C_icon_validate_14,
//       "Approve",
//     });

// UX_STEP_CB(
//     ux_confirm_reject_step,
//     pb,
//     io_seproxyhal_touch_data_cancel(NULL),
//     {
//       &C_icon_crossmark,
//       "Reject",
//     });

// // clang-format on

// // create staking
// UX_FLOW(ux_confirm_create_staking_flow_,
//         &ux_confirm_create_staking_step,
//         &ux_confirm_nodeID_step,
//         &ux_confirm_amount_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_create_staking() {
//     ux_flow_init(0, ux_confirm_create_staking_flow_, NULL);
// }

// // increase staking
// UX_FLOW(ux_confirm_increase_staking_flow_,
//         &ux_confirm_increase_staking_step,
//         &ux_confirm_nodeID_step,
//         &ux_confirm_amount_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_increase_staking() {
//     ux_flow_init(0, ux_confirm_increase_staking_flow_, NULL);
// }

// // withdrew staking
// UX_FLOW(ux_confirm_withdrew_staking_flow_,
//         &ux_confirm_withdrew_staking_step,
//         &ux_confirm_nodeID_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_withdrew_staking() {
//     ux_flow_init(0, ux_confirm_withdrew_staking_flow_, NULL);
// }

// // delegate
// UX_FLOW(ux_confirm_delegate_flow_,
//         &ux_confirm_delegate_step,
//         &ux_confirm_nodeID_step,
//         &ux_confirm_amount_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_delegate() {
//     ux_flow_init(0, ux_confirm_delegate_flow_, NULL);
// }

// // withdrew delegate
// UX_FLOW(ux_confirm_withdrew_delegate_flow_,
//         &ux_confirm_withdrew_delegate_step,
//         &ux_confirm_nodeID_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_withdrew_delegate() {
//     ux_flow_init(0, ux_confirm_withdrew_delegate_flow_, NULL);
// }

// //get delegate reward
// UX_FLOW(ux_confirm_get_delegate_reward_flow_,
//         &ux_confirm_get_delegate_reward_step,
//         &ux_confirm_approve_step,
//         &ux_confirm_reject_step);

// void ux_confirm_get_delegate_reward() {
//     ux_flow_init(0, ux_confirm_get_delegate_reward_flow_, NULL);
// }