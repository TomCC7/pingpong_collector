/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "MotorShield.h"
#include "N64Controller.h"
#include "OneWire.h"
#include "Servo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int pulse;
int ifshowdist;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void showdist(LCD *display)
{
    float distance = pulse / 148.0;
    uint8_t msg[16];
    if (pulse > 25000)
    {
        sprintf((char *)msg, "N/A pulse=%d", pulse);
        display->setCursor(1, 0);
        display->send(msg, 14);
        // HAL_UART_Transmit(&huart2, msg1, sizeof(msg1), 10000);
    }
    else
    {
        sprintf((char *)msg, "dist=%f", distance);
        display->setCursor(1, 0);
        display->send(msg, 14);
        // HAL_UART_Transmit(&huart2, msg1, sizeof(msg1), 10000);
    }
    ifshowdist = 0;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick.
     */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    MX_TIM4_Init();
    MX_I2C3_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM5_Init();
    /* USER CODE BEGIN 2 */
    SerialTransceiver serial(huart2);
    MotorShield _MS(&hi2c1, &huart2, MS_ADDR);
    LCD display(hi2c3);
    Servo servo(&htim4);
    // N64 n64;
    display.start();

    // distance sensor
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);   // Starts timer 3
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // Starts timer 2
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 2);
    HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_1);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    const size_t BUFFER_SIZE = 5;
    display.clear();
    display.setCursor(0, 0);
    uint8_t msg[11] = "Collected:";
    display.send(msg, 10);
    servo.SetServoAngle(100);
    while (1)
    {
        // distance sensor
        if (ifshowdist)
        {
            showdist(&display);
        }
        float distance = pulse / 148.0;
        // if (distance < 6)
        // {
        //     _MS.rotate(true);
        //     continue;
        // }
        // N64 {{
        // if (n64.is_connected)
        // {
        //     auto n64_data = n64.query();
        //     // Joystick test {{
        //     if (n64_data.R)
        //         _MS.rotate(false);
        //     else if (n64_data.L)
        //         _MS.rotate(true);
        //     else
        //         _MS.go_dir(n64_data.JoystickY, n64_data.JoystickX);
        //     if (n64_data.Z)
        //         servo.SetServoAngle(0);
        //     else
        //         servo.SetServoAngle(0);

        //     continue;
        // }
        // }}
        serial.heartbeat();
        Packet<BUFFER_SIZE> pac;
        pac.size = 0;
        serial.receive<BUFFER_SIZE>(pac);
        Device device = Device(pac.header.device);
        switch (device)
        {
        case SCREEN: {
            uint8_t str_collected[4] = "255";
            uint8_t tmp = pac.data[1];
            for (int i = 0; i < 3; i++)
            {
                str_collected[2 - i] = (uint8_t)(tmp % 10) + '0';
                tmp /= 10;
            }
            display.setCursor(0, 10);
            display.send(str_collected, 3);
            break;
        }
        case MOTOR: {
            const MotorData &data = *(const MotorData *)(pac.data);
            _MS.SetMotor(pac.header.index, data.pwm, data.in1, data.in2);
            break;
        }
        case SERVO:
            servo.SetServoAngle(pac.data[0]);
            break;
        default:
            break;
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
