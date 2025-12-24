/**
 * @file task_temphum.c
 * @brief 温湿度采集任务实现
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性(2秒)读取DHT11传感器的温湿度数据
 *       读取成功后更新共享数据结构供显示任务使用
 *       任务运行时点亮LED1(红色)作为指示
 */

#include "task_temphum.h"
#include "app_data.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include <stdio.h>

/**
 * ============================================================================
 * 全局变量定义
 * ============================================================================
 */

/* 任务句柄 */
TaskHandle_t Task_TempHum_Handle = NULL;

/**
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

/**
 * @brief 温湿度采集任务函数
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 任务执行流程:
 *       1. 点亮LED1(红色)指示任务运行
 *       2. 读取DHT11温湿度数据
 *       3. 更新共享数据结构
 *       4. 通过串口打印调试信息
 *       5. 熄灭LED1
 *       6. 延时等待下一周期(2秒)
 */
void Task_TempHum(void *pvParameters)
{
    DHT11_Data_TypeDef dht11_data;
    uint8_t read_result;
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_TEMPHUM_PERIOD_MS);

    /* 避免编译器警告 */
    (void)pvParameters;

    /* 初始化上次唤醒时间 */
    xLastWakeTime = xTaskGetTickCount();

    /* 任务主循环 */
    for (;;)
    {
        /* 点亮LED1(红色)指示任务正在运行 */
        LED1_ON;

        /* 读取DHT11温湿度数据 */
        read_result = Read_DHT11(&dht11_data);

        if (read_result == 1)
        {
            /* 读取成功，更新共享数据 */
            AppData_UpdateTempHum(dht11_data.temp_int,
                                  dht11_data.humi_int,
                                  1);
        }
        else
        {
            /* 读取失败，标记数据无效 */
            AppData_UpdateTempHum(0, 0, 0);
        }

        /* 保持LED点亮300ms */
        vTaskDelay(pdMS_TO_TICKS(300));

        /* 熄灭LED1 */
        LED1_OFF;

        /* 精确延时到下一个周期（2秒）*/
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief 创建温湿度采集任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 使用xTaskCreate创建任务
 *       任务栈大小: 256字
 *       任务优先级: 2
 */
BaseType_t Task_TempHum_Create(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate((TaskFunction_t)Task_TempHum,
                          (const char *)TASK_TEMPHUM_NAME,
                          (uint16_t)TASK_TEMPHUM_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)TASK_TEMPHUM_PRIORITY,
                          (TaskHandle_t *)&Task_TempHum_Handle);

    return xReturn;
}
