clion 中 printf 重定向到 uart

```c
uint8_t __io_putchar(int ch) {
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart3, temp, 1, 2);
    return (ch);
}

uint8_t _write(int file, char *ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        __io_putchar(*ptr++);
    }
    return len;
}
```

CMake中最后添加一行

```cmake
set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float ")
```