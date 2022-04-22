#include <string.h>
#include "lat_plugin_internal.h"
#include "lat_plugin_handler.h"
#include "shared_context.h"
#include "latUtils.h"
#include "latUstream.h"
#include "utils.h"

typedef enum { PRC20_TRANSFER = 0, PRC20_APPROVE } prc20Selector_t;

typedef enum { TARGET_ADDRESS = 0, TARGET_CONTRACT } targetType_t;

#define MAX_CONTRACT_NAME_LEN 15

typedef struct prc20_parameters_t {
    uint8_t selectorIndex;
    uint8_t destinationAddress[21];
    uint8_t amount[INT256_LENGTH];
    char ticker[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t target;
    char contract_name[MAX_CONTRACT_NAME_LEN];
} prc20_parameters_t;

typedef struct contract_t {
    char name[MAX_CONTRACT_NAME_LEN];
    uint8_t address[ADDRESS_LENGTH];
} contract_t;

#define NUM_CONTRACTS 11
const contract_t CONTRACTS[NUM_CONTRACTS] = {};

bool check_contract(prc20_parameters_t *context) {
    for (size_t i = 0; i < NUM_CONTRACTS; i++) {
        contract_t *contract = (contract_t *) PIC(&CONTRACTS[i]);
        if (memcmp(contract->address, context->destinationAddress, ADDRESS_LENGTH) == 0) {
            strncpy((char *) context->contract_name,
                    contract->name,
                    sizeof(context->contract_name));
            return true;
        }
    }
    return false;
}

bool prc20_plugin_available_check() {
    return true;
}

void prc20_plugin_call(int message, void *parameters) {
    switch (message) {
        case LAT_PLUGIN_INIT_CONTRACT: {
            latPluginInitContract_t *msg = (latPluginInitContract_t *) parameters;
            prc20_parameters_t *context = (prc20_parameters_t *) msg->pluginContext;
            // enforce that LAT amount should be 0
            if (!allzeroes(msg->pluginSharedRO->txContent->value.value, 32)) {
                PRINTF("Err: Transaction amount is not 0\n");
                msg->result = LAT_PLUGIN_RESULT_ERROR;
            } else {
                size_t i;
                for (i = 0; i < NUM_PRC20_SELECTORS; i++) {
                    if (memcmp((uint8_t *) PIC(PRC20_SELECTORS[i]), msg->selector, SELECTOR_SIZE) ==
                        0) {
                        context->selectorIndex = i;
                        break;
                    }
                }
                if (i == NUM_PRC20_SELECTORS) {
                    PRINTF("Unknown selector %.*H\n", SELECTOR_SIZE, msg->selector);
                    msg->result = LAT_PLUGIN_RESULT_ERROR;
                    break;
                }
                PRINTF("prc20 plugin init\n");
                msg->result = LAT_PLUGIN_RESULT_OK;
            }
        } break;

        case LAT_PLUGIN_PROVIDE_PARAMETER: {
            latPluginProvideParameter_t *msg = (latPluginProvideParameter_t *) parameters;
            prc20_parameters_t *context = (prc20_parameters_t *) msg->pluginContext;
            PRINTF("prc20 plugin provide parameter %d %.*H\n",
                   msg->parameterOffset,
                   32,
                   msg->parameter);
            switch (msg->parameterOffset) {
                case 4:
                    memmove(context->destinationAddress, msg->parameter + 12, 20);
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;
                case 4 + 32:
                    memmove(context->amount, msg->parameter, 32);
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;
                default:
                    PRINTF("Unhandled parameter offset\n");
                    msg->result = LAT_PLUGIN_RESULT_ERROR;
                    break;
            }
        } break;

        case LAT_PLUGIN_FINALIZE: {
            latPluginFinalize_t *msg = (latPluginFinalize_t *) parameters;
            prc20_parameters_t *context = (prc20_parameters_t *) msg->pluginContext;
            PRINTF("prc20 plugin finalize\n");
            if (context->selectorIndex == PRC20_TRANSFER) {
                msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
                msg->amount = context->amount;
                msg->address = context->destinationAddress;
                msg->uiType = LAT_UI_TYPE_AMOUNT_ADDRESS;
                msg->result = LAT_PLUGIN_RESULT_OK;
            } else if (context->selectorIndex == PRC20_APPROVE) {
                msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
                msg->numScreens = 2;
                msg->uiType = LAT_UI_TYPE_GENERIC;
                msg->result = LAT_PLUGIN_RESULT_OK;
            }
        } break;

        case LAT_PLUGIN_PROVIDE_TOKEN: {
            latPluginProvideToken_t *msg = (latPluginProvideToken_t *) parameters;
            prc20_parameters_t *context = (prc20_parameters_t *) msg->pluginContext;
            PRINTF("prc20 plugin provide token 1: %d - 2: %d\n",
                   (msg->token1 != NULL),
                   (msg->token2 != NULL));
            if (msg->token1 != NULL) {
                context->target = TARGET_ADDRESS;
                strlcpy(context->ticker, msg->token1->ticker, MAX_TICKER_LEN);
                context->decimals = msg->token1->decimals;
                if (context->selectorIndex == PRC20_APPROVE) {
                    if (check_contract(context)) {
                        context->target = TARGET_CONTRACT;
                    }
                }
                msg->result = LAT_PLUGIN_RESULT_OK;
            } else {
                msg->result = LAT_PLUGIN_RESULT_FALLBACK;
            }
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_ID: {
            latQueryContractID_t *msg = (latQueryContractID_t *) parameters;
            strlcpy(msg->name, "Type", msg->nameLength);
            strlcpy(msg->version, "Approve", msg->versionLength);
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_UI: {
            latQueryContractUI_t *msg = (latQueryContractUI_t *) parameters;
            prc20_parameters_t *context = (prc20_parameters_t *) msg->pluginContext;
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Amount", msg->titleLength);
                    if (ismaxint(context->amount, sizeof(context->amount))) {
                        strlcpy(msg->msg, "Unlimited ", msg->msgLength);
                        strlcat(msg->msg, context->ticker, msg->msgLength);
                    } else {
                        amountToString(context->amount,
                                       sizeof(context->amount),
                                       context->decimals,
                                       (char *) context->ticker,
                                       msg->msg,
                                       100);
                    }
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;
                case 1:
                    if (context->target >= TARGET_CONTRACT) {
                        strlcpy(msg->title, "Contract", msg->titleLength);
                        strlcpy(msg->msg, context->contract_name, msg->msgLength);
                    } else {
                        strlcpy(msg->title, "Address", msg->titleLength);
                        getLatAddressStringFromBinary(context->destinationAddress,
                                                      (uint8_t *) msg->msg,
                                                      msg->pluginSharedRW->sha3,
                                                      chainConfig);
                    }

                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;
                default:
                    break;
            }
        } break;

        default:
            PRINTF("Unhandled message %d\n", message);
    }
}
