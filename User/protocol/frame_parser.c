#include <string.h>

#include "frame_parser.h"
#include "command_fifo.h"

#include "veeprom.h"

#include "uppm_command_frame.h"
#include "uppm_fdk_frame.h"
#include "uppm_raw_comm_frame.h"

#include "uppm_control.h"

void mConvertEndians(UPPM_Command_Frame* comm);

void parseFrame(const uint8_t* inData, uint32_t inDataLen, uint8_t* outData, uint32_t* outDataLen)
{
    *outDataLen = 0;

    FrameHeader* recievedFrameHeader = (FrameHeader*)&(inData[0]);
    DatagramHeader* recievedDatagramHeader = (DatagramHeader*)&(inData[DATAGRAM_HEADER_POS]);

    switch(inData[HEADER_FRAME_TYPE_POS])
    {
        case UPPM_COMMAND_FRAME:
        {
            UPPM_Command_Frame *comand_ptr = (UPPM_Command_Frame *)&(inData[COMMAND_DATA_POS]);
            UPPM_Command_Frame recieved_command = *comand_ptr;

            mConvertEndians(&recieved_command);

            if(CommFIFO_PutData(recieved_command))
            {
                memcpy(outData, inData, inDataLen);

                outData[UPPM_BUFFER_SIZE_LW_POS] = COMMAND_FIFO_SIZE;
                outData[UPPM_QUEUE_SIZE_LW_POS] = CommFIFO_Count();

                *outDataLen = inDataLen + 16;
            }

            FrameHeader frameHeader;
            memset(frameHeader.rawData, 0, FRAME_HEADER_SIZE);

            frameHeader.structData.signature = __builtin_bswap16(FRAME_SIGNATURE);
            frameHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
            frameHeader.structData.TK = UPPM_ACK_FRAME;
            frameHeader.structData.RK = __builtin_bswap32(*outDataLen);
            frameHeader.structData.RF128 = __builtin_bswap16(*outDataLen/16);
            frameHeader.structData.PF = 1;
            frameHeader.structData.SCH = framesCounter;
            frameHeader.structData.NF = 0;

            DatagramHeader datagramHeader;
            memset(datagramHeader.rawData, 0, DATAGRAM_HEADER_SIZE);

            datagramHeader.structData.LAYOUT = 4;
            datagramHeader.structData.LAYOUT_SIZE128 = 3;
            datagramHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
            datagramHeader.structData.TK = UPPM_ACK_FRAME;
            datagramHeader.structData.RK = __builtin_bswap32(*outDataLen - FRAME_HEADER_SIZE);

            datagramHeader.structData.CTRL_OFFSET128 = __builtin_bswap32(UPPM_ACKFRAME_CTRL_OFFSET128);
            datagramHeader.structData.CTRL_SIZE128 = __builtin_bswap32(UPPM_ACKFRAME_CTRL_SIZE128);
            datagramHeader.structData.SYNC_OFFSET128 = __builtin_bswap32(UPPM_ACKFRAME_SYNC_OFFSET128);
            datagramHeader.structData.SYNC_SIZE128 = __builtin_bswap32(UPPM_ACKFRAME_SYNC_SIZE128);;
            datagramHeader.structData.HEAD_AUX_OFFSET128 = __builtin_bswap32(UPPM_ACKFRAME_AUX_DATA_OFFSET128);
            datagramHeader.structData.HEAD_AUX_SIZE128 = __builtin_bswap32(UPPM_ACKFRAME_AUX_DATA_SIZE128);
            datagramHeader.structData.SIGNAL_OFFSET128 = __builtin_bswap32(UPPM_ACKFRAME_SIGNAL_OFFSET128);
            datagramHeader.structData.SIGNAL_SIZE128 =  __builtin_bswap32(UPPM_ACKFRAME_SIGNAL_SIZE128);

            memcpy(&outData[0], frameHeader.rawData, FRAME_HEADER_SIZE);
            memcpy(&outData[FRAME_HEADER_SIZE], datagramHeader.rawData, DATAGRAM_HEADER_SIZE);
            break;
        }

        case UPPM_RAW_COMMAND_FRAME:
        {
            uint8_t* dataPayload = (uint8_t*)&inData[COMMAND_DATA_POS];
            uint8_t phaseShifterVal = dataPayload[3] & 0x3F;
            uint8_t filterVal = dataPayload[2] & 0x03;

            printf("Recieved raw command frame. Filter: %x, PhaseShifter: %x\r\n", filterVal, phaseShifterVal);

            UPPM_SetFilter(filterVal);
            UPPM_SetPhaseShifter(phaseShifterVal);

            memcpy(outData, inData, inDataLen);
            *outDataLen = inDataLen;

            FrameHeader frameHeader;
            memset(frameHeader.rawData, 0, FRAME_HEADER_SIZE);

            frameHeader.structData.signature = __builtin_bswap16(FRAME_SIGNATURE);
            frameHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
            frameHeader.structData.TK = UPPM_RAW_ACK_COMM_FRAME;
            frameHeader.structData.RK = __builtin_bswap32(*outDataLen);
            frameHeader.structData.RF128 = __builtin_bswap16(*outDataLen/16);
            frameHeader.structData.PF = 1;
            frameHeader.structData.SCH = framesCounter;
            frameHeader.structData.NF = 0;

            DatagramHeader datagramHeader;
            memset(datagramHeader.rawData, 0, DATAGRAM_HEADER_SIZE);

            datagramHeader.structData.LAYOUT = 4;
            datagramHeader.structData.LAYOUT_SIZE128 = 3;
            datagramHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
            datagramHeader.structData.TK = UPPM_RAW_ACK_COMM_FRAME;
            datagramHeader.structData.RK = __builtin_bswap32(*outDataLen - FRAME_HEADER_SIZE);

            datagramHeader.structData.CTRL_OFFSET128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_CTRL_OFFSET128);
            datagramHeader.structData.CTRL_SIZE128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_CTRL_SIZE128);
            datagramHeader.structData.SYNC_OFFSET128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_SYNC_OFFSET128);
            datagramHeader.structData.SYNC_SIZE128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_SYNC_SIZE128);;
            datagramHeader.structData.HEAD_AUX_OFFSET128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_AUX_DATA_OFFSET128);
            datagramHeader.structData.HEAD_AUX_SIZE128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_AUX_DATA_SIZE128);
            datagramHeader.structData.SIGNAL_OFFSET128 = __builtin_bswap32(UPPM_RAW_COMM_FRAME_SIGNAL_OFFSET128);
            datagramHeader.structData.SIGNAL_SIZE128 =  __builtin_bswap32(UPPM_RAW_COMM_FRAME_SIGNAL_SIZE128);

            memcpy(&outData[0], frameHeader.rawData, FRAME_HEADER_SIZE);
            memcpy(&outData[FRAME_HEADER_SIZE], datagramHeader.rawData, DATAGRAM_HEADER_SIZE);
            break;
        }

        default: printf("reviced UDP frame unknown type, TK: %d, size: %d\r\n", recievedFrameHeader->structData.TK, recievedDatagramHeader->structData.LAYOUT_SIZE128);
    }
}

