//
// Created by zuoyuan on 2024/1/26.
//

#ifndef STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_E220X_H
#define STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_E220X_H

#include "ebyte_conf.h"
#include "ebyte_callback.h"

/**
 * 载波中心频率
 *
 * @note 中国-工业和信息化部
 *
 * 《中华人民共和国无线电频率划分规定》(工业和信息化部令第46号令 2018年7月1日起施行)
 *  规定的无需特别批准ISM应用频段( Sub-1G范围 ):
 *          6.765—6.795MHz （中心频率6.780MHz）
 *         13.553—13.567MHz (中心频率13.560MHz)
 *         26.957—27.283MHz (中心频率27.120MHz)
 *          40.66—40.7MHz   (中心频率40.68MHz）
 *
 * 《微功率(短距离)无线电设备的技术要求》（信部无〔2005〕423号）(工业和信息化部第52号文 2019年)
 *  规定的微功率无线电设备应用频段( Sub-1G范围 ):
 *  A类      9KHz—190KHz
 *  B类    1.7MHz—2.1MHz
 *         2.2MHz—3.0MHz
 *         3.1MHz—4.1MHz
 *         4.2MHz—5.6MHz
 *         5.7MHz—6.2MHz
 *         7.3MHz—8.3MHz
 *         8.4MHz—9.9MHz
 *  C类  6.765MHz—6.795MHz
 *      13.553MHz—13.567MHz
 *      26.957MHz—27.283MHz
 *  D类    315kHz—30MHz
 *  E类  40.66MHz—40.7MHz
 *  -------------------------
 *      26.975MHz—27.255MHz    模型、玩具: 海摸/车模
 *       40.61MHz—40.75MHz     模型、玩具: 海摸/车模
 *       40.77MHz—40.85MHz     模型、玩具: 空模
 *          45MHz—45.475MHz    模拟式电话机: 座机类
 *          48MHz—48.475MHz    模拟式电话机: 手机类
 *       72.13MHz—72.84MHz     模型、玩具: 空模
 *        75.4MHz—76.0MHz      声音传输及计量仪表
 *          84MHz—87MHz        声音传输及计量仪表
 *          87MHz—108MHz       声音传输及计量仪表
 *         174MHz—216MHz       生物医学设备
 *       189.9MHz—223.0MHz     声音传输及计量仪表
 *         223MHz—224MHz       电子吊秤专用
 *         224MHz—231MHz       无线数据传送设备
 *         314MHz—316MHz       民用设备控制
 *         407MHz—425MHz       生物医学设备
 *      409.75MHz—409.9875MHz  公众对讲机
 *     419.950MHz—419.275MHz   工业用无线遥控设备
 *         430MHz—432MHz       民用设备控制
 *         433MHz—434.79MHz    民用设备控制
 *    450.0125MHz—450.2125MHz  民用设备控制
 *         470MHz—510MHz       民用计量设备
 *         470MHz—566MHz       通用无线遥控设备
 *         608MHz—630MHz       生物医学设备
 *         614MHz—787MHz       通用无线遥控设备
 *
 * @note 联合国-国际电信联盟-无线电通信部门(ITU-R)
 *
 * 《Report ITU-R SM.2180: Impact of Industrial, Scientific and Medical (ISM) Equipment on Radiocommunication Services》(2011年1月)
 *  SM.2180报告指定无需特别批准的ISM应用频段( Sub-1G范围 ):
 *          6.765—6.795MHz （中心频率6.780MHz）
 *         13.553—13.567MHz (中心频率13.560MHz)
 *         26.957—27.283MHz (中心频率27.120MHz)
 *          40.66—40.7MHz   (中心频率40.68MHz）
 *         433.05—434.79MHz (中心频率433.92MHz）   限制在ITU 1区使用 (欧洲和非洲以及蒙古、原苏联以北的地区和欧洲、非两洲以外原苏联及土耳其的领土)
 *            902—928MHz    (中心频率915MHz）      限制在ITU 2区使用 (南、北美洲和夏威夷)
 *  中国在ITU 3区(亚洲、大洋洲和两洲以外的伊朗领土)
 *
 * @note 欧盟
 *  《COMMISSION DECISION of 9 November 2006 - on harmonisation of the radio spectrum for use by short-range devices》(2006/771/EC)
 *   欧盟无线电开放频段中的：无需特别批准频段还包含有：
 *          868.0—869.25MHz
 *
 *   如需CE认证，请了解欧盟无线设备指令RED 2014/53/EU
 *
 * @note 美国
 *  《47 CFR Part 18 - INDUSTRIAL, SCIENTIFIC, AND MEDICAL EQUIPMENT》
 *   FCC第18部分规则规定了ISM频段
 *
 *   如需FCC认证，请了解 47 CFR Part 15
 */

