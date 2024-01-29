//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_LORA_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_LORA_H

#include "ebyte_core.h"

void LoRa_Init(void);
void LoRa_StartPollTask(void);
void LoRa_Send(uint8_t *buff, uint8_t size);

#endif //STM32F103C8T6_PET_LOCATION_PROJECT_LORA_H
