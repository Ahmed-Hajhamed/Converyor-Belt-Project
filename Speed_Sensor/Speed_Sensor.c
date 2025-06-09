/**
 * Speed_Sensor.c
 *
 * Motor Speed Sensor Driver Implementation for STM32F401VE
 * Uses Timer Input Capture mode to measure pulse periods from function
 * generator
 *
 * Created on: June 6, 2025
 * Author    : Mahmoud Mohamed
 */

#include "Speed_Sensor.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Speed_Sensor_Private.h"

/* ========================================================================== */
/*                               GLOBAL VARIABLES                            */
/* ========================================================================== */

static Speed_Sensor_State_t sensor_state = {0};

/* ========================================================================== */
/*                           PUBLIC FUNCTION IMPLEMENTATIONS                 */
/* ========================================================================== */

uint8 Speed_Sensor_Init(void)
{
    /* Enable clocks for GPIOA and TIM2 */
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_TIM2);

    /* Configure GPIO PA0 for TIM2_CH1 */
    if (Speed_Sensor_ConfigureGPIO() != SPEED_SENSOR_OK) {
        sensor_state.last_status = SPEED_SENSOR_ERROR;
        return SPEED_SENSOR_ERROR;
    }

    /* Configure TIM2 for Input Capture */
    if (Speed_Sensor_ConfigureTimer() != SPEED_SENSOR_OK) {
        sensor_state.last_status = SPEED_SENSOR_ERROR;
        return SPEED_SENSOR_ERROR;
    }

    /* Initialize state */
    sensor_state.initialized = 1;
    sensor_state.last_status = SPEED_SENSOR_OK;
    sensor_state.last_capture = 0;
    sensor_state.timeout_ticks =
            SPEED_SENSOR_TIMEOUT_TICKS(SPEED_SENSOR_TIMEOUT_MS);

    return SPEED_SENSOR_OK;
}

uint8 Speed_Sensor_MeasurePeriod(uint32 *period_us)
{
    uint32 first_capture, second_capture;
    uint32 elapsed_time;

    /* Input validation */
    if (period_us == (uint32 *) 0 || !sensor_state.initialized) {
        sensor_state.last_status = SPEED_SENSOR_ERROR;
        return SPEED_SENSOR_ERROR;
    }

    /* Wait for first rising edge */
    if (Speed_Sensor_WaitForCapture(sensor_state.timeout_ticks) !=
        SPEED_SENSOR_OK) {
        sensor_state.last_status = SPEED_SENSOR_TIMEOUT;
        return SPEED_SENSOR_TIMEOUT;
    }
    first_capture = Speed_Sensor_GetCaptureValue();

    /* Wait for second rising edge */
    if (Speed_Sensor_WaitForCapture(sensor_state.timeout_ticks) !=
        SPEED_SENSOR_OK) {
        sensor_state.last_status = SPEED_SENSOR_TIMEOUT;
        return SPEED_SENSOR_TIMEOUT;
    }
    second_capture = Speed_Sensor_GetCaptureValue();

    /* Calculate period */
    elapsed_time = Speed_Sensor_CalculateElapsed(first_capture, second_capture);

    /* Validate measurement */
    if (!Speed_Sensor_IsValidPeriod(elapsed_time)) {
        sensor_state.last_status = SPEED_SENSOR_ERROR;
        return SPEED_SENSOR_ERROR;
    }

    *period_us = elapsed_time;
    sensor_state.last_capture = second_capture;
    sensor_state.last_status = SPEED_SENSOR_OK;

    return SPEED_SENSOR_OK;
}

uint8 Speed_Sensor_MeasureAveragePeriod(uint32 *period_us)
{
    uint32 measurements[SPEED_SENSOR_SAMPLE_COUNT];
    uint32 valid_count = 0;
    uint32 sum = 0;
    uint8 i;

    /* Input validation */
    if (period_us == (uint32 *) 0) {
        sensor_state.last_status = SPEED_SENSOR_ERROR;
        return SPEED_SENSOR_ERROR;
    }

    /* Collect multiple measurements */
    for (i = 0; i < SPEED_SENSOR_SAMPLE_COUNT; i++) {
        if (Speed_Sensor_MeasurePeriod(&measurements[i]) == SPEED_SENSOR_OK) {
            sum += measurements[i];
            valid_count++;
        }
    }

    /* Check if we have enough valid measurements */
    if (valid_count < (SPEED_SENSOR_SAMPLE_COUNT / 2)) {
        sensor_state.last_status = SPEED_SENSOR_NO_SIGNAL;
        return SPEED_SENSOR_NO_SIGNAL;
    }

    /* Calculate average */
    *period_us = sum / valid_count;
    sensor_state.last_status = SPEED_SENSOR_OK;

    return SPEED_SENSOR_OK;
}

uint8 Speed_Sensor_CalculateFrequency(uint32 period_us, uint32 *frequency_hz)
{
    /* Input validation */
    if (frequency_hz == (uint32 *) 0 || period_us == 0) {
        return SPEED_SENSOR_ERROR;
    }

    /* Frequency = 1 / Period */
    /* period_us is in microseconds, so frequency = 1,000,000 / period_us */
    *frequency_hz = 1000000UL / period_us;

    return SPEED_SENSOR_OK;
}

