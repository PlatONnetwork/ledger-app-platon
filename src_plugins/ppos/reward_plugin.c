#include <string.h>
#include "lat_plugin_internal.h"
#include "lat_plugin_handler.h"
#include "shared_context.h"
#include "latUtils.h"
#include "utils.h"
#include "ppos.h"

#define GETDELEGATEREWARD 5000
#define GETDELEGATEREWARDPARANUM 4

void get_delegate_reward(uint8_t *data){
    strcpy(strings.common.ppos_type, "withdrew delegate");
}

static void parse_reward_info(){
    uint8_t *data = ppos_data.begin;
    if(0 == get_length(&data)) return;
    if(0 == get_length(&data)) return;
    uint64_t data_length = get_length(&data);
    uint16_t func_type = get_func_type(data, data_length);
    data += data_length;
    strings.common.bPpos = true;
    switch(func_type){
        case GETDELEGATEREWARD:
            get_delegate_reward(data);
            break;
        default:
            return;
    }
}

void reward_plugin_call(int message, void *parameters) {
    switch (message) {
        case LAT_PLUGIN_INIT_CONTRACT: {
            latPluginInitContract_t *msg = (latPluginInitContract_t *) parameters;
            // enforce that LAT amount should be 0
            if (!allzeroes(msg->pluginSharedRO->txContent->value.value, 32)) {
                PRINTF("Err: Transaction amount is not 0\n");
                msg->result = LAT_PLUGIN_RESULT_ERROR;
            } else {
                PRINTF("staking plugin init\n");
                ppos_data.begin =  msg->selector;
                ppos_data.end = msg->selector + msg->dataSize;
                msg->result = LAT_PLUGIN_RESULT_OK;
            }
        } break;

        case LAT_PLUGIN_PROVIDE_PARAMETER: {
            latPluginProvideParameter_t *msg = (latPluginProvideParameter_t *) parameters;
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_FINALIZE: {
            latPluginFinalize_t *msg = (latPluginFinalize_t *) parameters;
            parse_reward_info();
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_PROVIDE_TOKEN: {
            latPluginProvideToken_t *msg = (latPluginProvideToken_t *) parameters;
             msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_ID: {
            latQueryContractID_t *msg = (latQueryContractID_t *) parameters;
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_UI: {
            latQueryContractUI_t *msg = (latQueryContractUI_t *) parameters;
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        default:
            PRINTF("Unhandled message %d\n", message);
    }
}