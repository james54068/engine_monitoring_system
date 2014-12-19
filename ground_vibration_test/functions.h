#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "stm32f4xx.h"
#include "mpu6500.h"

extern __IO int TimingDelay;
extern int16_t 	acc1_offset[3];
extern int16_t 	acc2_offset[3];

void Delay_us(int nTime);
void lcd_int(void);
void mpu6500_int(void);
void initial_AccGyro(int8_t n);
s16 MoveAve_WMA(volatile int16_t NewData, volatile int16_t *MoveAve_FIFO, u8 SampleNum);

#endif