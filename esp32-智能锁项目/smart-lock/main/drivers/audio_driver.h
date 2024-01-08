#ifndef __AUDIO_DRIVER_H_
#define __AUDIO_DRIVER_H_

#include "driver/gpio.h"
#include "sys/unistd.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DELAY_US(i) usleep(i) // usleep(i)

#define DELAY_MS(i) vTaskDelay(i / portTICK_PERIOD_MS)

#define AUDIO_SDA_PIN 18
#define AUDIO_BUSY_PIN 19

#define AUDIO_SDA_H gpio_set_level(AUDIO_SDA_PIN, 1)
#define AUDIO_SDA_L gpio_set_level(AUDIO_SDA_PIN, 0)

#define AUDIO_READ_BUSY gpio_get_level(AUDIO_BUSY_PIN)

void AUDIO_Init(void);
void Line_1A_WT588F(uint8_t DDATA);

#endif