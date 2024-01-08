#include "keyboard_driver.h"
#include "audio_driver.h"

void Delay_ms(uint8_t time)
{
    vTaskDelay(time / portTICK_PERIOD_MS);
}

/// <summary>
/// 产生起始信号
/// </summary>
/// <param name=""></param>
void I2C_Start(void)
{
    I2C_SDA_OUT; // sda线输出
    I2C_SDA_H;
    I2C_SCL_H;
    Delay_ms(1);
    I2C_SDA_L; // START:when CLK is high,DATA change form high to low
    Delay_ms(1);
    I2C_SCL_L; // 钳住I2C总线，准备发送或接收数据
    Delay_ms(1);
}

/// <summary>
/// 产生停止信号
/// </summary>
/// <param name=""></param>
void I2C_Stop(void)
{
    I2C_SCL_L;
    I2C_SDA_OUT; // sda线输出
    I2C_SDA_L;   // STOP:when CLK is high DATA change form low to high
    Delay_ms(1);
    I2C_SCL_H;
    Delay_ms(1);
    I2C_SDA_H; // 发送I2C总线结束信号
}

/// <summary>
/// 下发应答
/// </summary>
/// <param name="ACKSignal"></param>
void I2C_Ack(uint8_t x)
{
    I2C_SCL_L;
    I2C_SDA_OUT;
    if (x)
    {
        I2C_SDA_H;
    }
    else
    {
        I2C_SDA_L;
    }
    Delay_ms(1);
    I2C_SCL_H;
    Delay_ms(1);
    I2C_SCL_L;
}

/// <summary>
/// 等待应答信号到来,成功返回0
/// </summary>
/// <param name=""></param>
/// <returns></returns>
uint8_t I2C_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    I2C_SCL_L;
    I2C_SDA_IN; // SDA设置为输入
    Delay_ms(1);
    I2C_SCL_H;
    Delay_ms(1);
    while (I2C_READ_SDA)
    {
        if (ucErrTime++ > 250)
        {
            // I2C_Stop();
            // printf("接受应答失败\n");
            return 1;
        }
    }
    I2C_SCL_L;
    // printf("接受应答成功\n");
    return 0;
}

/// <summary>
/// 发送一个字节
/// </summary>
/// <param name="txd"></param>
void I2C_Send_Byte(uint8_t d)
{
    uint8_t t = 0;
    I2C_SDA_OUT;
    while (8 > t++)
    {
        I2C_SCL_L;
        Delay_ms(1);
        if (d & 0x80)
        {
            I2C_SDA_H;
        }
        else
        {
            I2C_SDA_L;
        }
        Delay_ms(1); // 对TEA5767这三个延时都是必须的
        I2C_SCL_H;
        Delay_ms(1);
        d <<= 1;
    }
}

/// <summary>
/// 读1个字节
/// </summary>
/// <param name="ack">是否发送应答</param>
/// <returns></returns>
uint8_t I2C_Read_Byte(uint8_t ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    I2C_SDA_IN; // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        I2C_SCL_L;
        Delay_ms(1);
        I2C_SCL_H;
        receive <<= 1;
        if (I2C_READ_SDA)
        {
            receive++;
        }
        Delay_ms(1);
    }
    I2C_Ack(ack); // 发送ACK
    return receive;
}

/// <summary>
/// 发送数据并返回应答
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
uint8_t SendByteAndGetNACK(uint8_t data)
{
    I2C_Send_Byte(data);
    return I2C_Wait_Ack();
}

/// <summary>
/// SC12B 简易读取按键值函数（默认直接读取）
/// 此函数只有初始化配置默认的情况下，直接调用，如果在操作前有写入或者其他读取不能调用默认
/// </summary>
/// <param name="deviceAddr"></param>
/// <param name="DAT16"></param>
/// <returns></returns>
uint8_t I2C_Read_Key(void)
{
    I2C_Start();
    if (SendByteAndGetNACK((0x40 << 1) | 0x01))
    {
        I2C_Stop();
        return 0;
    }
    uint8_t i = 0;
    uint8_t k = 0;
    I2C_SDA_IN; // SDA设置为输入
    while (8 > i)
    {
        i++;
        I2C_SCL_L;
        Delay_ms(1);
        I2C_SCL_H;
        if (!k && I2C_READ_SDA)
        {
            k = i;
        }
        Delay_ms(1);
    }
    if (k)
    {
        I2C_Ack(1);
        I2C_Stop();
        return k;
    }
    I2C_Ack(0);
    I2C_SDA_IN; // SDA设置为输入
    while (16 > i)
    {
        i++;
        I2C_SCL_L;
        Delay_ms(1);
        I2C_SCL_H;
        if (!k && I2C_READ_SDA)
        {
            k = i;
        }
        Delay_ms(1);
    }
    I2C_Ack(1);
    I2C_Stop();
    return k;
}

uint8_t KEYBOARD_read_key(void)
{
    uint16_t key = I2C_Read_Key();
    Line_1A_WT588F(11);
    if (key == 4)
    {
        printf("key: 1\r\n");
        light_led(3);
        return 1;
    }
    else if (key == 3)
    {
        printf("key: 2\r\n");
        light_led(2);
        return 2;
    }
    else if (key == 2)
    {
        printf("key: 3\r\n");
        light_led(1);
        return 3;
    }
    else if (key == 7)
    {
        printf("key: 4\r\n");
        light_led(6);
        return 4;
    }
    else if (key == 6)
    {
        printf("key: 5\r\n");
        light_led(5);
        return 5;
    }
    else if (key == 5)
    {
        printf("key: 6\r\n");
        light_led(4);
        return 6;
    }
    else if (key == 10)
    {
        printf("key: 7\r\n");
        light_led(9);
        return 7;
    }
    else if (key == 9)
    {
        printf("key: 8\r\n");
        light_led(8);
        return 8;
    }
    else if (key == 8)
    {
        printf("key: 9\r\n");
        light_led(7);
        return 9;
    }
    else if (key == 1)
    {
        printf("key: 0\r\n");
        light_led(0);
        return 0;
    }
    else if (key == 12)
    {
        printf("key: #\r\n");
        light_led(11);
        return '#';
    }
    else if (key == 11)
    {
        printf("key: M\r\n");
        light_led(10);
        return 'M';
    }
    return 255;
}

void KEYBORAD_init(void)
{
    gpio_config_t io_conf;
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    // bit mask of the pins that you want to set,e.g.SDA
    io_conf.pin_bit_mask = ((1ULL << SC12B_SCL) | (1ULL << SC12B_SDA)); // 重中之重！！！！！！！！！！！！！
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    gpio_config(&io_conf);

    // 中断
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << SC12B_INT);
    gpio_config(&io_conf);
}