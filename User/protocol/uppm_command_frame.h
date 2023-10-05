#ifndef USER_PROTOCOL_UPPM_COMMAND_FRAME_H_
#define USER_PROTOCOL_UPPM_COMMAND_FRAME_H_

#include <stdint.h>
#include "frame_defines.h"

#define UPPM_BUFFER_SIZE_LW_POS FRAME_HEADER_SIZE + 28*WORD32_BYTE_SIZE + LOW_WORD_OFFSET//131 - low 8 bits of 28 word32
#define UPPM_QUEUE_SIZE_LW_POS FRAME_HEADER_SIZE + 30*WORD32_BYTE_SIZE + LOW_WORD_OFFSET //139

// Fields value
#define UPPM_ACKFRAME_CTRL_OFFSET128 3
#define UPPM_ACKFRAME_CTRL_SIZE128 3

#define UPPM_ACKFRAME_SYNC_OFFSET128 6
#define UPPM_ACKFRAME_SYNC_SIZE128 1

#define UPPM_ACKFRAME_AUX_DATA_OFFSET128 7
#define UPPM_ACKFRAME_AUX_DATA_SIZE128 1

#define UPPM_ACKFRAME_SIGNAL_OFFSET128 8
#define UPPM_ACKFRAME_SIGNAL_SIZE128 0

typedef struct
{
    // word 12, [31:24]
   uint8_t reserved1  :6;
   uint8_t SBR_OCH    :1;     //§³§¢§² §°§¹
   uint8_t REG        :1;     //§²§¦§¤

   // word 12, [23:16]
   uint8_t NKCH       :8;     //§¯§¬§¹

   // word 12, [15:8]
   uint8_t NPO        :4;     //§¯§±§°
   uint8_t reserved2  :4;

   // word 12, [7:0]
   uint8_t KP         :8;     // §¬§±

   // word 13, [31:24]
   uint8_t reserved3  :8;

   // word 13, [23:16]
   uint8_t ZI         :4;     //§©§ª
   uint8_t reserved4  :4;

   // word 13, [15:8]
   uint8_t reserved5  :8;

   // word 13, [7:0]
   uint8_t PS         :1;     //§±§³
   uint8_t CDO        :1;     //§¸§¥§°
   uint8_t PBL        :1;     //§±§¢§­
   uint8_t PF         :1;     //§±§¶
   uint8_t AKP        :1;     //§¡§¬§±
   uint8_t EKV        :1;     //§¿§¬§£
   uint8_t reserved6  :2;

   //word 14, [32:0]
   uint32_t index; // §ª§ß§Õ§Ö§Ü§ã

   //word 15, [32:0]
   uint32_t TVRS;  // §´§Ó§â§ã

   //word 16, [32:8]
   uint8_t reserved7;
   uint8_t reserved8;
   uint8_t reserved9;

   //word 16, [7:0]
   uint8_t PRM      :1; //§±§²§®
   uint8_t FILTR    :2; //§¶§ª§­§¾§´§²
   uint8_t VUM      :1; //§£§µ§®
   uint8_t reserved10   :4;

   //word 17, [32:0]
   uint8_t KOEF4    :8;
   uint8_t KOEF3    :8;
   uint8_t KOEF2    :8;
   uint8_t KOEF1    :8;

   //word 18
   uint32_t DVRS; //§¥§£§²§³

   //word 19, [32:0]
   uint32_t TNO; //§´§¯§°

   //word 20, [32:0]
   uint32_t azimut; //§¡§Ù§Ú§Þ§å§ä

   //word 21, [32:0]
   uint32_t Tev_HIGH; //§´§Ö§Ó [64:32]

   //word 22, [32:0]
   uint32_t Tev_LOW; //§´§Ö§Ó [32:0]

   //word 23, [32:0]
   uint16_t KPK; //§¬§±§¬
   uint16_t Hpl; // §·§á§Ý

   //word 24, [32:0]
   uint32_t reserved11;

   //word 25, [32:0]
   uint32_t timestamp_hw;

   //word 26, [32:0]
   uint32_t timestamp_mw;

   //word 27, [32:0]
   uint32_t timestamp_lw;
}UPPM_Command_Frame;

#endif /* USER_PROTOCOL_UPPM_COMMAND_FRAME_H_ */
