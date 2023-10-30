#include "main.h"
#include "stdio.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart2;

uint8_t __io_putchar(int ch) {
    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart2, temp, 1, 2);
    return (ch);
}

uint8_t _write(int file, char *ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        __io_putchar(*ptr++);
    }
    return len;
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);

static void MX_ADC1_Init(void);

static void MX_USART2_UART_Init(void);

// ADC1转换的电压值通过DMA方式传到SRAM
volatile uint16_t ADC_ConvertedValue;
uint8_t adc_conv_complete_flag = 0;

float TDS = 0.0, TDS_voltage;
float TDS_value = 0.0, voltage_value;
float temp_data = 0.0;
float compensationCoefficient = 1.0; // 温度校准系数
float compensationVolatge;
float kValue = 1.0;

unsigned char Tx[20]; // 无线发送缓存

/**************TDS值采集函数***************/
void TDS_Value_Conversion() {
    TDS_voltage = (float) ADC_ConvertedValue / 4096 * 3.3; // 读取转换的AD值

    compensationCoefficient = 1.0 + 0.02 * ((temp_data / 10) - 25.0);
    compensationVolatge = TDS_voltage / compensationCoefficient;
    TDS_value = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge -
                 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) *
                0.5 * kValue;

    if ((TDS_value <= 0)) {
        TDS_value = 0;
    }
    if ((TDS_value > 1400)) {
        TDS_value = 1400;
    }

    printf("tds value: %f\r\n", TDS_value);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t * ) & ADC_ConvertedValue, 1);

    while (1) {
        if (adc_conv_complete_flag == 1) {
            TDS_Value_Conversion();
            HAL_Delay(1000);
            HAL_ADC_Start_DMA(&hadc1, (uint32_t * ) & ADC_ConvertedValue, 1);
        }
    }
}

// when DMA conversion is completed, HAL_ADC_ConvCpltCallback function
// will interrupt the processor. You can find this function in
// Drivers>STM32F4xx_HAL_Drivers>stm32f4xx_hal_adc.c file as __weak attribute
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    // I set adc_conv_complete_flag variable to 1 when,
    // HAL_ADC_ConvCpltCallback function is call.
    adc_conv_complete_flag = 1;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
