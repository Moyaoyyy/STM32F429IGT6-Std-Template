/**
 * @file    main.c
 * @author  Yukikaze
 * @brief   主函数
 * @version 0.1
 * @date    2025-12-05
 * @note    说明:
 *       - Task_TempHum: 周期2秒，读取DHT11温湿度，优先级2，LED1(红)
 *       - Task_Light:   周期1.5秒，读取光敏ADC值，优先级3，LED2(绿)
 *       - Task_Display: 周期1秒，刷新OLED显示，优先级4，LED3(蓝)
 *
 * @copyright Copyright (c) 2025 Yukikaze
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* BSP驱动头文件 */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"
#include "bsp_iic.h"
#include "bsp_oled.h"
#include "bsp_adc.h"

/* 应用层任务头文件 */
#include "app_data.h"
#include "task_temphum.h"
#include "task_light.h"
#include "task_display.h"
#include "task_test.h"

/**
 * ============================================================================
 * 任务句柄定义
 * ============================================================================
 */

/* AppTaskCreate任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;

/**
 * ============================================================================
 * 私有函数声明
 * ============================================================================
 */

static void BSP_Init(void);
static void AppTaskCreate(void *pvParameters);
static void SystemClock_Config(void);

/**
 * @brief 主函数
 * @author Yukikaze
 *
 * @return int 返回值(嵌入式系统中不会返回)
 *
 * @note 开发板硬件初始化
 *       创建APP应用任务
 *       启动FreeRTOS，开始多任务调度
 */
int main(void)
{
    BaseType_t xReturn = pdPASS;

    /* 配置系统时钟为180MHz */
    SystemClock_Config();

    /* 开发板硬件初始化 */
    BSP_Init();

    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)1,
                          (TaskHandle_t *)&AppTaskCreate_Handle);

    printf("AppTaskCreate Task Created!\r\n");

    // 创建成功，启动调度器
    if (pdPASS == xReturn)
    {
        /* 启动FreeRTOS调度器 */
        vTaskStartScheduler();
    }
    // 创建失败，红灯常亮
    else
    {
        LED_RED;
        while (1)
            ;
    }

    /* 正常情况下不会执行到这里 */
    while (1)
    {
        /* 空置 */
    }
}

/**
 * @brief 板级外设初始化
 * @author Yukikaze
 *
 * @note 初始化顺序:
 *       1. LED GPIO
 *       2. 串口(调试输出)
 *       3. DHT11温湿度传感器
 *       4. I2C总线(OLED使用)
 *       5. OLED显示屏
 *       6. 光敏电阻ADC
 */
static void BSP_Init(void)
{
    uint32_t i;

    /* 设置NVIC优先级分组为4 (全部用于抢占优先级) */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* LED初始化 */
    LED_GPIO_Config();
    LED_BLUE;

    /* 串口初始化 */
    USARTx_Config();
    printf("USART Initialized\r\n");

    /* 简单延时 */
    for (i = 0; i < 1800000; i++)
    {
        __NOP();
    }
    LED_RGBOFF;

    /* DHT11初始化 */
    DHT11_GPIO_Config();
    printf("DHT11 Initialized\r\n");

    /* I2C初始化(OLED使用) */
    IIC_GPIO_Config();
    printf("I2C Initialized\r\n");

    /* OLED初始化 */
    OLED_Init();
    OLED_CLS();
    OLED_ShowStr(0, 3, (unsigned char *)"Initializing", 1);
    printf("OLED Initialized\r\n");

    /* 光敏电阻ADC初始化 */
    PhotoResistor_Init();
    printf("PhotoResistor ADC Initialized\r\n");
}

/**
 * @brief 应用任务创建函数
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 为了方便管理，所有的任务创建函数都放在这个函数里面
 *       任务创建完成后删除自身，释放资源
 *
 *       任务优先级说明(数值越大优先级越高):
 *       - Task_TempHum: 优先级2 (低)
 *       - Task_Light:   优先级3 (中)
 *       - Task_Display: 优先级4 (高)
 *       - Task_Test:    优先级1 (最低，仅用于验证调度)
 */
