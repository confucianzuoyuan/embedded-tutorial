//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CALLBACK_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CALLBACK_H

#include "ebyte_port.h"

void Ebyte_Port_TransmitCallback( uint16e_t state );
void Ebyte_Port_ReceiveCallback(  uint16e_t state ,uint8e_t *buffer, uint8e_t length);


#endif //STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CALLBACK_H
