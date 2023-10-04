#ifndef __CriticalSection_h__
#define __CriticalSection_h__

#include "stm32f411xe.h"

class CriticalSection
{
public:
    static constexpr uint32_t coeffcient = 5;
    static inline uint32_t __delay_coefficient;

    inline CriticalSection()
    {
        __disable_irq();
    }

    inline ~CriticalSection()
    {
        __enable_irq();
    }

    static inline uint32_t __update_delay_coefficient()
    {
        __delay_coefficient = (SystemCoreClock / 1000000UL - 24) / 60 * coeffcient;
        return __delay_coefficient;
    }

    static inline void __attribute__((optimize("O0"))) __delay_us(uint16_t us)
    {
        uint32_t loops = __delay_coefficient * us;
        while (--loops)
            ;
    }
};

#endif
