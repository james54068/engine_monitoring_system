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
#include <string.h>  
#include "main.h"
#include "mpu6500.h"
#include "mcu_setting.h"


int TimingDelay;

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
   
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */      
  MCU_initialization();
  MPU9250_Config(SPI1);
  MPU9250_Init(SPI1);
  MPU9250_Config(SPI4);
  MPU9250_Init(SPI4);
  

  


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
    int8_t    buff_size;
    int16_t   AccelGyroA[7];
    int16_t   AccelGyroB[7];
    int16_t   temperature;
  while (1)
  {
      
      GPIO_ToggleBits(GPIOA,GPIO_Pin_2);
      MPU9250_ReadRegs(SPI1,MPU6500_ACCEL_XOUT_H, mpu6500A_buf, 6);
      MPU9250_ReadRegs(SPI4,MPU6500_ACCEL_XOUT_H, mpu6500B_buf, 8);

      int i=0; 
      for(i=0; i<3; i++) 
      AccelGyroA[i]=((s16)((u16)mpu6500A_buf[2*i] << 8) + mpu6500A_buf[2*i+1]);
      for(i=0; i<4; i++) 
      AccelGyroB[i]=((s16)((u16)mpu6500B_buf[2*i] << 8) + mpu6500B_buf[2*i+1]);
      temperature = (AccelGyroB[3]-21)/333 + 21;
      //printf("%d,%d,%d,\r\n",AccelGyro[0],AccelGyro[1],AccelGyro[2]);
      sprintf(buff,"%d,%d,%d,%d,%d,%d,%d \r\n",AccelGyroA[0],AccelGyroA[1],AccelGyroA[2],AccelGyroB[0],AccelGyroB[1],AccelGyroB[2],temperature);
      buff_size = strlen(buff);
      DMA2_Stream7->NDTR = buff_size ;
      USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
      DMA_Cmd(DMA2_Stream7,ENABLE);         
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
 
      USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE);

       //USART1_puts(buff);
  }
  
}


/************************ (C) COPYRIGHT 071lab *****END OF FILE****/