static void AppTaskCreate(void *pvParameters)
{
    BaseType_t xReturn = pdPASS;

    /* 避免编译器警告 */
    (void)pvParameters;

    /* 进入临界区(禁止任务调度) */
    taskENTER_CRITICAL();

    /* 初始化共享数据模块 */
    xReturn = AppData_Init();
    if (pdPASS != xReturn)
    {
        goto error;
    }

    /* 创建温湿度采集任务 */
    xReturn = Task_TempHum_Create();
    if (pdPASS != xReturn)
    {
        goto error;
    }

    /* 创建光照采集任务 */
    xReturn = Task_Light_Create();
    if (pdPASS != xReturn)
    {
        goto error;
    }

    /* 创建显示任务 */
    xReturn = Task_Display_Create();
    if (pdPASS != xReturn)
    {
        goto error;
    }

    // /* 创建心跳任务：验证调度与时基 */
    // xReturn = Task_Test_Create();
    // if (pdPASS != xReturn)
    // {
    //     goto error;
    // }

    /* 退出临界区后再删除自身任务，避免调度器一直被禁止 */
    taskEXIT_CRITICAL();
    vTaskDelete(AppTaskCreate_Handle);
    return;

error:
    /* 任务创建失败 */
    LED_RED;
    taskEXIT_CRITICAL();
    vTaskDelete(AppTaskCreate_Handle);
}

/**
 * @brief 堆栈溢出钩子函数
 * @author Yukikaze
 *
 * @param xTask 发生溢出的任务句柄
 * @param pcTaskName 发生溢出的任务名称
 *
 * @note 当检测到任务堆栈溢出时，FreeRTOS会调用此函数
 *       此函数必须在 configCHECK_FOR_STACK_OVERFLOW 非0时实现
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;

    /* 禁止任务切换，保持错误状态 */
    taskDISABLE_INTERRUPTS();

    /* 使用LED闪烁指示错误(不依赖HAL_Delay，防止阻塞) */
    for (;;)
    {
        LED_RED;
        for (volatile uint32_t i = 0; i < 800000U; ++i)
        {
            __NOP();
        }
        LED_RGBOFF;
        for (volatile uint32_t i = 0; i < 800000U; ++i)
        {
            __NOP();
        }
    }
}

/**
 * @brief Malloc失败钩子函数
 * @author Yukikaze
 *
 * @note 当内存分配失败时调用
 */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    LED_RED;
    for (;;)
    {
        /* 常亮指示内存不足 */
    }
}

/**
 * @brief  配置系统时钟为180MHz
 *         HSE = 25MHz
 *         SYSCLK = 180MHz
 *         AHB = 180MHz
 *         APB1 = 45MHz
 *         APB2 = 90MHz
 * @param  无
 * @retval 无
 */
static void SystemClock_Config(void)
{
    RCC_DeInit();
    /* 使能HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    /* 等待HSE就绪 */
    if (RCC_WaitForHSEStartUp() == SUCCESS)
    {
        /* 使能PWR时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        /* 设置调压器输出电压等级为Scale1模式，支持180MHz */
        PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);
        /* HCLK = SYSCLK / 1 = 180MHz */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        /* APB2 = HCLK / 2 = 90MHz */
        RCC_PCLK2Config(RCC_HCLK_Div2);
        /* APB1 = HCLK / 4 = 45MHz (APB1最大频率45MHz) */
        RCC_PCLK1Config(RCC_HCLK_Div4);
        /* 配置PLL: HSE/25 * 360 / 2 = 180MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE, 25, 360, 2, 7);
        /* 使能PLL */
        RCC_PLLCmd(ENABLE);
        /* 等待PLL就绪 */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
        /* 配置Flash预取指，指令缓存，数据缓存和等待状态 */
        FLASH_SetLatency(FLASH_Latency_5);
        FLASH_PrefetchBufferCmd(ENABLE);
        FLASH_InstructionCacheCmd(ENABLE);
        FLASH_DataCacheCmd(ENABLE);
        /* 当PLL准备好后，选择PLL作为系统时钟源 */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        /* 等待PLL被选择为系统时钟源 */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
            /* 等待 */
        }
    }
    else
    {
        /* HSE启动失败时在此处理错误 */
        while (1)
        {
            /* 可以添加错误处理代码 */
        }
    }

    /* 更新系统时钟全局变量 */
    SystemCoreClockUpdate();
}