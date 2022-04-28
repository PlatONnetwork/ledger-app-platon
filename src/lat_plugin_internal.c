#include "lat_plugin_internal.h"

bool prc20_plugin_available_check(void);
bool prc721_plugin_available_check(void);

void prc20_plugin_call(int message, void* parameters);
void prc721_plugin_call(int message, void* parameters);

static const uint8_t PRC20_TRANSFER_SELECTOR[SELECTOR_SIZE] = {0xa9, 0x05, 0x9c, 0xbb};
static const uint8_t PRC20_APPROVE_SELECTOR[SELECTOR_SIZE] = {0x09, 0x5e, 0xa7, 0xb3};

const uint8_t STAKINGCONTRACTADDRESS[20] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x02};

const uint8_t REWARDCONTRACTADDRESS[20] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x06};

const uint8_t* const PRC20_SELECTORS[NUM_PRC20_SELECTORS] = {PRC20_TRANSFER_SELECTOR,
                                                             PRC20_APPROVE_SELECTOR};

static const uint8_t PRC721_TRANSFERFROM_SELECTOR[SELECTOR_SIZE] = {0x23, 0xb8, 0x72, 0xdd};

const uint8_t* const PRC721_SELECTORS[NUM_PRC721_SELECTORS] = {PRC721_TRANSFERFROM_SELECTOR};

// All internal alias names start with 'minus'

const internalLatPlugin_t INTERNAL_LAT_PLUGINS[] = {
    {prc20_plugin_available_check,
     (const uint8_t**) PRC20_SELECTORS,
     NUM_PRC20_SELECTORS,
     "prc20",
     prc20_plugin_call},

    {prc721_plugin_available_check,
     (const uint8_t**) PRC721_SELECTORS,
     NUM_PRC721_SELECTORS,
     "prc721",
     prc721_plugin_call},

    {NULL, NULL, 0, "", NULL}};
