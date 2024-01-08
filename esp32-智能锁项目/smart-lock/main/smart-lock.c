#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "freertos/event_groups.h"

// 按键模块
#include "drivers/keyboard_driver.h"
// led模块
#include "drivers/led_driver.h"
// 蓝牙模块
#include "drivers/bluetooth_driver.h"
// wifi模块
#include "drivers/wifi_driver.h"
// 指纹模块
#include "drivers/fingerprint_driver.h"
// tcp服务器模块
#include "drivers/tcp_driver.h"
// 电机驱动
#include "drivers/motor_driver.h"
// 语音模块
#include "drivers/audio_driver.h"

// Statically allocate and initialize the spinlock
static portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;

static uint8_t keyboard_password[6] = {1, 2, 3, 4, 5, 6};
static uint8_t keyboard_password_count = 0;
static uint8_t keyboard_password_array[6] = {255, 255, 255, 255, 255, 255};

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void clear_keyboard(void)
{
    keyboard_password_count = 0;
    keyboard_password_array[0] = 255;
    keyboard_password_array[1] = 255;
    keyboard_password_array[2] = 255;
    keyboard_password_array[3] = 255;
    keyboard_password_array[4] = 255;
    keyboard_password_array[5] = 255;
}

static void process_isr(void *arg)
{
    uint32_t io_num;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            printf("io_num: %ld\r\n", io_num);
            if (io_num == 0)
            {
                printf("从键盘来了个中断。\r\n");
                uint8_t key = KEYBOARD_read_key();
                if (key == 'M')
                {
                    clear_keyboard();
                }
                else
                {
                    keyboard_password_array[keyboard_password_count] = key;
                    keyboard_password_count++;
                    if (keyboard_password_count == 6)
                    {
                        int i = 0;
                        while (i < 6)
                        {
                            if (keyboard_password[i] == keyboard_password_array[i])
                            {
                                i++;
                                continue;
                            }
                            else
                            {
                                printf("密码错误，请重新输出密码。\r\n");
                                clear_keyboard();
                                Line_1A_WT588F(69);
                                break;
                            }
                        }
                        if (i == 6)
                        {
                            MOTOR_Open_lock();
                            printf("通过键盘开锁成功。\r\n");
                            clear_keyboard();
                        }
                    }
                }
            }
        }
    }
}

static void ISR_QUEUE_Init(void)
{
    // create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    // start gpio task
    xTaskCreate(process_isr, "process_isr", 2048, NULL, 10, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(SC12B_INT, gpio_isr_handler, (void *)SC12B_INT);
    gpio_isr_handler_add(TOUCH_INT, gpio_isr_handler, (void *)TOUCH_INT);
}

void FINGERPRINT_OpenLock(void)
{
    while (1)
    {
        int res = get_image();
        if (res == 0)
        {
            // taskENTER_CRITICAL(&my_spinlock);
            printf("获取指纹图像成功。\r\n");
            int gen_char_res = gen_char();
            if (gen_char_res == 0)
            {
                // printf("生成指纹特征成功。\r\n");
                int search_res = search();
                if (search_res == 0)
                {
                    // printf("通过指纹开锁成功。\r\n");
                    Line_1A_WT588F(85);
                    MOTOR_Open_lock();
                }
                else
                {
                    Line_1A_WT588F(86);
                    // printf("指纹不存在。\r\n");
                }
            }
            // taskEXIT_CRITICAL(&my_spinlock);
        }
        // else if (res == 2)
        // {
        //     // printf("模块上没有指纹。\r\n");
        // }

        vTaskDelay(500 / portTICK_PERIOD_MS);
        // int result = PS_AutoIdentify();
        // if (result == 0)
        // {
        //     MOTOR_Open_lock();
        // }
        // else
        // {
        //     Line_1A_WT588F(86);
        // }
    }
}

// 主程序
void app_main(void)
{
    AUDIO_Init();
    // 电容键盘模块
    ISR_QUEUE_Init();
    KEYBORAD_init();
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // 按键led模块
    RMT_Init();
    // 蓝牙模块
    BLUETOOTH_Init();
    // TCP_SERVER_Init();

    esp_netif_init();
    // esp_event_loop_create_default();
    WIFI_Init();
    // TCP_Init();

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);

    MOTOR_Init();

    FINGERPRINT_Init();

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    xTaskCreate(FINGERPRINT_OpenLock, "finger print open lock", 4096, NULL, 5, NULL);

    // get_chip_sn();     // 获取芯片序列号
    // read_sys_params(); // 读取系统基本参数

    // while (1)
    // {
    //     int res = get_image(); // 探测手指
    //     if (res == 0)
    //     {
    //         printf("有指纹按在模块上了。\r\n");
    //         int gen_char_res = gen_char();
    //         printf("gen char res ==> %d\r\n", gen_char_res);
    //         if (gen_char_res == 0)
    //         {
    //             printf("生成指纹特征成功。\r\n");
    //             int search_res = search();
    //             if (search_res == 0)
    //             {
    //                 MOTOR_Open_lock();
    //             }
    //             else
    //             {
    //                 printf("没有指纹。\r\n");
    //             }
    //         }
    //     }
    //     else if (res == 1)
    //     {
    //         printf("packet error\r\n");
    //     }
    //     else if (res == 2)
    //     {
    //         printf("没指纹按在模块上。\r\n");
    //     }
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);
    // }
}
