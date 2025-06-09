/**
 * Speed_Sensor.h
 *
 * Motor Speed Sensor Driver for STM32F401VE
 * Uses Timer Input Capture mode to measure pulse periods from function
 * generator
 *
 * Created on: June 6, 2025
 * Author    : Mahmoud Mohamed
 */

#ifndef SPEED_SENSOR_H_
#define SPEED_SENSOR_H_

#include "Std_Types.h"

/* ========================================================================== */
/*                                 DEFINITIONS                                */
/* ========================================================================== */

/* Speed Sensor Return Status */
#define SPEED_SENSOR_OK (0U)
#define SPEED_SENSOR_ERROR (1U)
#define SPEED_SENSOR_TIMEOUT (2U)
#define SPEED_SENSOR_NO_SIGNAL (3U)

/* Speed Sensor Configuration */
#define SPEED_SENSOR_PORT GPIO_A
#define SPEED_SENSOR_PIN 0U /* PA0 - TIM2_CH1 */
#define SPEED_SENSOR_TIMER TIM2
#define SPEED_SENSOR_CHANNEL 1U

/* Measurement Parameters */
#define SPEED_SENSOR_SAMPLE_COUNT 4U /* Average over 4 pulses */
#define SPEED_SENSOR_TIMEOUT_MS 1000 /* 1 second timeout */
#define SPEED_SENSOR_MIN_PERIOD_US 100U /* Minimum 100µs period (10kHz max) */
#define SPEED_SENSOR_MAX_PERIOD_US 1000000U /* Maximum 1s period (1Hz min) */

/* Clock Configuration */
#define SPEED_SENSOR_SYSCLK_HZ 16000000U /* 16 MHz HSI system clock */
#define SPEED_SENSOR_TIMER_PRESCALER                                           \
    (1U - 1U) /* 1 MHz timer clock (1µs resolution) from 32MHz timer clock */

/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                            */
/* ========================================================================== */

/**
 * @brief Initialize the speed sensor driver
 *
 * Configures:
 * - GPIO PA0 as alternate function for TIM2_CH1
 * - TIM2 in Input Capture mode
 * - Rising edge detection
 * - Timer prescaler for 1µs resolution
 *
 * @return SPEED_SENSOR_OK if successful, SPEED_SENSOR_ERROR if failed
 */
uint8 Speed_Sensor_Init(void);

/**
 * @brief Measure single pulse period
 *
 * Captures time between two consecutive rising edges
 * Uses polling method (no interrupts)
 *
 * @param period_us Pointer to store measured period in microseconds
 * @return SPEED_SENSOR_OK if successful
 *         SPEED_SENSOR_TIMEOUT if no pulse detected within timeout
 *         SPEED_SENSOR_ERROR if measurement failed
 */
uint8 Speed_Sensor_MeasurePeriod(uint32 *period_us);

/**
 * @brief Measure average period over multiple pulses
 *
 * Takes multiple samples and returns the average for better accuracy
 * Filters out readings outside valid range
 *
 * @param period_us Pointer to store average period in microseconds
 * @return SPEED_SENSOR_OK if successful
 *         SPEED_SENSOR_TIMEOUT if insufficient valid samples
 *         SPEED_SENSOR_ERROR if measurement failed
 */
uint8 Speed_Sensor_MeasureAveragePeriod(uint32 *period_us);

/**
 * @brief Calculate frequency from period measurement
 *
 * Converts period in microseconds to frequency in Hz
 *
 * @param period_us Period in microseconds
 * @param frequency_hz Pointer to store calculated frequency in Hz
 * @return SPEED_SENSOR_OK if successful, SPEED_SENSOR_ERROR if invalid period
 */
uint8 Speed_Sensor_CalculateFrequency(uint32 period_us, uint32 *frequency_hz);

/**
 * @brief Calculate RPM from frequency (for motor applications)
 *
 * Converts frequency to RPM based on pulses per revolution
 *
 * @param frequency_hz Frequency in Hz
 * @param pulses_per_rev Number of pulses per motor revolution
 * @param rpm Pointer to store calculated RPM
 * @return SPEED_SENSOR_OK if successful, SPEED_SENSOR_ERROR if invalid
 * parameters
 */
uint8 Speed_Sensor_CalculateRPM(uint32 frequency_hz, uint32 pulses_per_rev,
                                uint32 *rpm);

/**
 * @brief Get last measurement status
 *
 * @return Last operation status
 */
uint8 Speed_Sensor_GetStatus(void);

/**
 * @brief Reset the speed sensor driver
 *
 * Resets timer and clears any pending flags
 *
 * @return SPEED_SENSOR_OK if successful
 */
uint8 Speed_Sensor_Reset(void);

#endif /* SPEED_SENSOR_H_ */
