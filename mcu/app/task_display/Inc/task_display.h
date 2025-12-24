/**
 * @file task_display.h
 * @brief OLED显示任务头文件
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性刷新OLED屏幕显示传感器数据
 *       刷新周期: 1秒 (使用vTaskDelayUntil精确控制)
 *       任务优先级: 4 (高优先级)
 *       LED指示: LED3(蓝色) 运行时点亮
 */

#ifndef __TASK_DISPLAY_H
#define __TASK_DISPLAY_H

#include "FreeRTOS.h"
#include "task.h"

/**
 * ============================================================================
 * 任务配置参数
 * ============================================================================
 */
#define TASK_DISPLAY_NAME "Task_Display" /**< 任务名称 */
#define TASK_DISPLAY_STACK_SIZE 768      /**< 任务栈大小(字) */
#define TASK_DISPLAY_PRIORITY 4          /**< 任务优先级(最高) */
#define TASK_DISPLAY_PERIOD_MS 1000      /**< 刷新周期(毫秒) */

/**
 * ============================================================================
 * 显示模式定义
 * ============================================================================
 */
typedef enum
{
    DISPLAY_MODE_TEMPHUM = 0, /**< 显示温湿度数据 */
    DISPLAY_MODE_LIGHT,       /**< 显示光照数据 */
    DISPLAY_MODE_MAX          /**< 显示模式数量 */
} DisplayMode_t;

/**
 * ============================================================================
 * 外部变量声明
 * ============================================================================
 */

/* 任务句柄 */
extern TaskHandle_t Task_Display_Handle;

/**
 * ============================================================================
 * 函数声明
 * ============================================================================
 */

/**
 * @brief OLED显示任务函数
 * @author Yukikaze
 *
 * @param pvParameters 任务参数(未使用)
 *
 * @note 任务执行流程:
 *       1. 点亮LED3(蓝色)指示任务运行
 *       2. 获取共享传感器数据
 *       3. 根据当前显示模式刷新OLED屏幕
 *       4. 切换显示模式(轮流显示)
 *       5. 熄灭LED3
 *       6. 使用vTaskDelayUntil精确等待下一周期
 *
 *       使用vTaskDelayUntil而非vTaskDelay的原因:
 *       - vTaskDelay: 从调用时刻开始延时，不考虑任务执行时间
 *       - vTaskDelayUntil: 从上次唤醒时刻开始计算，保证精确周期
 */
void Task_Display(void *pvParameters);

/**
 * @brief 创建OLED显示任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 */
BaseType_t Task_Display_Create(void);

#endif /* __TASK_DISPLAY_H */
