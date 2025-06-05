#include <Std_Types.h>
#include "Gpio.h"

#include <sys/types.h>

#include "Utils.h"
#include "Gpio_Private.h"

#define GPIO_REG(PORT_ID, REG_ID)  ((volatile uint32*) ((PORT_ID) + (REG_ID)))
#define REG32(ADD)                  ((volatile uint32 *)  (ADD))

uint32 addressMap[5] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR,
    GPIOC_BASE_ADDR, GPIOD_BASE_ADDR, GPIOE_BASE_ADDR};

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    uint8 addressIndex = PortName - GPIO_A;

    volatile uint32* moderReg = GPIO_REG(addressMap[addressIndex], GPIOx_MODER);
    volatile uint32* pullUpDownReg = GPIO_REG(addressMap[addressIndex], GPIOx_PUPDR);
    volatile uint32*  outputTypeReg =  GPIO_REG(addressMap[addressIndex], GPIOx_OTYPER);

    *moderReg &= ~(0x03 << (PinNumber * 2));
    *moderReg |=  (PinMode << (PinNumber * 2));

    if (PinMode == GPIO_INPUT) {
        *pullUpDownReg &= ~(0x03 << (PinNumber * 2));
        *pullUpDownReg |=  (DefaultState << (PinNumber * 2));
    }
    else {
        *outputTypeReg &= ~(0x03 << (PinNumber * 2));
        *outputTypeReg |=  (DefaultState << (PinNumber * 2));
    }
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    uint8 status = NOK;
    uint8 addressIndex = PortName - GPIO_A;
    volatile uint32* odr = GPIO_REG(addressMap[addressIndex], GPIOx_ODR);
    // volatile uint32* moderReg = GPIO_REG(addressMap[addressIndex], GPIOx_MODER);

    // if ((*moderReg & (0x03 << (PinNumber * 2)) >> (PinNumber * 2)) != GPIO_INPUT) {
        *odr &= ~(0x1 << PinNumber);
        *odr |= (Data << PinNumber);

        status = OK;
    // }
    return status;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber) {
    uint8 data = 0;
    uint8 addressIndex = PortName - GPIO_A;
    volatile uint32* idr = GPIO_REG(addressMap[addressIndex], GPIOx_IDR);
    data = (*idr & (0x1 << PinNumber)) >> PinNumber;
    return data;
}
