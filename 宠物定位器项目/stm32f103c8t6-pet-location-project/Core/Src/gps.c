//
// Created by zuoyuan on 2024/1/26.
//
#include "gps.h"
#include "main.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;

uint8_t gps_receive_buffer[1000];
volatile uint8_t gps_count = 0;


/// 使能GPS
void gps_enable(void) {
    HAL_UART_Transmit(&huart2, "adminAT+GPSEN=ON\n", 17, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &gps_receive_buffer[gps_count], 1, HAL_MAX_DELAY);
        if (gps_receive_buffer[gps_count] == 'K' && gps_receive_buffer[gps_count - 1] == 'O') {
            printf("\r\n打开GPS：\r\n");
            printf("%.*s\r\n", gps_count + 1, gps_receive_buffer);
            gps_count = 0;
            break;
        }
        gps_count++;
    }
}

/// 关闭GPS
void gps_disable(void) {
    HAL_UART_Transmit(&huart2, "adminAT+GPSEN=OFF\n", 18, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &gps_receive_buffer[gps_count], 1, HAL_MAX_DELAY);
        if (gps_receive_buffer[gps_count] == 'K' && gps_receive_buffer[gps_count - 1] == 'O') {
            printf("\r\n关闭GPS：\r\n");
            printf("%.*s\r\n", gps_count + 1, gps_receive_buffer);
            gps_count = 0;
            break;
        }
        gps_count++;
    }
}

/// 设置GPS为冷启动
void gps_cold_start(void) {
    HAL_UART_Transmit(&huart2, "adminAT+GPSST=2\n", 16, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &gps_receive_buffer[gps_count], 1, HAL_MAX_DELAY);
        if (gps_receive_buffer[gps_count] == 'K' && gps_receive_buffer[gps_count - 1] == 'O') {
            printf("\r\nGPS设置为冷启动：\r\n");
            printf("%.*s\r\n", gps_count + 1, gps_receive_buffer);
            gps_count = 0;
            break;
        }
        gps_count++;
    }
}

/// 获取GPS位置信息
void gps_location(void) {
    HAL_UART_Transmit(&huart2, "adminAT+GPS?\n", 13, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &gps_receive_buffer[gps_count], 1, HAL_MAX_DELAY);
        if (gps_receive_buffer[gps_count] == 'K' && gps_receive_buffer[gps_count - 1] == 'O') {
            printf("\r\nGPS位置信息：\r\n");
            printf("%.*s\r\n", gps_count + 1, gps_receive_buffer);
            gps_count = 0;
            break;
        }
        gps_count++;
    }
}

void gps_location_info(void) {
    HAL_UART_Transmit(&huart2, "adminAT+GPSINFO?\n", 17, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &gps_receive_buffer[gps_count], 1, HAL_MAX_DELAY);
        if (gps_receive_buffer[gps_count] == 'K' && gps_receive_buffer[gps_count - 1] == 'O') {
            printf("\r\nGPS详细位置信息：\r\n");
            printf("%.*s\r\n", gps_count + 1, gps_receive_buffer);
            gps_count = 0;
            break;
        }
        gps_count++;
    }
}