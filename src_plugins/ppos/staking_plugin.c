#include <string.h>
#include "lat_plugin_internal.h"
#include "lat_plugin_handler.h"
#include "shared_context.h"
#include "latUtils.h"
#include "utils.h"
#include "ppos.h"

#define CREATESTAKING 1000
#define CREATESTAKINGPARANUM 14
#define INCREASESTAKING 1002
#define INCREASESTAKINGPARANUM 4
#define WITHDREWSTAKING 1003
#define WITHDREWSTAKINGPARANUM 2
#define DELEGATE 1004
#define DELEGATEPARANUM 4
#define WITHDREWDELEGATE 1005
#define WITHDREWDELEGATEPARANUM 4

void create_staking(uint8_t *data){
    int node_id_step = 3;
    uint8_t node_id[64] = {};
    int amount_step = 8;

    for(int i = 1; i < CREATESTAKINGPARANUM; ++i){
        get_length(&data);
        uint64_t data_length = get_length(&data);
        if(node_id_step == i){
            memcpy(node_id, data, data_length);
        }
        if(amount_step == i){
            get_amount(data, data_length);
        }
        data += data_length;
    }

    node_id_to_string(node_id, strings.common.nodeID, sizeof(strings.common.nodeID));
    strcpy(strings.common.ppos_type, "Staking");
    strings.common.display_content = DISPLAYNODEID | DISPLAYAMOUNT;
}

void increase_staking(uint8_t *data){
    int node_id_step = 1;
    uint8_t node_id[64] = {};
    int amount_step = 3;

    for(int i = 1; i < INCREASESTAKINGPARANUM; ++i){
        get_length(&data);
        uint64_t data_length = get_length(&data);
        if(node_id_step == i){
            memcpy(node_id, data, data_length);
        }
        if(amount_step == i){
            get_amount(data, data_length);
        }
        data += data_length;
    }

    node_id_to_string(node_id, strings.common.nodeID, sizeof(strings.common.nodeID));
    strcpy(strings.common.ppos_type, "Add Staking");
    strings.common.display_content = DISPLAYNODEID | DISPLAYAMOUNT;
}

void withdrew_staking(uint8_t *data){
    int node_id_step = 1;
    uint8_t node_id[64] = {};

    for(int i = 1; i < WITHDREWSTAKINGPARANUM; ++i){
        get_length(&data);
        uint64_t data_length = get_length(&data);
        if(node_id_step == i){
            memcpy(node_id, data, data_length);
        }
        data += data_length;
    }

    node_id_to_string(node_id, strings.common.nodeID, sizeof(strings.common.nodeID));
    strcpy(strings.common.ppos_type, "Unstaking");
    strings.common.display_content = DISPLAYNODEID;
}

void delegate(uint8_t *data){
    int node_id_step = 2;
    uint8_t node_id[64] = {};
    int amount_step = 3;

    for(int i = 1; i < DELEGATEPARANUM; ++i){
        get_length(&data);
        uint64_t data_length = get_length(&data);
        if(node_id_step == i){
            memcpy(node_id, data, data_length);
        }
        if(amount_step == i){
            get_amount(data, data_length);
        }
        data += data_length;
    }

    node_id_to_string(node_id, strings.common.nodeID, sizeof(strings.common.nodeID));
    strcpy(strings.common.ppos_type, "Delegation");
    strings.common.display_content = DISPLAYNODEID | DISPLAYAMOUNT;
}

void withdrew_delegate(uint8_t *data){
    int node_id_step = 2;
    uint8_t node_id[64] = {};
    int amount_step = 3;

    for(int i = 1; i < WITHDREWDELEGATEPARANUM; ++i){
        get_length(&data);
        uint64_t data_length = get_length(&data);
        if(node_id_step == i){
            memcpy(node_id, data, data_length);
        }
        if(amount_step == i){
            get_amount(data, data_length);
        }
        data += data_length;
    } 

    node_id_to_string(node_id, strings.common.nodeID, sizeof(strings.common.nodeID));
    strcpy(strings.common.ppos_type, "UnDelegation");
    strings.common.display_content = DISPLAYNODEID | DISPLAYAMOUNT;
}

static void parse_staking_info(){
    uint8_t *data = ppos_data.begin;
    if(0 == get_length(&data)) return;
    if(0 == get_length(&data)) return;
    uint64_t data_length = get_length(&data);
    uint16_t func_type = get_func_type(data, data_length);

    data += data_length;
    strings.common.bPpos = true;

    switch(func_type){
        case CREATESTAKING:
            create_staking(data);
            break;
        case INCREASESTAKING:
            increase_staking(data);
            break;
        case WITHDREWSTAKING:
            withdrew_staking(data);
            break;
        case DELEGATE:
            delegate(data);
            break;
        case WITHDREWDELEGATE:
            withdrew_delegate(data);
            break;
        default:
            return;
    }
}

void staking_plugin_call(int message, void *parameters) {
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
            parse_staking_info();
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