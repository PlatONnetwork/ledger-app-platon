#include "ppos.h"
#include "shared_context.h"

ppos_data_t ppos_data;

uint64_t get_length(uint8_t **data_ptr){
    uint8_t *data = *data_ptr;
    uint64_t result = 0;
    if(*data > 0x80 && *data <= 0xb7){
        result = *data - 0x80;
        data++;
        *data_ptr = data;
    }else if(*data > 0xb7 && *data <= 0xc0){
        uint8_t data_length = *data - 0xb7;
        ++data;
        for(int i = 0; i < data_length; ++i, ++data){
            result <<= 8;
            result += *data;
        }
        *data_ptr = data;
    }else if( *data > 0xc0 && *data <= 0xf7){
        result = *data - 0xc0;
        data++;
        *data_ptr = data;
    }else if(*data > 0xf7){
        uint8_t data_length = *data - 0xf7;
        ++data;
        for(int i = 0; i < data_length; ++i, ++data){
            result <<= 8;
            result += *data;
        }
        *data_ptr = data;
    } else {
        result = 1;
    }

    return result;
}

uint16_t get_func_type(const uint8_t *data, uint32_t length){
    uint16_t result = 0;
    switch(length){
        case 1:
            result = *data;
            break;
        case 2:
            result = *data;
            result <<= 8;
            ++data;
            result += *data;
            break;
        default:
            result = 0;
    }

    return result;
}

void get_amount(uint8_t *data, uint32_t length){
    amountToString(data, length, 18, "Lat", strings.common.pposAmount, sizeof(strings.common.pposAmount));
}

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