void mConvertEndians(UPPM_Command_Frame* comm)
{
    comm->TVRS = __builtin_bswap32(comm->TVRS);
    comm->index = __builtin_bswap32(comm->index);
}

void getFdkPayload(uint8_t* data_ptr, uint16_t* dataLen_ptr)
{
    if(!data_ptr) return;

    uint32_t datagramSize = FRAME_HEADER_SIZE + DATAGRAM_HEADER_SIZE + UPPM_FDKFRAME_SIZE;

    FrameHeader frameHeader;
    memset(frameHeader.rawData, 0, FRAME_HEADER_SIZE);

    frameHeader.structData.signature = __builtin_bswap16(FRAME_SIGNATURE);
    frameHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
    frameHeader.structData.TK = UPPM_FDK_FRAME;
    frameHeader.structData.RK = __builtin_bswap32(datagramSize);
    frameHeader.structData.RF128 = __builtin_bswap16(datagramSize/16);
    frameHeader.structData.PF = 1;
    frameHeader.structData.SCH = framesCounter;
    frameHeader.structData.NF = 0;

    DatagramHeader datagramHeader;
    memset(datagramHeader.rawData, 0, DATAGRAM_HEADER_SIZE);

    datagramHeader.structData.LAYOUT = 4;
    datagramHeader.structData.LAYOUT_SIZE128 = 3;
    datagramHeader.structData.RTK = VEEPROM_GetSavedData().ppmNumber;
    datagramHeader.structData.TK = UPPM_FDK_FRAME;
    datagramHeader.structData.RK = __builtin_bswap32(datagramSize - FRAME_HEADER_SIZE);

    datagramHeader.structData.CTRL_OFFSET128 = __builtin_bswap32(UPPM_FDKFRAME_CTRL_OFFSET128);
    datagramHeader.structData.CTRL_SIZE128 = __builtin_bswap32(UPPM_FDKFRAME_CTRL_SIZE128);

    uint32_t othersOffsetValues = __builtin_bswap32(UPPM_FDKFRAME_CTRL_OFFSET128 + UPPM_FDKFRAME_CTRL_SIZE128);
    datagramHeader.structData.SYNC_OFFSET128 = othersOffsetValues;
    datagramHeader.structData.SYNC_SIZE128 = 0;
    datagramHeader.structData.HEAD_AUX_OFFSET128 = othersOffsetValues;
    datagramHeader.structData.HEAD_AUX_SIZE128 = 0;
    datagramHeader.structData.SIGNAL_OFFSET128 = othersOffsetValues;
    datagramHeader.structData.SIGNAL_SIZE128 = 0;

    UPPM_FDK_Frame fdkFrame;
    memset(fdkFrame.rawData, 0, UPPM_FDKFRAME_SIZE);

    // FDK data
    fdkFrame.structData.statCh1 = 1;
    fdkFrame.structData.statCh2 = 1;
    fdkFrame.structData.statCh3 = 1;
    fdkFrame.structData.statCh4 = 1;

    fdkFrame.structData.tempPpm1 = 1;
    fdkFrame.structData.tempPpm2 = 1;
    fdkFrame.structData.tempPpm3 = 1;
    fdkFrame.structData.tempPpm4 = 1;

    fdkFrame.structData.osPpm = 1;

    fdkFrame.structData.pwrZi = 0x30;
    fdkFrame.structData.pwrGet = 0x31;

    fdkFrame.structData.pwrK4 = 0x15;
    fdkFrame.structData.pwrK3 = 0x14;
    fdkFrame.structData.pwrK2 = 0x13;
    fdkFrame.structData.pwrK1 = 0x12;

    fdkFrame.structData.RKsM4 = 1;
    fdkFrame.structData.RKsM3 = 1;
    fdkFrame.structData.RKsM2 = 1;
    fdkFrame.structData.RKsM1 = 1;

    fdkFrame.structData.SsM4 = 1;
    fdkFrame.structData.SsM3 = 1;
    fdkFrame.structData.SsM2 = 1;
    fdkFrame.structData.SsM1 = 1;

    memcpy(&data_ptr[0], frameHeader.rawData, FRAME_HEADER_SIZE);
    memcpy(&data_ptr[FRAME_HEADER_SIZE], datagramHeader.rawData, DATAGRAM_HEADER_SIZE);
    memcpy(&data_ptr[FRAME_HEADER_SIZE + DATAGRAM_HEADER_SIZE], fdkFrame.rawData, UPPM_FDKFRAME_SIZE);

    if(dataLen_ptr) *dataLen_ptr = datagramSize;
}
