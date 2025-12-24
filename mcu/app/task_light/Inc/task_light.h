/**
 * @file task_light.h
 * @brief 光照采集任务头文件
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性读取光敏电阻的ADC值
 *       采集周期: 1.5秒
 *       任务优先级: 3 (中优先级)
 *       LED指示: LED2(绿色) 运行时点亮
 */

#ifndef __TASK_LIGHT_H
#define __TASK_LIGHT_H

#include "FreeRTOS.h"
#include "task.h"

/**
 * ============================================================================
 * 任务配置参数
 * ============================================================================
 */
#define TASK_LIGHT_NAME "Task_Light" /**< 任务名称 */
#define TASK_LIGHT_STACK_SIZE 512    /**< 任务栈大小(字) */
#define TASK_LIGHT_PRIORITY 3        /**< 任务优先级(中) */
#define TASK_LIGHT_PERIOD_MS 1500    /**< 采集周期(毫秒) */

/**
 * ============================================================================
 * 外部变量声明
 * ============================================================================
 */

/* 任务句柄 */
extern TaskHandle_t Task_Light_Handle;

/**
 * ============================================================================
 * 函数声明
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
 *       2. 读取光敏电阻ADC值
 *       3. 更新共享数据结构
 *       4. 通过串口打印调试信息
 *       5. 熄灭LED2
 *       6. 延时等待下一周期
 */
void Task_Light(void *pvParameters);

/**
 * @brief 创建光照采集任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 */
BaseType_t Task_Light_Create(void);

#endif /* __TASK_LIGHT_H */
