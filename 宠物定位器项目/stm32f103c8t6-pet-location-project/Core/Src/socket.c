//
// Created by zuoyuan on 2024/1/26.
//
#include "main.h"
#include "stdio.h"
#include "socket.h"

extern UART_HandleTypeDef huart2;

uint8_t socket_receive_buffer[1000];
volatile uint8_t socket_count = 0;


/// AT测试指令
void socket_at_test(void) {
    HAL_UART_Transmit(&huart2, "adminAT\n", 8, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\nAT指令测试结果：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

/// 查询信号质量
void socket_at_csq(void) {
    HAL_UART_Transmit(&huart2, "adminAT+CSQ?\n", 13, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n信号质量：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

/// 查询模组运行状态
void socket_at_runst(void) {
    HAL_UART_Transmit(&huart2, "adminAT+RUNST?\n", 15, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n模组运行状态：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_query_params(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCK?\n", 14, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\nSOCKET参数：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_set_params(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCK=TCP,47.92.80.71,8090\n", 34, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n设置SOCKET参数：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_enable(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCKEN=ON\n", 18, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\nSOCKET使能：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_query_connect_state(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCKLK?\n", 16, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\nSOCKET连接状态：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_set_socket_type(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCKSL=LONG\n", 20, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\nSOCKET设置为长连接：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_creg(void) {
    HAL_UART_Transmit(&huart2, "adminAT+CREG?\n", 14, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n网络注册状态：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_at_max_connection(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCKRSNUM=60\n", 21, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n最大重连次数：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_exit_config_mode(void) {
    HAL_UART_Transmit(&huart2, "adminAT+ENTM\n", 13, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n退出配置模式：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}

void socket_disable_ssl(void) {
    HAL_UART_Transmit(&huart2, "adminAT+SOCKSSL=OFF\n", 20, HAL_MAX_DELAY);
    while (1) {
        HAL_UART_Receive(&huart2, &socket_receive_buffer[socket_count], 1, HAL_MAX_DELAY);
        if (socket_receive_buffer[socket_count] == 'K' && socket_receive_buffer[socket_count - 1] == 'O') {
            printf("\r\n禁用SSL：\r\n");
            printf("%.*s\r\n", socket_count + 1, socket_receive_buffer);
            socket_count = 0;
            break;
        }
        socket_count++;
    }
}