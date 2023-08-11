#ifndef USER_PROTOCOL_UPPM_FDK_FRAME_H_
#define USER_PROTOCOL_UPPM_FDK_FRAME_H_

#include <stdint.h>
#include "frame_defines.h"

#define UPPM_FDKFRAME_SIZE UPPM_FDKFRAME_CTRL_SIZE128 * WORD128_WORD32_SIZE * WORD32_BYTE_SIZE

// Fields value
#define UPPM_FDKFRAME_CTRL_OFFSET128 3
#define UPPM_FDKFRAME_CTRL_SIZE128 1

typedef struct
{
    //word1[32:24]
    uint8_t tempPpm1    :1;
    uint8_t tempPpm2    :1;
    uint8_t tempPpm3    :1;
    uint8_t tempPpm4    :1;
    uint8_t reserved1   :4;

    //word1[24:16]
    uint8_t statCh1    :1;
    uint8_t statCh2    :1;
    uint8_t statCh3    :1;
    uint8_t statCh4    :1;
    uint8_t reserved2   :4;

    //word1[15:0]
    uint8_t reserved3;

    uint8_t osPpm       :1;
    uint8_t reserved4   :7;

    //word2[32:0]
    uint16_t reserved5;
    uint8_t pwrZi;
    uint8_t pwrGet;

    //word3[32:0]
    uint8_t pwrK4;
    uint8_t pwrK3;
    uint8_t pwrK2;
    uint8_t pwrK1;

    //word4[32:16]
    uint16_t reserved6;

    //word4[15:8]
    uint8_t RKsM1   :1;
    uint8_t RKsM2   :1;
    uint8_t RKsM3   :1;
    uint8_t RKsM4   :1;
    uint8_t reserved7   :4;

    //word4[7:0]
    uint8_t SsM1   :1;
    uint8_t SsM2   :1;
    uint8_t SsM3   :1;
    uint8_t SsM4   :1;
    uint8_t reserved8   :4;
}UPPM_FDK_Frame_struct;

typedef union
{
    UPPM_FDK_Frame_struct structData;
    uint8_t rawData[UPPM_FDKFRAME_SIZE];
}UPPM_FDK_Frame;

#endif /* USER_PROTOCOL_UPPM_FDK_FRAME_H_ */
