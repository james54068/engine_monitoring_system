/**
  ******************************************************************************
  * @file    Touch_Panel/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to configure and use the touch panel 
  *          mounted on STM32F429I-DISCO boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mpu6500.h"
#include "mcu_setting.h"

int TimingDelay;

 void SysTick_cfg(void)
{
  if (SysTick_Config(SystemCoreClock/1000000))
  { 
    /* Capture error */ 
    while (1);
  }
} 


void Delay_us(int nTime)
{ 
    // 將 nTime 傳給 TimingDelay，之後讓 TimingDelay 遞減
    TimingDelay = nTime;
 
    // 等待 SysTick_Handler() 中斷涵式
    // 把 TimingDelay 減到 0 才跳出迴圈
    // TimingDelay 非零將會一直空轉
    while(TimingDelay != 0);
}

int main(void)
{

  //char lcd_text_buff[100];
  //uint32_t test_int32 =0;
    
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */      
  MCU_initialization();
  SysTick_cfg();
  MPU9250_Config(SPI1);
  MPU9250_Init(SPI1);
  DMA2_stream0_channel3_init();
  SPI_I2S_DMACmd(SPI1,SPI_DMAReq_Rx,ENABLE);


  /* LCD initialization */
  //LCD_Init();
  
  /* LCD Layer initialization */
  //LCD_LayerInit();
    
  /* Enable the LTDC */
  //LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  //LCD_SetLayer(LCD_FOREGROUND_LAYER);
  
  /* Clear the LCD */ 
/*
  LCD_SetFont(&Font8x12);
  LCD_DisplayStringLine(LINE(1), (uint8_t*)" LCD text print example ");
  LCD_DisplayStringLine(LINE(2), (uint8_t*)" Ming6842 @ github");
  LCD_DisplayStringLine(LINE(3), (uint8_t*)" -------------------");
  LCD_DisplayStringLine(LINE(4), (uint8_t*)" !@#$%%^&*()_+)(*&^%%$#$%%^&*");
*/
u8        k = 0;
u8        ReadBuf[14];
int16_t   AccelGyro[7];
  while (1)
  {
 
      //k = DMA_GetCmdStatus(DMA2_Stream0);
      //printf("%d\r\n",k);
      DMA_Cmd(DMA2_Stream0,ENABLE);
      
      GPIO_ResetBits(GPIOA,GPIO_Pin_4);
      SPIx_WriteByte(SPI1, 0x80 | MPU6500_ACCEL_XOUT_H); 
      
      for(k=0;k<16;k++)
      {   
          SPIx_WriteByte(SPI1,0xFF);
      }
 
      GPIO_SetBits(GPIOA,GPIO_Pin_4);

      int i=0; 
      for(i=1; i<4; i++) 
      AccelGyro[i]=((s16)((u16)mpu6500_buf[2*i] << 8) + mpu6500_buf[2*i+1]);
      /* Get Angular rate */
      for(i=5; i<8; i++)
      AccelGyro[i-1]=((s16)((u16)mpu6500_buf[2*i] << 8) + mpu6500_buf[2*i+1]);
      //printf("%d,%d,%d,%d,%d,%d\r\n",AccelGyro[0],AccelGyro[1],AccelGyro[2],AccelGyro[3],AccelGyro[4],AccelGyro[5]);
    printf("%d,%d,%d,%d,%d,%d\r\n",AccelGyro[1],AccelGyro[2],AccelGyro[3],AccelGyro[4],AccelGyro[5],AccelGyro[6]);
  }
  
}


/************************ (C) COPYRIGHT 071lab *****END OF FILE****/
