#ifndef __MCU_SETTING_H
#define __MCU_SETTING_H

#include "stm32f4xx.h"

extern u8 	mpu6500A_buf[16];
extern u8	mpu6500B_buf[16];
extern char 	buff[50];

extern    int8_t    buff_size;
extern    __IO	int16_t   AccelGyroA[7];
extern    __IO	int16_t   AccelGyroB[7];
extern    __IO	int8_t    temperature;



void MCU_initialization(void);
void SysTick_cfg(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_configuration(void);
void USART1_Configuration(void);
void Timer4_Initialization(void);
void DMA2_stream0_channel3_init(void);
void DMA2_stream7_channel4_init(void);
void USART1_puts(char* s);
void send_byte(uint8_t b);
int _write (int fd, char *ptr, int len);


#endif

