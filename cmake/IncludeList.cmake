set(INCLUDE_LIST ${INCLUDE_LIST}
        ${PROJECT_PATH}/STM32-base/startup
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/ARM/inc
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/inc
        ${PROJECT_PATH}/include
        ${PROJECT_PATH}/GPIO
        ${PROJECT_PATH}/Lib
        ${PROJECT_PATH}/Rcc
        ${PROJECT_PATH}/Sevenseg
        ${PROJECT_PATH}/EXTI
        ${PROJECT_PATH}/ADC
        ${PROJECT_PATH}/LCD
        ${PROJECT_PATH}/PWM
        ${PROJECT_PATH}/Speed_Sensor
        ${PROJECT_PATH}/TIM
)

if (USE_HAL)
    set(INCLUDE_LIST ${INCLUDE_LIST} ${PROJECT_PATH}/STM32-base-STM32Cube/HAL/${SERIES_FOLDER}/inc)
endif ()