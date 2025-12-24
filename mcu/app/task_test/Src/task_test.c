/**
 * @file    task_test.c
 * @author  Yukikaze
 * @brief   测试任务：心跳任务实现
 * @version 0.1
 * @date    2025-12-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "task_test.h"

/**
 * ============================================================================
 * 全局变量定义
 * ============================================================================
 */

/* 任务句柄 */
TaskHandle_t Task_Test_Handle = NULL;

/**
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

/**
 * @brief 心跳测试任务：500ms 翻转一次 LED3，用于验证调度
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 使用LED3(蓝灯)避免与温湿度任务共享LED1造成指示冲突
 *       若LED3正常闪烁，说明FreeRTOS调度器和时基工作正常
 */
void Task_Test(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        /* 使用LED3避免与温湿度任务共享LED1造成指示冲突 */
        LED3_TOGGLE;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief 创建心跳测试任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 使用xTaskCreate创建任务
 *       任务栈大小: 256字(最小心跳任务，栈需求很小)
 *       任务优先级: 1(低优先级，不干扰其他任务)
 */
BaseType_t Task_Test_Create(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate((TaskFunction_t)Task_Test,
                          (const char *)TASK_TEST_NAME,
                          (uint16_t)TASK_TEST_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)TASK_TEST_PRIORITY,
                          (TaskHandle_t *)&Task_Test_Handle);

    return xReturn;
}