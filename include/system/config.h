#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define MCU_08_BIT_DATA         1
#define MCU_16_BIT_DATA         0
#define MCU_08_BIT_ADDRESS      1
#define MCU_16_BIT_ADDRESS      0

#if ((MCU_08_BIT_DATA) & (MCU_08_BIT_ADDRESS))
typedef struct {
    uint8_t value;
    uint8_t address;
} AddressValue, *AddressValuePtr;
#elif ((MCU_08_BIT_DATA) & (MCU_16_BIT_ADDRESS))
typedef struct {
    uint8_t value;
    uint16_t data;
} AddressValue, *AddressValuePtr;
#elif ((MCU_16_BIT_DATA) & (MCU_16_BIT_ADDRESS))
typedef struct {
    uint8_t value;
    uint16_t data;
} AddressValue, *AddressValuePtr;
#endif


#endif // CONFIG_H
