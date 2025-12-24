/**
 * @file task_temphum.h
 * @brief 温湿度采集任务头文件
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性读取DHT11传感器的温湿度数据
 *       采集周期: 2秒
 *       任务优先级: 2 (低优先级)
 *       LED指示: LED1(红色) 运行时点亮
 */

#ifndef __TASK_TEMPHUM_H
#define __TASK_TEMPHUM_H

#include "FreeRTOS.h"
#include "task.h"

/**
 * ============================================================================
 * 任务配置参数
 * ============================================================================
 */
#define TASK_TEMPHUM_NAME "Task_TempHum" /**< 任务名称 */
#define TASK_TEMPHUM_STACK_SIZE 512      /**< 任务栈大小(字) */
#define TASK_TEMPHUM_PRIORITY 2          /**< 任务优先级(低) */
#define TASK_TEMPHUM_PERIOD_MS 2000      /**< 采集周期(毫秒) */

/**
 * ============================================================================
 * 外部变量声明
 * ============================================================================
 */

/* 任务句柄 */
extern TaskHandle_t Task_TempHum_Handle;

/**
 * ============================================================================
 * 函数声明
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
 *       6. 延时等待下一周期
 */
void Task_TempHum(void *pvParameters);

/**
 * @brief 创建温湿度采集任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 */
BaseType_t Task_TempHum_Create(void);

#endif /* __TASK_TEMPHUM_H */
