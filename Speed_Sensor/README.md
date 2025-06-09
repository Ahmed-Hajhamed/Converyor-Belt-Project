# Speed Sensor Driver for STM32F401VE

## Overview

This driver implements a motor speed sensor using Timer Input Capture mode on the STM32F401VE microcontroller. It measures the time between rising edges of pulse signals from a function generator that simulates a motor speed sensor.

## Features

- **Timer Input Capture**: Uses TIM2_CH1 on pin PA0
- **No HAL Abstraction**: Direct register access using stm32f401xe.h
- **No Interrupts**: Polling-based edge detection for simplicity
- **1µs Resolution**: Timer configured for microsecond precision
- **Period Validation**: Configurable min/max period limits
- **Average Measurements**: Multiple sample averaging for accuracy
- **Overflow Handling**: Proper handling of 32-bit timer overflow
- **Frequency/RPM Calculation**: Built-in conversion functions

## Hardware Configuration

- **Pin**: PA0 (TIM2_CH1)
- **Timer**: TIM2 (32-bit timer)
- **Clock**: 42 MHz system clock
- **Prescaler**: 42 (1 MHz timer clock = 1µs resolution)
- **Edge Detection**: Rising edge
- **Input Signal**: Connect function generator to PA0

## API Reference

### Initialization

```c
uint8 Speed_Sensor_Init(void);
```

Initializes the speed sensor driver, configures GPIO and timer.

### Single Measurement

```c
uint8 Speed_Sensor_MeasurePeriod(uint32 *period_us);
```

Measures the period between two consecutive rising edges.

### Average Measurement

```c
uint8 Speed_Sensor_MeasureAveragePeriod(uint32 *period_us);
```

Takes multiple samples and returns the average for better accuracy.

### Utility Functions

```c
uint8 Speed_Sensor_CalculateFrequency(uint32 period_us, uint32 *frequency_hz);
uint8 Speed_Sensor_CalculateRPM(uint32 frequency_hz, uint32 pulses_per_rev, uint32 *rpm);
uint8 Speed_Sensor_GetStatus(void);
uint8 Speed_Sensor_Reset(void);
```

### Return Values

- `SPEED_SENSOR_OK` (0): Success
- `SPEED_SENSOR_ERROR` (1): General error
- `SPEED_SENSOR_TIMEOUT` (2): No signal detected within timeout
- `SPEED_SENSOR_NO_SIGNAL` (3): Insufficient valid samples

## Configuration Parameters

```c
#define SPEED_SENSOR_TIMEOUT_MS             1000U           /* 1 second timeout */
#define SPEED_SENSOR_MIN_PERIOD_US          100U            /* 100µs (10kHz max) */
#define SPEED_SENSOR_MAX_PERIOD_US          1000000U        /* 1s (1Hz min) */
#define SPEED_SENSOR_SAMPLE_COUNT           4U              /* Average over 4 pulses */
```

## Usage Example

```c
#include "Speed_Sensor.h"

int main(void) {
    uint32 period_us, frequency_hz, rpm;
    uint8 result;
    
    /* Initialize the driver */
    if (Speed_Sensor_Init() != SPEED_SENSOR_OK) {
        // Handle initialization error
        return -1;
    }
    
    /* Measure period */
    result = Speed_Sensor_MeasurePeriod(&period_us);
    if (result == SPEED_SENSOR_OK) {
        /* Calculate frequency and RPM */
        Speed_Sensor_CalculateFrequency(period_us, &frequency_hz);
        Speed_Sensor_CalculateRPM(frequency_hz, 1, &rpm);  // 1 pulse per revolution
        
        printf("Period: %lu us, Frequency: %lu Hz, RPM: %lu\n", 
               period_us, frequency_hz, rpm);
    } else {
        printf("Measurement failed: %d\n", result);
    }
    
    return 0;
}
```

## Testing with Function Generator

1. **Connect function generator output to PA0**
2. **Set signal parameters**:
   - Waveform: Square wave or pulse
   - Frequency: 1 Hz to 10 kHz
   - Amplitude: 3.3V (STM32 logic level)
   - Duty cycle: 50% (not critical)

3. **Expected behavior**:
   - 1 kHz signal → 1000 µs period
   - 10 Hz signal → 100,000 µs period
   - Driver should timeout if no signal present

## Integration Notes

- **Dependencies**: Requires existing GPIO and RCC drivers
- **Include Path**: Add Speed_Sensor directory to build system
- **Thread Safety**: Not thread-safe (single measurement at a time)
- **Power**: Timer continues running after initialization

## Limitations

- **Single Channel**: Only TIM2_CH1 supported
- **Polling Only**: No interrupt-driven measurements
- **Resolution**: Limited to 1µs (42 MHz / 42 prescaler)
- **Range**: 100µs to 1s period (1Hz to 10kHz frequency)

## Driver Architecture

The driver follows the existing project patterns:

- **Public API**: Speed_Sensor.h
- **Private definitions**: Speed_Sensor_Private.h  
- **Implementation**: Speed_Sensor.c
- **Examples**: Speed_Sensor_Example.c
- **Documentation**: README.md

Files are automatically included in CMake build system via GLOB_RECURSE.
