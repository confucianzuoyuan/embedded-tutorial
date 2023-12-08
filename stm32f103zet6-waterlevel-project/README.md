# 水位检测项目

本模块是一个外置 A/D 转换器。精度为 24-bit 。

硬件使用 STM32F103ZET6 。

调试使用 USART1 。

接线方式：

```
PB6 <----> 模块的SCK引脚
PB7 <----> 模块的OUT引脚
```

配置如下：

```c
GPIO_InitStruct.Pin = GPIO_PIN_6;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

GPIO_InitStruct.Pin = GPIO_PIN_7;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
```

读取一个采样点的方法

```c
int32_t hx710_read() {
    int32_t data = 0;
    // 拉低时钟信号
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    // 轮询到模块的OUT引脚输出低电平为止
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {}
    // AD转换的精度是 24-bit，所以需要读取 24 个 bit 。
    for (int8_t i = 0; i < 24; i++) {
        // 拉高时钟信号
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        data = data << 1;
        // 拉低时钟信号
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        // 查看OUT引脚输出的电平
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET)
            data++;
    }
    // 拉高时钟信号
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    // 最高位置为 1 。
    data = data ^ 0x800000;
    // 拉低时钟信号
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    return data;
}
```