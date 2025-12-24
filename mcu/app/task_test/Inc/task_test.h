#ifndef __TASK_TEST_H
#define __TASK_TEST_H

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "bsp_led.h"

/**
 * ============================================================================
 * 任务配置参数
 * ============================================================================
 */
#define TASK_TEST_NAME "Task_Heartbeat" /**< 任务名称 */
#define TASK_TEST_STACK_SIZE 256        /**< 任务栈大小(字) */
#define TASK_TEST_PRIORITY 1            /**< 任务优先级(低) */

/**
 * ============================================================================
 * 外部变量声明
 * ============================================================================
 */

/* 任务句柄 */
extern TaskHandle_t Task_Test_Handle;

/**
 * ============================================================================
 * 函数声明
 * ============================================================================
 */

/**
 * @brief 心跳测试任务函数
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 500ms翻转一次LED3，用于验证FreeRTOS调度器工作正常
 */
void Task_Test(void *pvParameters);

/**
 * @brief 创建心跳测试任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 */
BaseType_t Task_Test_Create(void);

#endif /* __TASK_TEST_H */