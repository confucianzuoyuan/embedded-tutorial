#include "audio_driver.h"

static const char *AUDIO_TAG = "WTN6170P20";

/// @brief
/// @param DDATA
void Line_1A_WT588F(uint8_t DDATA)
{
    ESP_LOGI(AUDIO_TAG, "Line_1A_WT588F data:0X%2X", DDATA);
    uint8_t S_DATA, j;
    uint8_t B_DATA;
    S_DATA = DDATA;
    AUDIO_SDA_L;
    DELAY_MS(10); // 延时5ms
    B_DATA = S_DATA & 0X01;
    for (j = 0; j < 8; j++)
    {
        if (B_DATA == 1)
        {
            AUDIO_SDA_H;
            DELAY_US(600); // 延时600us
            AUDIO_SDA_L;
            DELAY_US(200); // 延时200us
        }
        else
        {
            AUDIO_SDA_H;
            DELAY_US(200); // 延时200us
            AUDIO_SDA_L;
            DELAY_US(600); // 延时600us
        }
        S_DATA = S_DATA >> 1;
        B_DATA = S_DATA & 0X01;
    }
    AUDIO_SDA_H;
    DELAY_MS(2);
}

/// @brief 一线串口单字节连码示例  一线串口单字节连码示例(F3 + 01 + F3 + 02)
/// @param
void List_1A_Play_WT588F(void)
{
    Line_1A_WT588F(0xF3);
    DELAY_MS(2); // 延时2ms
    Line_1A_WT588F(0x01);
    DELAY_MS(5); // 延时5ms
    Line_1A_WT588F(0xF3);
    DELAY_MS(2); // 延时2ms
    Line_1A_WT588F(0x02);
    DELAY_MS(5); // 延时5ms
}

static void WNT_TEST_Tsk(void *arg)
{
    uint8_t d = 1;
    while (1)
    {
        DELAY_MS(1000);
        Line_1A_WT588F(d++);
        // ESP_LOGI(TAG, "READ_BUSY:%d", READ_BUSY);
        DELAY_MS(1000);
        while (!AUDIO_READ_BUSY)
        {
            ESP_LOGI(AUDIO_TAG, "READ_BUSY:%d", AUDIO_READ_BUSY);
            DELAY_MS(1000);
        }
        if (d == 106)
        {
            d = 0;
        }
    }
    vTaskDelete(NULL);
}

void AUDIO_Init(void)
{
    ESP_LOGI(AUDIO_TAG, "WTN6170P20_Init");

    gpio_config_t io_conf = {};
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    // bit mask of the pins that you want to set,e.g.SDA
    io_conf.pin_bit_mask = (1ULL << AUDIO_SDA_PIN);
    // disable pull-down mode
    // io_conf.pull_down_en = 0;
    // disable pull-up mode
    // io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    gpio_config(&io_conf);

    // io_conf.pull_up_en = 0;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    // // bit mask of the pins that you want to set,e.g.SDA
    io_conf.pin_bit_mask = (1ULL << AUDIO_BUSY_PIN);
    gpio_config(&io_conf);
}