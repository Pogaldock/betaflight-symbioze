#include <stdint.h>

#include <platform.h>
#include "drivers/io.h"
#include "drivers/dma.h"
#include "drivers/timer.h"
#include "drivers/stm32/timer_def.h"

const timerHardware_t timerHardware[USABLE_TIMER_CHANNEL_COUNT] = {
    DEF_TIM(TIM4, CH4, PB9, TIMER_OUTPUT_NONE, 0),
    DEF_TIM(TIM4, CH2, PB7, TIMER_OUTPUT_NONE, 0),
    DEF_TIM(TIM4, CH1, PB6, TIMER_OUTPUT_NONE, 0),
    DEF_TIM(TIM4, CH3, PB8, TIMER_OUTPUT_NONE, 0),
    DEF_TIM(TIM5, CH1, PA0, TIMER_OUTPUT_NONE, 0),
};
