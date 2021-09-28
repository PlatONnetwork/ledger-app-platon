#include "shared_context.h"
#include "apdu_constants.h"
#include "ui_flow.h"
#include "feature_signTx.h"

static void debug_write(char *buf)
{
  asm volatile (
     "movs r0, #0x04\n"
     "movs r1, %0\n"
     "svc      0xab\n"
     :: "r"(buf) : "r0", "r1"
  );
}

#define CHUNKSIZE 150
uint8_t all_tx_data[512];
uint16_t all_tx_data_offset;

void handleSign(uint8_t p1,
                uint8_t p2,
                uint8_t *workBuffer,
                uint16_t dataLength,
                unsigned int *flags,
                unsigned int *tx) {
    UNUSED(tx);
    parserStatus_e txResult;
    uint32_t i;

    char buffer[10] = {};
    snprintf(buffer, 7, "%d\n", dataLength);
    debug_write(buffer);

    if (p1 == P1_FIRST) {
        all_tx_data_offset = 0;
    }

    memmove(all_tx_data+all_tx_data_offset, workBuffer, dataLength);
    all_tx_data_offset += dataLength;

    if(CHUNKSIZE == dataLength) {
        G_io_apdu_buffer[(*tx)++] = 0x90;
        G_io_apdu_buffer[(*tx)++] = 0x00;
        return;
    }

    workBuffer = all_tx_data;
    dataLength = all_tx_data_offset;
    p1 = P1_FIRST;

    if (p1 == P1_FIRST) {
        if (dataLength < 1) {
            PRINTF("Invalid data\n");
            THROW(0x6a80);
        }
        if (appState != APP_STATE_IDLE) {
            reset_app_context();
        }
        appState = APP_STATE_SIGNING_TX;
        tmpCtx.transactionContext.pathLength = workBuffer[0];
        if ((tmpCtx.transactionContext.pathLength < 0x01) ||
            (tmpCtx.transactionContext.pathLength > MAX_BIP32_PATH)) {
            PRINTF("Invalid path\n");
            THROW(0x6a80);
        }
        workBuffer++;
        dataLength--;
        for (i = 0; i < tmpCtx.transactionContext.pathLength; i++) {
            if (dataLength < 4) {
                PRINTF("Invalid data\n");
                THROW(0x6a80);
            }
            tmpCtx.transactionContext.bip32Path[i] = U4BE(workBuffer, 0);
            workBuffer += 4;
            dataLength -= 4;
        }
        dataPresent = false;
        dataContext.tokenContext.pluginStatus = LAT_PLUGIN_RESULT_UNAVAILABLE;

        // EIP 2718: TransactionType might be present before the TransactionPayload.
        uint8_t txType = *workBuffer;
        if (txType >= MIN_TX_TYPE && txType <= MAX_TX_TYPE) {
            // Enumerate through all supported txTypes here...
            if (txType == EIP2930) {
                txContext.txType = txType;
                workBuffer++;
                dataLength--;
            } else {
                PRINTF("Transaction type not supported\n");
                THROW(0x6501);
            }
        } else {
            txContext.txType = LEGACY;
        }
        initTx(&txContext, &global_sha3, &tmpContent.txContent, customProcessor, NULL);
    } else if (p1 != P1_MORE) {
        THROW(0x6B00);
    }
    if (p2 != 0) {
        THROW(0x6B00);
    }
    if ((p1 == P1_MORE) && (appState != APP_STATE_SIGNING_TX)) {
        PRINTF("Signature not initialized\n");
        THROW(0x6985);
    }
    if (txContext.currentField == RLP_NONE) {
        PRINTF("Parser not initialized\n");
        THROW(0x6985);
    }
    debug_write("processTx\n");
    txResult = processTx(&txContext,
                         workBuffer,
                         dataLength,
                         0);
    switch (txResult) {
        case USTREAM_SUSPENDED:
            break;
        case USTREAM_FINISHED:
            break;
        case USTREAM_PROCESSING:
            THROW(0x9000);
        case USTREAM_FAULT:
            THROW(0x6A80);
        default:
            PRINTF("Unexpected parser status\n");
            THROW(0x6A80);
    }

    debug_write("finalizeParsing\n");
    if (txResult == USTREAM_FINISHED) {
        finalizeParsing(false);
    }

    *flags |= IO_ASYNCH_REPLY;

    debug_write("handleSign end\n");
}