/* EBYTE_E220_400M22S 模块基础通信频率  */
#if defined(EBYTE_E220_400M22S)
#define RF_FREQUENCY_START                          410125000 // Hz 起始频率
#define RF_FREQUENCY_CH_STEP                        1000000   // Hz 频率计算步进
#define RF_FREQUENCY_CH                             23        // Hz 频率计算通道(步进量)     设定频率 410.125 + 23 * 1 = 433.125MHz

/* EBYTE_E220_900M22S 模块基础通信频率  */
#elif defined(EBYTE_E220_900M22S)
#define RF_FREQUENCY_START                          902125000 // Hz 起始频率
#define RF_FREQUENCY_CH_STEP                        1000000   // Hz 频率计算步进
#define RF_FREQUENCY_CH                             13        // Hz 频率计算通道(步进量)     设定频率 902.125 + 13 * 1 = 915.125MHz

#endif

/* 发射功率 */
#define RF_TX_OUTPUT_POWER                          22        // dBm

/**
 * 单次接收数据包长度限制
 * （内部缓存最大深度 256-Byte ) 当接收长度大于该值时，应用层会主动抛弃该次数据包
 */
#define RF_RX_BUFFER_SIZE                           255

/* 信号调制方式 */
#define RF_TX_RX_MODE                               0         // 0=LoRa  1=FSK

/**
 * LoRa 数据包同步字
 * 注意：当多个模块的同步字不相同时，它们之间无法通信
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x574B    // 0x3444

/* LoRa模式 空速(bps)配置  */
#define RF_LORA_AIR_BPS                             2         // 0=0.3k  1=1.2k  2=2.4k  3=4.8k  4=9.6k  5=19.5k  6=38.4k  7=62.5k

/* FSK模式 前导码长度*/
#define FSK_PREAMBLE_LENGTH                         20

/* FSK模式 同步字 最多8 Byte 需要指定长度*/
#define FSK_MAC_PUBLIC_SYNCWORD_LENGTH              2
#define FSK_MAC_PUBLIC_SYNCWORD_7                   0x57
#define FSK_MAC_PUBLIC_SYNCWORD_6                   0x4B
#define FSK_MAC_PUBLIC_SYNCWORD_5                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_4                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_3                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_2                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_1                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_0                   0x00

/**
 * 常用关系表
 *
 *   DataRate          FrenquencyDeviation        BW
 *    4.8kbps                16KHz                31KHz
 *    9.6kbps                20KHz                31KHz
 *     20kbps                30KHz                50KHz
 *     40kbps                40KHz              62.5KHz
 *    100kbps               100KHz               166KHz
 */
/* FSK模式 空速  */
#define RF_FSK_DATA_RATE                            4800     // 4.8kbps
/* FSK模式 频差  */
#define RF_FSK_FREQUENCY_DEVIATION                  16000    // 16k
/* FSK模式 频宽  */
#define RF_FSK_BANDWIDTH                            31000    // 31k


void Ebyte_E220x_Init(void);

void Ebyte_E220x_SetRfFrequency(uint32e_t frequency);

void Ebyte_E220x_SetRx(uint32e_t timeout);

void Ebyte_E220x_SendPayload(uint8e_t *payload, uint8e_t size, uint32e_t timeout);

void Ebyte_E220x_ClearIrqStatus(uint16e_t irq);

void Ebyte_E220x_ClearDeviceErrors(void);

void Ebyte_E220x_SendPayload(uint8e_t *payload, uint8e_t size, uint32e_t timeout);

void Ebyte_E220x_IntOrPollTask(void);

void Ebyte_E220x_InterruptTrigger(void);

void Ebyte_E220x_SetSleep(uint8e_t cmd);

uint32e_t Ebyte_E220x_GetName(void);

uint8e_t Ebyte_E220x_GetDriverVersion(void);

#endif //STM32F103C8T6_PET_LOCATION_PROJECT_EBYTE_E220X_H
