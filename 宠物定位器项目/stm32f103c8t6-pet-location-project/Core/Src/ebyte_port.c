//
// Created by zuoyuan on 2024/1/26.
//
#include "ebyte_port.h"
#include "main.h"
#include "stdio.h"

extern SPI_HandleTypeDef hspi1;

/*= !!!配置目标硬件平台头文件 =======================================*/

/*==================================================================*/

/* !
 * @brief 配置目标硬件平台SPI接口收发函数
 *
 * @param send EBYTE驱动库上层调用需要传输的数据 1 Byte
 * @return SPI接收的数据 1 Byte
 * @note 当目标平台采用硬件SPI_NSS时，请配置 ebyte_conf.h文件 关闭驱动库的软件控制SPI_CS(NSS)
 *       当目标平台采用软件SPI_NSS时，还需额外配置 Ebyte_Port_SpiCsIoControl() 具体请查阅函数说明
 */
uint8e_t Ebyte_Port_SpiTransmitAndReceivce(uint8e_t send)
{
    uint8e_t result = 0;
    /* !必须提供: SPI接口 */
    HAL_SPI_TransmitReceive(&hspi1, &send, &result, 1, 1000);
    return result;
}

/* !
 * @brief (可选)配置目标硬件平台SPI_CS(NSS)函数
 *
 * @param cmd EBYTE驱动库上层期望的控制指令
 *            @arg 0: 期望CS(NSS)引脚输出低电平逻辑信号 EBYTE模块SPI接口为低电平选中
 *            @arg 1: 期望CS(NSS)引脚输出高电平逻辑信号
 * @note 当目标平台采用硬件SPI_NSS时，由配置文件定义 此函数将无效
 *       当目标平台采用软件SPI_NSS时，请配置此函数
 */
void Ebyte_Port_SpiCsIoControl(uint8e_t cmd)
{
    if (cmd == 1)
    {
        /* !可选: SPI CS控制 E22xx高电平未选中 */
        HAL_GPIO_WritePin(LoRa_CS_GPIO_Port, LoRa_CS_Pin, GPIO_PIN_SET);
    }
    else
    {
        /* !可选: SPI CS控制 E22xx低电平选中 */
        HAL_GPIO_WritePin(LoRa_CS_GPIO_Port, LoRa_CS_Pin, GPIO_PIN_RESET);
    }
}

/* !
 * @brief 配置目标硬件平台复位引脚 NRST 控制函数
 *
 * @param cmd EBYTE驱动库上层期望的控制指令
 *            @arg 0: 期望NRST 引脚输出低电平逻辑信号
 *            @arg 1: 期望NRST 引脚输出高电平逻辑信号
 * @note EBYTE模块NRST为低电平复位
 */
void Ebyte_Port_RstIoControl(uint8e_t cmd)
{
    if (cmd == 1)
    {
        /* !必须提供: NRST控制IO 输出高电平 */
        HAL_GPIO_WritePin(LoRa_RST_GPIO_Port, LoRa_RST_Pin, GPIO_PIN_SET);
    }
    else
    {
        /* !必须提供: 目标硬件电路对应NRST控制IO 输出低电平 */
        HAL_GPIO_WritePin(LoRa_RST_GPIO_Port, LoRa_RST_Pin, GPIO_PIN_RESET);
    }
}

/* !
 * @brief (可选)配置目标硬件平台发送使能引脚 TXEN 控制函数
 *
 * @param cmd EBYTE驱动库上层期望的控制指令
 *            @arg 0: 期望TXEN 引脚输出低电平逻辑信号
 *            @arg 1: 期望TXEN 引脚输出高电平逻辑信号
 * @note EBYTE模块TXEN为高电平生效
 *       当目标平台采用模块DIO2自动控制TXEN时，此函数将无效
 */
void Ebyte_Port_TxenIoControl(uint8e_t cmd)
{
    if (cmd == 1)
    {
        /* !可选: 目标硬件电路对应TXEN控制IO 输出高电平 */
        HAL_GPIO_WritePin(LoRa_TxEN_GPIO_Port, LoRa_TxEN_Pin, GPIO_PIN_SET);
    }
    else
    {
        /* !可选: 目标硬件电路对应TXEN控制IO 输出低电平 */
        HAL_GPIO_WritePin(LoRa_TxEN_GPIO_Port, LoRa_TxEN_Pin, GPIO_PIN_RESET);
    }
}

/* !
 * @brief 配置目标硬件平台接收使能引脚 RXEN 控制函数
 *
 * @param cmd EBYTE驱动库上层期望的控制指令
 *            @arg 0: 期望RXEN引脚输出低电平逻辑信号
 *            @arg 1: 期望RXEN引脚输出高电平逻辑信号
 * @note EBYTE模块RXEN为高电平生效
 */
void Ebyte_Port_RxenIoControl(uint8e_t cmd)
{
    if (cmd == 1)
    {
        /* !必须提供: 目标硬件电路对应RXEN控制IO 输出高电平 */
        HAL_GPIO_WritePin(LoRa_RxEN_GPIO_Port, LoRa_RxEN_Pin, GPIO_PIN_SET);
    }
    else
    {
        /* !必须提供: 目标硬件电路对应RXEN控制IO 输出低电平 */
        HAL_GPIO_WritePin(LoRa_RxEN_GPIO_Port, LoRa_RxEN_Pin, GPIO_PIN_RESET);
    }
}

/* !
 * @brief 配置目标硬件平台接收使能引脚 BUSY 控制函数
 *
 * @note EBYTE模块BUSY为高电平表示忙
 */
uint8e_t Ebyte_Port_BusyIoRead(void)
{
    uint8e_t result = 0;

    /* !必须提供: 目标硬件电路对应BUSY IO 状态读取 */
    result = HAL_GPIO_ReadPin(LoRa_BUSY_GPIO_Port, LoRa_BUSY_Pin);
    return result;
}

/* !
 * @brief 配置目标硬件平台延时函数
 *
 * @param time 期望延时毫秒
 * @note 请注意时序，模块初始化时将调用此函数，请注意中断是否会影响到此函数
 */
void Ebyte_Port_DelayMs(uint16e_t time)
{
    /* !必须提供: 延时函数 */
    HAL_Delay(time);
}
