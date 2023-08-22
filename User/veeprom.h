#ifndef USER_VEEPROM_H_
#define USER_VEEPROM_H_

#include <debug.h>

typedef struct
{
    uint16_t serial;
    uint8_t dataByte;
    uint8_t ppmNumber;
}SaveData;

void VEEPROM_Init();

SaveData VEEPROM_GetSavedData();
void VEEPROM_SaveData(const SaveData* dataStruct);


extern uint32_t _user_data_start;
extern uint32_t _user_data_end;

#endif /* USER_VEEPROM_H_ */
