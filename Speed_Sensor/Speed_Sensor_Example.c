/**
 * Speed_Sensor_Example.c
 *
 * Example usage of the Speed Sensor driver
 *
 * Created on: June 6, 2025
 * Author    : Mahmoud Mohaemd

 */

#include <stdio.h>
#include "Speed_Sensor.h"
#include "Usart.h"

void Speed_Sensor_BasicExample(void)
{
    uint32 period_us;
    uint32 frequency_hz;
    uint8 result;
    char message[100];

    /* Initialize speed sensor */
    result = Speed_Sensor_Init();
    if (result != SPEED_SENSOR_OK) {
        sprintf(message, "Speed Sensor initialization failed! Error: %d\r\n",
                result);
        // Send message via UART (implementation depends on your UART driver)
        return;
    }

    sprintf(message, "Speed Sensor initialized successfully!\r\n");
    // Send message via UART

    /* Measure period */
    result = Speed_Sensor_MeasurePeriod(&period_us);
    if (result == SPEED_SENSOR_OK) {
        /* Calculate frequency */
        Speed_Sensor_CalculateFrequency(period_us, &frequency_hz);

        sprintf(message, "Period: %lu us, Frequency: %lu Hz\r\n", period_us,
                frequency_hz);
        // Send message via UART
    } else if (result == SPEED_SENSOR_TIMEOUT) {
        sprintf(message, "Measurement timeout - no signal detected\r\n");
        // Send message via UART
    } else {
        sprintf(message, "Measurement error: %d\r\n", result);
        // Send message via UART
    }
}

void Speed_Sensor_AverageExample(void)
{
    uint32 period_us;
    uint32 frequency_hz;
    uint32 rpm;
    uint8 result;
    char message[100];

    /* Initialize speed sensor */
    if (Speed_Sensor_Init() != SPEED_SENSOR_OK) {
        return;
    }

    /* Measure average period over multiple samples */
    result = Speed_Sensor_MeasureAveragePeriod(&period_us);
    if (result == SPEED_SENSOR_OK) {
        /* Calculate frequency and RPM (assuming 1 pulse per revolution) */
        Speed_Sensor_CalculateFrequency(period_us, &frequency_hz);
        Speed_Sensor_CalculateRPM(frequency_hz, 1, &rpm);

        sprintf(message, "Average - Period: %lu us, Freq: %lu Hz, RPM: %lu\r\n",
                period_us, frequency_hz, rpm);
        // Send message via UART
    } else {
        sprintf(message, "Average measurement failed: %d\r\n", result);
        // Send message via UART
    }
}

void Speed_Sensor_ContinuousExample(void)
{
    uint32 period_us;
    uint32 frequency_hz;
    uint8 result;
    char message[100];

    /* Initialize speed sensor */
    if (Speed_Sensor_Init() != SPEED_SENSOR_OK) {
        return;
    }

    sprintf(message, "Starting continuous measurements...\r\n");
    // Send message via UART

    /* Continuous measurement loop */
    while (1) {
        result = Speed_Sensor_MeasurePeriod(&period_us);
        if (result == SPEED_SENSOR_OK) {
            Speed_Sensor_CalculateFrequency(period_us, &frequency_hz);
            sprintf(message, "Period: %lu us, Freq: %lu Hz\r\n", period_us,
                    frequency_hz);
            // Send message via UART
        } else if (result == SPEED_SENSOR_TIMEOUT) {
            sprintf(message, "No signal detected\r\n");
            // Send message via UART
        }

        /* Add some delay between measurements if needed */
        // Delay_ms(100);  // Uncomment if you have a delay function
    }
}
