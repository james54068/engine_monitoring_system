#include "functions.h"
#include "stm32f4xx.h"
#include "mpu6500.h"
#include "mcu_setting.h"
#include "main.h"
#include <stdio.h>

int16_t 	acc1_offset[3];
int16_t 	acc2_offset[3];

volatile int16_t ACC_FIFO[3][256] = {{0}};

#define MagCorrect_time   10000
#define MagCorrect_Ave    10

__IO int TimingDelay;
void Delay_us(int nTime)
{ 
    // 將 nTime 傳給 TimingDelay，之後讓 TimingDelay 遞減
    TimingDelay = nTime;
 
    // 等待 SysTick_Handler() 中斷涵式
    // 把 TimingDelay 減到 0 才跳出迴圈
    // TimingDelay 非零將會一直空轉
    while(TimingDelay != 0);
}

void lcd_int(void)
{
	  /* LCD initialization */
  LCD_Init(); 
  /* LCD Layer initialization */
  LCD_LayerInit();    
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE); 

}

void mpu6500_int(void)
{
	MPU9250_Config(SPI1);
  	MPU9250_Init(SPI1);
  	// MPU9250_Config(SPI4);
  	// MPU9250_Init(SPI4);
  	// initial_AccGyro(1);
  	// Delay_us(5000000);
  	// initial_AccGyro(2);
  	// Delay_us(5000000);
}

void initial_AccGyro(int8_t n)
{
	int16_t 		acc_offset[3];

	int 			i=0;
	switch (n)
	{
		case 1:
		for(i=0;i<MagCorrect_time;i++)
		{

		MPU9250_ReadRegs(SPI1,MPU6500_ACCEL_XOUT_H, mpu6500A_buf, 6);

		int a = 0; 
      	for(a = 0; a< 3; a++) 
      	AccelGyroA[a]=((s16)((u16)mpu6500A_buf[2*a] << 8) + mpu6500A_buf[2*a+1]);

		acc_offset[0] = (s16)MoveAve_WMA((s16)AccelGyroA[0], ACC_FIFO[0], MagCorrect_Ave);
		acc_offset[1] = (s16)MoveAve_WMA((s16)AccelGyroA[1], ACC_FIFO[1], MagCorrect_Ave);
		acc_offset[2] = (s16)MoveAve_WMA((s16)AccelGyroA[2], ACC_FIFO[2], MagCorrect_Ave);
		
		acc_offset[2] = 2048 - (acc_offset[2]);
		

		printf("acc_x,%d,acc_y,%d,acc_z,%d \r\n",acc_offset[0],acc_offset[1],acc_offset[2]);	
		
		}
		acc1_offset[0]	= 	acc_offset[0];	
		acc1_offset[1]	= 	acc_offset[1];
		acc1_offset[2]	= 	acc_offset[2];
		printf("Calibration outcome\r\n");
		printf("acc_x,%d,acc_y,%d,acc_z,%d \r\n",acc_offset[0],acc_offset[1],acc_offset[2]);	
		printf("\r\n");
		break;

		case 2:
		for(i=0;i<MagCorrect_time;i++)
		{

		MPU9250_ReadRegs(SPI4,MPU6500_ACCEL_XOUT_H, mpu6500B_buf, 6);


		int b = 0; 
      	for(b = 0; b < 3; b++) 
      	AccelGyroB[b]=((s16)((u16)mpu6500B_buf[2*b] << 8) + mpu6500B_buf[2*b+1]);

		acc_offset[0] = (s16)MoveAve_WMA((s16)AccelGyroB[0], ACC_FIFO[0], MagCorrect_Ave);
		acc_offset[1] = (s16)MoveAve_WMA((s16)AccelGyroB[1], ACC_FIFO[1], MagCorrect_Ave);
		acc_offset[2] = (s16)MoveAve_WMA((s16)AccelGyroB[2], ACC_FIFO[2], MagCorrect_Ave);

		acc_offset[2] = 2048 - (acc_offset[2]);
		
		printf("acc_x,%d,acc_y,%d,acc_z,%d \r\n",acc_offset[0],acc_offset[1],acc_offset[2]);	
		
		}
		acc2_offset[0]	= 	acc_offset[0];	
		acc2_offset[1]	= 	acc_offset[1];
		acc2_offset[2]	= 	acc_offset[2];
		printf("Calibration outcome\r\n");
		printf("acc_x,%d,acc_y,%d,acc_z,%d \r\n",acc_offset[0],acc_offset[1],acc_offset[2]);	
		printf("\r\n");
		break;

		default:
		USART1_puts("Check IMU connection\r\n");
	}

}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MoveAve_WMA
**功能 : Weighted Moving Average
**輸入 : NewData, MoveAve_FIFO, SampleNum
**輸出 : AveData
**使用 : MoveAve_WMA(NewData, MoveAve_FIFO, SampleNum)
**=====================================================================================================*/
/*=====================================================================================================*/
s16 MoveAve_WMA(volatile int16_t NewData, volatile int16_t *MoveAve_FIFO, u8 SampleNum)
{
	u8 i = 0;
	s16 AveData = 0;
	u16 SampleSum = 0;
	s32 MoveAve_Sum = 0;

	for (i = 0; i < SampleNum - 1; i++)         // 陣列移動
		MoveAve_FIFO[i] = MoveAve_FIFO[i + 1];

	MoveAve_FIFO[SampleNum - 1] = NewData;      // 加入新數據

	for (i = 0; i < SampleNum; i++)             // 求和 & 加權
		MoveAve_Sum += MoveAve_FIFO[i] * (i + 1);

	SampleSum = (SampleNum * (SampleNum + 1)) / 2; // 計算加權除數
	AveData = (s16)(MoveAve_Sum / SampleSum);   // 計算平均值

	return AveData;
}