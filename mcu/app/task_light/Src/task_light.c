/**
 * @file task_light.c
 * @brief 光照采集任务实现
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性(1.5秒)读取光敏电阻的ADC值
 *       读取成功后更新共享数据结构供显示任务使用
 *       任务运行时点亮LED2(绿色)作为指示
 */

#include "task_light.h"
#include "app_data.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include <stdio.h>

/**
 * ============================================================================
 * 外部变量引用
 * ============================================================================
 */

/* ADC转换结果(由ADC中断回调更新) */
extern __IO uint32_t ADC_ConvertedValue;

/**
 * ============================================================================
 * 全局变量定义
 * ============================================================================
 */

/* 任务句柄 */
TaskHandle_t Task_Light_Handle = NULL;

/**
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

/**
 * @brief 光照采集任务函数
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 任务执行流程:
 *       1. 点亮LED2(绿色)指示任务运行
 *       2. 读取光敏电阻ADC转换值
 *       3. 更新共享数据结构
 *       4. 通过串口打印调试信息
 *       5. 熄灭LED2
 *       6. 延时等待下一周期(1.5秒)
 *
 *       ADC值说明:
 *       - 范围: 0-4095 (12位ADC)
 *       - 值越大表示光照越弱(光敏电阻特性)
 *       - 值越小表示光照越强
 */
void Task_Light(void *pvParameters)
{
    uint32_t light_value;
    uint8_t light_percent;
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_LIGHT_PERIOD_MS);

    /* 避免编译器警告 */
    (void)pvParameters;

    /* 初始化上次唤醒时间 */
    xLastWakeTime = xTaskGetTickCount();

    /* 任务主循环 */
    for (;;)
    {
        /* 点亮LED2(绿色)指示任务正在运行 */
        LED2_ON;

        /* 读取ADC转换值 */
        light_value = ADC_ConvertedValue;

        /* 更新共享数据 */
        AppData_UpdateLight(light_value, 1);

        /* 计算光照百分比(值越小光照越强) */
        light_percent = (uint8_t)(100 - (light_value * 100 / 4095));

        /* 保持LED点亮250ms */
        vTaskDelay(pdMS_TO_TICKS(250));

        /* 熄灭LED2 */
        LED2_OFF;

        /* 精确延时到下一个周期（1.5秒）*/
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/**
 * @brief 创建光照采集任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 使用xTaskCreate创建任务
 *       任务栈大小: 512字
 *       任务优先级: 3
 */
BaseType_t Task_Light_Create(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate((TaskFunction_t)Task_Light,
                          (const char *)TASK_LIGHT_NAME,
                          (uint16_t)TASK_LIGHT_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)TASK_LIGHT_PRIORITY,
                          (TaskHandle_t *)&Task_Light_Handle);

    return xReturn;
}
