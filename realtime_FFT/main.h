#ifndef __MAIN_H
#define __MAIN_H

/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_dac_signal.h"

#include <stdio.h>

/* Include arm_math.h mathematic functions */
#include "arm_math.h"

#include "mcu_setting.h"
#include "stm32f4xx_it.h"
#include "mpu6500.h"
/* FFT settings */
#define SAMPLES					8192			/* 256 real party and 256 imaginary parts */
#define FFT_SIZE				SAMPLES / 2		/* FFT size is always the same size as we have samples, so 256 in our case */

#define FFT_BAR_MAX_HEIGHT		60 			/* 120 px on the LCD */

void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background);



#endif