uint8 Speed_Sensor_CalculateRPM(uint32 frequency_hz, uint32 pulses_per_rev,
                                uint32 *rpm)
{
    /* Input validation */
    if (rpm == (uint32 *) 0 || pulses_per_rev == 0) {
        return SPEED_SENSOR_ERROR;
    }

    /* RPM = (Frequency * 60) / Pulses_per_revolution */
    *rpm = (frequency_hz * 60UL) / pulses_per_rev;

    return SPEED_SENSOR_OK;
}

uint8 Speed_Sensor_GetStatus(void) { return sensor_state.last_status; }

uint8 Speed_Sensor_Reset(void)
{
    if (!sensor_state.initialized) {
        return SPEED_SENSOR_ERROR;
    }

    /* Reset TIM2 counter */
    SPEED_SENSOR_TIM2->CNT = 0;

    /* Clear any pending flags */
    SPEED_SENSOR_TIM2->SR = 0;

    /* Reset state */
    sensor_state.last_capture = 0;
    sensor_state.last_status = SPEED_SENSOR_OK;

    return SPEED_SENSOR_OK;
}

/* ========================================================================== */
/*                           PRIVATE FUNCTION IMPLEMENTATIONS                */
/* ========================================================================== */

static uint8 Speed_Sensor_ConfigureGPIO(void)
{
    /* Configure PA0 as alternate function for TIM2_CH1 */
    // if (Gpio_Init(SPEED_SENSOR_PORT, SPEED_SENSOR_PIN, GPIO_AF,
    //               GPIO_PUSH_PULL) != OK) {
    //     return SPEED_SENSOR_ERROR;
    // }

    /* Set alternate function to TIM2 (AF1) on PA0 */
    /* Clear the AFSEL0 bits first */
    GPIOA->AFR[0] &= ~(0xF << (SPEED_SENSOR_PIN * 4));
    /* Set AF1 for TIM2 */
    GPIOA->AFR[0] |= (GPIO_AF1_TIM2 << (SPEED_SENSOR_PIN * 4));

    return SPEED_SENSOR_OK;
}

static uint8 Speed_Sensor_ConfigureTimer(void)
{
    /* Reset TIM2 configuration */
    SPEED_SENSOR_TIM2->CR1 = 0;
    SPEED_SENSOR_TIM2->CR2 = 0;
    SPEED_SENSOR_TIM2->SMCR = 0;
    SPEED_SENSOR_TIM2->DIER = 0;
    SPEED_SENSOR_TIM2->SR = 0;
    SPEED_SENSOR_TIM2->CCMR1 = 0;
    SPEED_SENSOR_TIM2->CCMR2 = 0;
    SPEED_SENSOR_TIM2->CCER = 0;

    /* Set prescaler for 1µs resolution (32MHz / 32 = 1MHz) */
    SPEED_SENSOR_TIM2->PSC = SPEED_SENSOR_TIMER_PRESCALER;

    /* Set auto-reload to maximum (32-bit timer) */
    SPEED_SENSOR_TIM2->ARR = 0xFFFFFFFF;

    /* Configure Channel 1 for Input Capture */
    /* CC1S = 01: CC1 channel is configured as input, IC1 is mapped on TI1 */
    SPEED_SENSOR_TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;

    /* Configure capture on rising edge (CC1P = 0, default) */
    SPEED_SENSOR_TIM2->CCER &= ~TIM_CCER_CC1P;

    /* Enable Channel 1 capture */
    SPEED_SENSOR_TIM2->CCER |= TIM_CCER_CC1E;

    /* Enable timer */
    SPEED_SENSOR_TIM2->CR1 |= TIM_CR1_CEN;

    return SPEED_SENSOR_OK;
}

static uint8 Speed_Sensor_WaitForCapture(uint32 timeout_us)
{
    uint32 start_time = SPEED_SENSOR_TIM2->CNT;
    uint32 current_time;

    /* Clear any existing capture flag */
    SPEED_SENSOR_TIM2->SR &= ~TIM_SR_CC1IF;

    /* Wait for capture event or timeout */
    while (!(SPEED_SENSOR_TIM2->SR & TIM_SR_CC1IF)) {
        current_time = SPEED_SENSOR_TIM2->CNT;

        /* Check for timeout (handle counter overflow) */
        if (current_time >= start_time) {
            if ((current_time - start_time) > timeout_us) {
                return SPEED_SENSOR_TIMEOUT;
            }
        } else {
            /* Counter overflowed */
            if ((0xFFFFFFFF - start_time + current_time) > timeout_us) {
                return SPEED_SENSOR_TIMEOUT;
            }
        }
    }

    return SPEED_SENSOR_OK;
}

static uint32 Speed_Sensor_GetCaptureValue(void)
{
    uint32 capture_value;

    /* Read captured value */
    capture_value = SPEED_SENSOR_TIM2->CCR1;

    /* Clear capture flag */
    SPEED_SENSOR_TIM2->SR &= ~TIM_SR_CC1IF;

    return capture_value;
}

static uint8 Speed_Sensor_IsValidPeriod(uint32 period_us)
{
    return (period_us >= SPEED_SENSOR_MIN_PERIOD_US &&
            period_us <= SPEED_SENSOR_MAX_PERIOD_US)
                   ? 1
                   : 0;
}

static uint32 Speed_Sensor_CalculateElapsed(uint32 start_capture,
                                            uint32 end_capture)
{
    /* Handle timer overflow */
    if (end_capture >= start_capture) {
        return (end_capture - start_capture);
    } else {
        /* Timer overflowed */
        return (0xFFFFFFFF - start_capture + end_capture + 1);
    }
}
