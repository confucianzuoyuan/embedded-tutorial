//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CONF_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CONF_H

#include "main.h"
/*= !!!配置产品型号 单选 ============================================*/
// #define EBYTE_E07_900M10S
// #define EBYTE_E10_400M20S
// #define EBYTE_E10_900M20S
// #define EBYTE_E22_400M22S
// #define EBYTE_E22_900M22S
// #define EBYTE_E49_400M20S
// #define EBYTE_E49_900M20S
#define EBYTE_E220_400M22S
// #define EBYTE_E220_900M22S
/*==================================================================*/

/* SPI通信 由软件控制CS(NSS)信号  0:禁用 1:启用 */
#define EBYTE_PORT_SPI_CS_SOFTWARE 1

/* 如何检测模块发送时的状态  0:异步中断通知  1:同步阻塞 */
#define EBYTE_RF_TRANSMIT_CHECK_MODE 1

#endif //STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_CONF_H
