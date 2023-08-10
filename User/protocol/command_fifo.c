/*
 * command_fifo.c
 *
 *  Created on: May 24, 2023
 *      Author: Dmitriy Kostyuchik
 */

#include "command_fifo.h"

uint8_t buf_wr_index, buf_rd_index, buf_counter;
UPPM_Command_Frame command_buf[32];

void CommFIFO_Init()
{
    buf_wr_index = 0;
    buf_rd_index = 0;
    buf_counter = 0;
}

bool CommFIFO_PutData(UPPM_Command_Frame new_data)
{
    if(buf_counter < COMMAND_FIFO_SIZE)
    {
        command_buf[buf_wr_index++] = new_data;

        if(buf_wr_index == COMMAND_FIFO_SIZE) buf_wr_index = 0;

        buf_counter++;
        return true;
    }
    else return false;
}

UPPM_Command_Frame CommFIFO_GetData()
{
    UPPM_Command_Frame data;

    if(buf_counter == 0)
    {
        //doevents;
        // �ӧ�٧ӧ�ѧ�ѧ�� �ߧ�ݧ֧ӧ�� ��ѧ�ܧ� �ڧݧ� �اէѧ�� ���ܧ� �ҧ���֧� �ߧѧ��ݧߧڧ���?
        UPPM_Command_Frame zero_pack = {0};
        return zero_pack;
    }

    data = command_buf[buf_rd_index++];

    if(buf_rd_index == COMMAND_FIFO_SIZE) buf_rd_index = 0;

    buf_counter--;

    return data;
}

UPPM_Command_Frame CommFIFO_PeekData()
{
    return command_buf[buf_rd_index];
}

uint8_t CommFIFO_Count()
{
    return buf_counter;
}
