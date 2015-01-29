/**
 *	Keil project example for FFT on STM32F4 device.
 *
 *	Works on STM32F429-Discovery board because it has LCD
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 *	Notes: 
 *		- Under "Options for target" > "C/C++" > "Define" you must add 2 defines (I've already add them):
 *			- ARM_MATH_CM4
 *			- __FPU_PRESENT=1
 */
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
/* FFT settings */
#define SAMPLES					8192			/* 256 real party and 256 imaginary parts */
#define FFT_SIZE				SAMPLES / 2		/* FFT size is always the same size as we have samples, so 256 in our case */

#define FFT_BAR_MAX_HEIGHT		100 			/* 120 px on the LCD */
 
/* Global variables */
float32_t Input[SAMPLES];
float32_t Output[FFT_SIZE];
float32_t Output_average[256];

/* Draw bar for LCD */
/* Simple library to draw bars */
void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background) {
	uint16_t height;
	height = (uint16_t)((float32_t)value / (float32_t)maxValue * (float32_t)maxHeight);
	if (height == maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
	} else if (height < maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
		TM_ILI9341_DrawLine(bottomX, bottomY - height, bottomX, bottomY - maxHeight, background);
	}
}

int main(void) {
	arm_cfft_radix4_instance_f32 S;	/* ARM CFFT module */
	float32_t maxValue;				/* Max FFT value is stored here */
	uint32_t maxIndex;				/* Index in Output array where max value is */
	/*FOR AVERAGE DATA*/
	float32_t avgmaxValue;				
	uint32_t avgmaxIndex;	
	uint16_t i;
	
	// /* Initialize system */
	SystemInit();
	
	// /* Delay init */
	TM_DELAY_Init();
	
	// /* Initialize LED's on board */
	TM_DISCO_LedInit();
	
	// /* Initialize LCD */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
	
	/* Initialize DAC2, PA5 for fake sinus, use TIM4 to generate DMA */
	// TM_DAC_SIGNAL_Init(TM_DAC2, TIM4);
	
	/* Set sinus with 10kHz */
	// TM_DAC_SIGNAL_SetSignal(TM_DAC2, TM_DAC_SIGNAL_Signal_Sinus, 10000);

	/* Initialize ADC, PA0 is used */
	// TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Print on LCD */
	TM_ILI9341_Puts(10, 10, "Vibration FFT Graphic", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);
		
	GPIO_Configuration();
	LED_Initialization();
	USART1_Configuration(); 
	Timer4_Initialization();
	NVIC_configuration();
	MPU9250_Config(SPI1);
  	MPU9250_Init(SPI1);
	
	while (1) {
		/* This part should be done with DMA and timer for ADC treshold */
		/* Actually, best solution is double buffered DMA with timer for ADC treshold */
		/* But this is only for show principle on how FFT works */
		// for (i = 0; i < SAMPLES; i += 2) {
		// 	/* Each 22us ~ 45kHz sample rate */
		// 	while (TM_DELAY_Time() < 22);
		// 	TM_DELAY_SetTime(0);
		// 	GPIO_ToggleBits(GPIOA,GPIO_Pin_1);
		// 	 Real part, must be between -1 and 1 
		// 	Input[(uint16_t)i] = (float32_t)((float32_t)TM_ADC_Read(ADC1, ADC_Channel_0) - (float32_t)2048.0) / (float32_t)2048.0;
		// 	/* Imaginary part */
		// 	Input[(uint16_t)(i + 1)] = 0;
		// }
		if(colection_flag == RESET){
			GPIO_ToggleBits(GPIOG,GPIO_Pin_14);
			/* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
			arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
		
			/* Process the data through the CFFT/CIFFT module */
			arm_cfft_radix4_f32(&S, collect_buff);
		
			/* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
			arm_cmplx_mag_f32(collect_buff, Output, FFT_SIZE);
		
			/* Calculates maxValue and returns corresponding value */
			arm_max_f32(Output, FFT_SIZE, &maxValue, &maxIndex);

			/*Get average data of sample rate at 4096/2 -> 256 to display on LCD*/
			for (i = 0; i < 256; i++) Output_average[i]=(Output[8*i]+Output[8*i+1]+Output[8*i+2]+Output[8*i+3]+Output[8*i+4]+Output[8*i+5]+Output[8*i+6]+Output[8*i+7])/8.0;
			arm_max_f32(Output_average, 256, &avgmaxValue, &avgmaxIndex);
			/* Display data on LCD */
			for (i = 0; i < 256; i++) {
				/* Draw FFT results */
				DrawBar(30 + i,
						220,
						FFT_BAR_MAX_HEIGHT,
						(uint16_t)avgmaxValue,
						(float32_t)Output_average[(uint16_t)i],
						0x1234,
						0xFFFF
				);
			}
			/* We want to turn led ON only when low frequencies are active */
			/* Output[0] = Signals DC value */
			// if ((Output[1] + Output[2] + Output[3] + Output[4] + Output[5] + Output[6] + Output[7]) > 120) {
			// 	TM_DISCO_LedOn(LED_GREEN);
			// } else {
			// 	TM_DISCO_LedOff(LED_GREEN);
			// }

		colection_flag = SET;
		}
	}
}
