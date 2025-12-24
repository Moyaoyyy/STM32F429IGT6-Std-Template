/**
 * @file task_display.c
 * @brief OLED显示任务实现
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本任务周期性(1秒)刷新OLED屏幕显示传感器数据
 *       使用vTaskDelayUntil实现精确的1秒刷新周期
 *       轮流显示温湿度数据和光照数据
 *       任务运行时点亮LED3(蓝色)作为指示
 */

#include "task_display.h"
#include "app_data.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include <stdio.h>
#include <string.h>

/**
 * ============================================================================
 * 全局变量定义
 * ============================================================================
 */

/* 任务句柄 */
TaskHandle_t Task_Display_Handle = NULL;

/**
 * ============================================================================
 * 私有函数声明
 * ============================================================================
 */

static void Display_TempHum(SensorData_TypeDef *pData);
static void Display_Light(SensorData_TypeDef *pData);

/**
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

/**
 * @brief 在OLED上显示温湿度数据
 * @author Yukikaze
 *
 * @param pData 指向传感器数据结构的指针
 *
 * @note 显示格式:
 *       第0行: "==TempHum Data=="
 *       第2行: "Temp: XX C"
 *       第4行: "Humi: XX %"
 *       如果数据无效则显示"--"
 */
static void Display_TempHum(SensorData_TypeDef *pData)
{
    char line_buf[20];

    /* 清屏 */
    OLED_CLS();

    /* 标题行 */
    OLED_ShowStr(0, 0, (unsigned char *)"==TempHum Data==", 1);

    /* 温度行 */
    if (pData->dht11_valid)
    {
        sprintf(line_buf, "Temp: %d C", pData->temperature);
    }
    else
    {
        sprintf(line_buf, "Temp: -- C");
    }
    OLED_ShowStr(0, 2, (unsigned char *)line_buf, 1);

    /* 湿度行 */
    if (pData->dht11_valid)
    {
        sprintf(line_buf, "Humi: %d %%", pData->humidity);
    }
    else
    {
        sprintf(line_buf, "Humi: -- %%");
    }
    OLED_ShowStr(0, 4, (unsigned char *)line_buf, 1);

    /* 状态行 */
    if (pData->dht11_valid)
    {
        OLED_ShowStr(0, 6, (unsigned char *)"Status: OK", 1);
    }
    else
    {
        OLED_ShowStr(0, 6, (unsigned char *)"Status: ERR", 1);
    }
}

/**
 * @brief 在OLED上显示光照数据
 * @author Yukikaze
 *
 * @param pData 指向传感器数据结构的指针
 *
 * @note 显示格式:
 *       第0行: "==Light Data=="
 *       第2行: "ADC: XXXX"
 *       第4行: "Light: XX %"
 *       光照百分比计算: 100 - (ADC * 100 / 4095)
 */
static void Display_Light(SensorData_TypeDef *pData)
{
    char line_buf[20];
    uint8_t light_percent;

    /* 清屏 */
    OLED_CLS();

    /* 标题行 */
    OLED_ShowStr(0, 0, (unsigned char *)"==Light Data==", 1);

    /* ADC原始值行 */
    if (pData->light_valid)
    {
        sprintf(line_buf, "ADC: %lu", pData->light_adc);
    }
    else
    {
        sprintf(line_buf, "ADC: ----");
    }
    OLED_ShowStr(0, 2, (unsigned char *)line_buf, 1);

    /* 光照百分比行 */
    if (pData->light_valid)
    {
        light_percent = (uint8_t)(100 - (pData->light_adc * 100 / 4095));
        sprintf(line_buf, "Light: %d %%", light_percent);
    }
    else
    {
        sprintf(line_buf, "Light: -- %%");
    }
    OLED_ShowStr(0, 4, (unsigned char *)line_buf, 1);

    /* 状态行 */
    if (pData->light_valid)
    {
        OLED_ShowStr(0, 6, (unsigned char *)"Status: OK", 1);
    }
    else
    {
        OLED_ShowStr(0, 6, (unsigned char *)"Status: ERR", 1);
    }
}

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
 *       4. 切换显示模式(轮流显示温湿度和光照)
 *       5. 熄灭LED3
 *       6. 使用vTaskDelayUntil精确等待下一周期(1秒)
 *
 * @note vTaskDelayUntil vs vTaskDelay:
 *       vTaskDelay(1000): 任务执行完后延时1000ms
 *       vTaskDelayUntil(&xLastWakeTime, 1000): 从上次唤醒开始算1000ms
 *       后者保证精确的周期性执行，不受任务执行时间影响
 */
void Task_Display(void *pvParameters)
{
    SensorData_TypeDef sensor_data;
    DisplayMode_t current_mode = DISPLAY_MODE_TEMPHUM;
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK_DISPLAY_PERIOD_MS);

    /* 避免编译器警告 */
    (void)pvParameters;

    /* 初始化xLastWakeTime为当前时间 */
    xLastWakeTime = xTaskGetTickCount();

    /* 任务主循环 */
    for (;;)
    {
        /* 点亮LED3(蓝色)指示任务正在运行 */
        LED3_ON;

        /* 获取传感器数据副本(线程安全) */
        AppData_GetSensorData(&sensor_data);

        /* 根据当前模式显示数据 */
        switch (current_mode)
        {
        case DISPLAY_MODE_TEMPHUM:
            Display_TempHum(&sensor_data);
            break;

        case DISPLAY_MODE_LIGHT:
            Display_Light(&sensor_data);
            break;

        default:
            current_mode = DISPLAY_MODE_TEMPHUM;
            break;
        }

        /* 切换到下一个显示模式 */
        current_mode++;
        if (current_mode >= DISPLAY_MODE_MAX)
        {
            current_mode = DISPLAY_MODE_TEMPHUM;
        }

        /* 使用vTaskDelayUntil实现精确的1秒周期
         * 不同于vTaskDelay，vTaskDelayUntil从xLastWakeTime开始计算
         * 即使任务执行时间有波动，也能保证精确的周期性
         */
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        /* 周期结束前熄灭LED3 */
        LED3_OFF;
    }
}

/**
 * @brief 创建OLED显示任务
 * @author Yukikaze
 *
 * @return BaseType_t 创建结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 使用xTaskCreate创建任务
 *       任务栈大小: 512字(因为使用sprintf需要较大栈空间)
 *       任务优先级: 4 (最高，确保显示更新及时)
 */
BaseType_t Task_Display_Create(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate((TaskFunction_t)Task_Display,
                          (const char *)TASK_DISPLAY_NAME,
                          (uint16_t)TASK_DISPLAY_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)TASK_DISPLAY_PRIORITY,
                          (TaskHandle_t *)&Task_Display_Handle);

    return xReturn;
}
