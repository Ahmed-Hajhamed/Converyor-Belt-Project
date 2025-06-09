/**
 * Speed_Sensor_Private.h
 *
 * Private definitions and internal functions for Speed Sensor driver
 *
 * Created on: June 6, 2025
 * Author    : Mahmoud Mohamed
 */

#ifndef SPEED_SENSOR_PRIVATE_H_
#define SPEED_SENSOR_PRIVATE_H_

#include "Std_Types.h"
#include "stm32f401xe.h"

/* ========================================================================== */
/*                             PRIVATE DEFINITIONS                           */
/* ========================================================================== */

/* Timer definitions */
#define SPEED_SENSOR_TIM2 TIM2

/* GPIO Alternate Function for TIM2 */
#define GPIO_AF1_TIM2 0x01U

/* Timeout calculation macros */
#define SPEED_SENSOR_TIMEOUT_TICKS(ms)                                         \
    ((ms) * 1000U) /* Convert ms to µs (timer ticks) */

/* Internal state tracking */
typedef struct {
    uint8 initialized;
    uint8 last_status;
    uint32 last_capture;
    uint32 timeout_ticks;
} Speed_Sensor_State_t;

/* ========================================================================== */
/*                           PRIVATE FUNCTION PROTOTYPES                     */
/* ========================================================================== */

/**
 * @brief Configure GPIO PA0 for TIM2_CH1 alternate function
 * @return SPEED_SENSOR_OK if successful, SPEED_SENSOR_ERROR if failed
 */
static uint8 Speed_Sensor_ConfigureGPIO(void);

/**
 * @brief Configure TIM2 for Input Capture mode
 * @return SPEED_SENSOR_OK if successful, SPEED_SENSOR_ERROR if failed
 */
static uint8 Speed_Sensor_ConfigureTimer(void);

/**
 * @brief Wait for capture event with timeout
 * @param timeout_us Timeout in microseconds
 * @return SPEED_SENSOR_OK if captured, SPEED_SENSOR_TIMEOUT if timeout
 */
static uint8 Speed_Sensor_WaitForCapture(uint32 timeout_us);

/**
 * @brief Clear capture flag and get captured value
 * @return Captured timer value
 */
static uint32 Speed_Sensor_GetCaptureValue(void);

/**
 * @brief Check if period measurement is within valid range
 * @param period_us Period in microseconds
 * @return 1 if valid, 0 if invalid
 */
static uint8 Speed_Sensor_IsValidPeriod(uint32 period_us);

/**
 * @brief Calculate elapsed time between two timer captures
 * @param start_capture First capture value
 * @param end_capture Second capture value
 * @return Elapsed time in timer ticks (microseconds)
 */
static uint32 Speed_Sensor_CalculateElapsed(uint32 start_capture,
                                            uint32 end_capture);

#endif /* SPEED_SENSOR_PRIVATE_H_ */
