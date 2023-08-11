#include "veeprom.h"

SavedData deviceData;

void VEEPROM_Init()
{
    deviceData.ppmNumber = 0;
}

SavedData VEEPROM_GetSavedData()
{
     return deviceData;
}

void VEEPROM_SaveData(const SavedData* dataStruct)
{

}
