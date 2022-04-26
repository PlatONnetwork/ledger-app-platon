#ifndef __SHARED_CONTEXT_H__

#define __SHARED_CONTEXT_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"
#include "os_io_seproxyhal.h"
#include "latUstream.h"
#include "latUtils.h"
#include "uint256.h"
#include "tokens.h"
#include "chainConfig.h"
#include "lat_plugin_interface.h"

#define MAX_BIP32_PATH 10

#define MAX_HRP_LENGTH 8

#define MAX_TOKEN 2

#define WEI_TO_ETHER 18

#define SELECTOR_LENGTH 4

#define N_storage (*(volatile internalStorage_t *) PIC(&N_storage_real))

typedef struct internalStorage_t {
    unsigned char dataAllowed;
    unsigned char contractDetails;
    unsigned char displayNonce;
    uint8_t initialized;
} internalStorage_t;

typedef struct tokenContext_t {
    char pluginName[PLUGIN_ID_LENGTH];
    uint8_t pluginStatus;

    uint8_t data[INT256_LENGTH];
    uint8_t fieldIndex;
    uint8_t fieldOffset;

    uint8_t pluginUiMaxItems;
    uint8_t pluginUiCurrentItem;
    uint8_t pluginUiState;

    union {
        struct {
            uint8_t contract_address[ADDRESS_LENGTH];
            uint8_t method_selector[SELECTOR_LENGTH];
        };
        uint8_t pluginContext[5 * INT256_LENGTH];
    };

} tokenContext_t;

typedef struct publicKeyContext_t {
    cx_ecfp_public_key_t publicKey;
    uint8_t address[45];
    uint8_t chainCode[INT256_LENGTH];
    bool getChaincode;
} publicKeyContext_t;

typedef struct transactionContext_t {
    uint8_t pathLength;
    uint32_t bip32Path[MAX_BIP32_PATH];
    uint8_t hash[INT256_LENGTH];
    tokenDefinition_t tokens[MAX_TOKEN];
    uint8_t tokenSet[MAX_TOKEN];
    uint8_t currentTokenIndex;
} transactionContext_t;

typedef struct messageSigningContext_t {
    uint8_t pathLength;
    uint32_t bip32Path[MAX_BIP32_PATH];
    uint8_t hash[INT256_LENGTH];
    uint32_t remainingLength;
} messageSigningContext_t;

typedef union {
    publicKeyContext_t publicKeyContext;
    transactionContext_t transactionContext;
    messageSigningContext_t messageSigningContext;
} tmpCtx_t;

typedef union {
    txContent_t txContent;
    cx_sha256_t sha2;
    char tmp[100];
} tmpContent_t;

typedef union {
    tokenContext_t tokenContext;
} dataContext_t;

typedef enum { APP_STATE_IDLE, APP_STATE_SIGNING_TX, APP_STATE_SIGNING_MESSAGE } app_state_t;

typedef enum {
    CONTRACT_NONE,
    CONTRACT_PRC20,
    CONTRACT_ALLOWANCE,
} contract_call_t;

#define NETWORK_NAME_MAX_SIZE 12
#define PPOS_TYPE_MAX_SIZE    15

typedef struct txStringProperties_t {
    char fullAddress[43];
    char fullAmount[90];
    char maxFee[50];
    char nonce[8];  // 10M tx per account ought to be enough for everybody
    char network_name[NETWORK_NAME_MAX_SIZE];
    bool bPpos;
    uint8_t display_content;
    char ppos_type[PPOS_TYPE_MAX_SIZE];
    char nodeID[132];
    char pposAmount[90];
} txStringProperties_t;

#define SHARED_CTX_FIELD_1_SIZE 100
#define SHARED_CTX_FIELD_2_SIZE 40

typedef struct strDataTmp_t {
    char tmp[SHARED_CTX_FIELD_1_SIZE];
    char tmp2[SHARED_CTX_FIELD_2_SIZE];
} strDataTmp_t;

typedef union {
    txStringProperties_t common;
    strDataTmp_t tmp;
} strings_t;

extern chain_config_t *chainConfig;

extern tmpCtx_t tmpCtx;
extern txContext_t txContext;
extern tmpContent_t tmpContent;
extern dataContext_t dataContext;
extern strings_t strings;
extern cx_sha3_t global_sha3;
extern const internalStorage_t N_storage_real;

extern bool called_from_swap;
extern bool dataPresent;
extern bool externalPluginIsSet;
extern uint8_t appState;

void reset_app_context(void);

#endif  // __SHARED_CONTEXT_H__
