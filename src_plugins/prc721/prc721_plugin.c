#include <string.h>
#include "lat_plugin_internal.h"
#include "lat_plugin_handler.h"
#include "shared_context.h"
#include "latUtils.h"
#include "utils.h"

typedef enum { PRC721_TRANSFERFROM = 0 } prc721Selector_t;

typedef struct prc721_parameters_t {
    uint8_t selectorIndex;
    uint8_t address[ADDRESS_LENGTH];
    uint8_t to_address[ADDRESS_LENGTH];
    uint8_t tokenId[INT256_LENGTH];
} prc721_parameters_t;

bool prc721_plugin_available_check() {
    return true;
}

void prc721_plugin_call(int message, void *parameters) {
    switch (message) {
        case LAT_PLUGIN_INIT_CONTRACT: {
            latPluginInitContract_t *msg = (latPluginInitContract_t *) parameters;
            prc721_parameters_t *context = (prc721_parameters_t *) msg->pluginContext;
            // enforce that LAT amount should be 0
            if (!allzeroes(msg->pluginSharedRO->txContent->value.value, 32)) {
                PRINTF("Err: Transaction amount is not 0 for prc721 approval\n");
                msg->result = LAT_PLUGIN_RESULT_ERROR;
            } else {
                size_t i;
                for (i = 0; i < NUM_PRC721_SELECTORS; i++) {
                    if (memcmp((uint8_t *) PIC(PRC721_SELECTORS[i]),
                               msg->selector,
                               SELECTOR_SIZE) == 0) {
                        context->selectorIndex = i;
                        break;
                    }
                }
                if (i == NUM_PRC721_SELECTORS) {
                    PRINTF("Unknown prc721 selector %.*H\n", SELECTOR_SIZE, msg->selector);
                    break;
                }
                if (msg->dataSize != 4 + 32 + 32 + 32) {
                    PRINTF("Invalid erc721 approval data size %d\n", msg->dataSize);
                    break;
                }
                PRINTF("prc721 plugin init\n");
                msg->result = LAT_PLUGIN_RESULT_OK;
            }
        } break;

        case LAT_PLUGIN_PROVIDE_PARAMETER: {
            latPluginProvideParameter_t *msg = (latPluginProvideParameter_t *) parameters;
            prc721_parameters_t *context = (prc721_parameters_t *) msg->pluginContext;
            PRINTF("prc721 plugin provide parameter %d %.*H\n",
                   msg->parameterOffset,
                   32,
                   msg->parameter);

            switch (msg->parameterOffset) {
            case 4:
                memmove(context->address, msg->parameter + 32 - 20, 20);
                msg->result = LAT_PLUGIN_RESULT_OK;
                break;
            case 4 + 32:
                memmove(context->to_address, msg->parameter + 32 - 20, 20);
                msg->result = LAT_PLUGIN_RESULT_OK;
                break;
            case 4 + 32 + 32:
                memmove(context->tokenId, msg->parameter, 32);
                msg->result = LAT_PLUGIN_RESULT_OK;
                break;
            default:
                PRINTF("Unhandled parameter offset\n");
                break;
            }

        } break;

        case LAT_PLUGIN_FINALIZE: {
            latPluginFinalize_t *msg = (latPluginFinalize_t *) parameters;
            prc721_parameters_t *context = (prc721_parameters_t *) msg->pluginContext;
            PRINTF("prc721 plugin finalize\n");
            msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
            msg->tokenLookup2 = context->address;
            msg->numScreens = 4;
            msg->uiType = LAT_UI_TYPE_GENERIC;
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_PROVIDE_TOKEN: {
            latPluginProvideToken_t *msg = (latPluginProvideToken_t *) parameters;
            PRINTF("prc721 plugin provide token dest: %d - address: %d\n",
                   (msg->token1 != NULL),
                   (msg->token2 != NULL));
            // context->tokenSelf = msg->token1;
            // context->tokenAddress = msg->token2;
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_ID: {
            latQueryContractID_t *msg = (latQueryContractID_t *) parameters;
            strlcpy(msg->name, "Type", msg->nameLength);
            strlcpy(msg->version, "prc721 transferFrom", msg->versionLength);
            msg->result = LAT_PLUGIN_RESULT_OK;
        } break;

        case LAT_PLUGIN_QUERY_CONTRACT_UI: {
            latQueryContractUI_t *msg = (latQueryContractUI_t *) parameters;
            prc721_parameters_t *context = (prc721_parameters_t *) msg->pluginContext;
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Contract Address", msg->titleLength);
                    getLatAddressStringFromBinary(tmpContent.txContent.destination,
                                             (uint8_t *)msg->msg,
                                             &global_sha3,
                                             chainConfig);
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;

                case 1:
                    strlcpy(msg->title, "From Address", msg->titleLength);
                    getLatAddressStringFromBinary(context->address,
                                             (uint8_t *)msg->msg,
                                             &global_sha3,
                                             chainConfig);
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;

                case 2:
                    strlcpy(msg->title, "To Address", msg->titleLength);
                    getLatAddressStringFromBinary(context->to_address,
                                             (uint8_t *)msg->msg,
                                             &global_sha3,
                                             chainConfig);
                    msg->result = LAT_PLUGIN_RESULT_OK;
                    break;

                case 3:
                    strlcpy(msg->title, "TokenID", msg->titleLength);
                    amountToString(context->tokenId, sizeof(context->tokenId),
                                       0,
                                       "",
                                       msg->msg,
                                       100);
                    // snprintf(msg->msg, 70, "0x%.*H", 32, context->tokenId);
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
