//
// Created by zuoyuan on 2024/1/26.
//
#include "LoRa.h"

/**
 * @description: 初始化 LoRa
 */
void LoRa_Init(void)
{
    /* 1. 初始化 LoRa */
    Ebyte_RF.Init();
}

/**
 * @description: LoRa的周期执行的函数，建议放在主循环中
 * @return {*}
 */
void LoRa_StartPollTask(void)
{
    Ebyte_RF.StartPollTask();
}

/**
 * @description: 发送数据
 * @param {uint8_t} *buff 要发送的数据
 * @param {uint8_t} size 要发送的数据字节数
 * @return {*}
 */
void LoRa_Send(uint8_t *buff, uint8_t size)
{
    /* 1. 发送数据  注意超时时间必须是0*/
    Ebyte_RF.Send(buff, size, 0);
}
