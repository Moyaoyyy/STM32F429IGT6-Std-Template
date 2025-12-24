/**
 * @file app_data.c
 * @brief 应用层共享数据模块实现
 * @author Yukikaze
 * @date 2025-12-2
 *
 * @note 本模块管理各任务间共享的传感器数据
 *       使用互斥量保护数据访问，确保线程安全
 */

#include "app_data.h"
#include <string.h>

/**
 * ============================================================================
 * 全局变量定义
 * ============================================================================
 */

/* 传感器数据实例 */
SensorData_TypeDef g_SensorData = {0};

/* 数据访问互斥量句柄 */
SemaphoreHandle_t g_xDataMutex = NULL;

/**
 * ============================================================================
 * 函数实现
 * ============================================================================
 */

/**
 * @brief 初始化共享数据模块
 * @author Yukikaze
 *
 * @return BaseType_t 初始化结果(pdPASS=成功, pdFAIL=失败)
 *
 * @note 创建互斥量用于保护共享数据
 *       初始化传感器数据结构为默认值
 */
BaseType_t AppData_Init(void)
{
    /* 创建互斥量 */
    g_xDataMutex = xSemaphoreCreateMutex();
    if (g_xDataMutex == NULL)
    {
        return pdFAIL;
    }

    /* 初始化传感器数据 */
    memset(&g_SensorData, 0, sizeof(SensorData_TypeDef));

    return pdPASS;
}

/**
 * @brief 更新温湿度数据(线程安全)
 * @author Yukikaze
 *
 * @param temp 温度值
 * @param humi 湿度值
 * @param valid 数据有效标志
 *
 * @note 使用互斥量保护数据写入
 *       等待时间设置为100ms，超时则放弃更新
 */
void AppData_UpdateTempHum(uint8_t temp, uint8_t humi, uint8_t valid)
{
    if (xSemaphoreTake(g_xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        g_SensorData.temperature = temp;
        g_SensorData.humidity = humi;
        g_SensorData.dht11_valid = valid;
        xSemaphoreGive(g_xDataMutex);
    }
}

/**
 * @brief 更新光照数据(线程安全)
 * @author Yukikaze
 *
 * @param adc_value ADC原始值
 * @param valid 数据有效标志
 *
 * @note 使用互斥量保护数据写入
 *       等待时间设置为100ms，超时则放弃更新
 */
void AppData_UpdateLight(uint32_t adc_value, uint8_t valid)
{
    if (xSemaphoreTake(g_xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        g_SensorData.light_adc = adc_value;
        g_SensorData.light_valid = valid;
        xSemaphoreGive(g_xDataMutex);
    }
}

/**
 * @brief 获取传感器数据副本(线程安全)
 * @author Yukikaze
 *
 * @param pData 指向数据结构的指针，用于存储读取的数据副本
 *
 * @note 使用互斥量保护数据读取
 *       返回数据的完整副本，避免读取时数据被修改
 */
void AppData_GetSensorData(SensorData_TypeDef *pData)
{
    if (pData == NULL)
    {
        return;
    }

    if (xSemaphoreTake(g_xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        memcpy(pData, &g_SensorData, sizeof(SensorData_TypeDef));
        xSemaphoreGive(g_xDataMutex);
    }
}
