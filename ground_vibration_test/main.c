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
#include "functions.h"
#include "drawing.h"
int main(void)
{
   
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */   

  lcd_int();   
  MCU_initialization();
  mpu6500_int();
  NVIC_configuration();
  //USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);

  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetFont(&Font8x12);
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE); 
  //Circle_Meter(120,160,100,100,-100);
  Semi_Circle_Meter(120,80,70,100,0);
  Semi_Circle_Meter(120,200,70,10000,0);

  while (1)
  {
      float T,R = 0;
      GPIO_ToggleBits(GPIOA,GPIO_Pin_1);
      sprintf(IMU2," %d ",temperature);
      LCD_SetLayer(LCD_FOREGROUND_LAYER);
      LCD_SetFont(&Font8x8);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LINE(0), (uint8_t*)IMU2);
      memset(IMU2,0x00,10);  
      T = (float)temperature;
      // Draw_CircleNeedle(120,80,60,100,-100,T);
      // LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
      // Draw_CircleNeedle(120,80,60,100,-100,T);
      Draw_SemiCircleNeedle(120,80,45,100,0,T);
      LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
      Draw_SemiCircleNeedle(120,80,45,100,0,T);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);


      sprintf(rpm_buff," %d ",rpm); 
      LCD_DisplayStringLine(LINE(15), (uint8_t*)rpm_buff);
      memset(rpm_buff,0x00,10);    
      R = (float)rpm;
      Draw_SemiCircleNeedle(120,200,45,10000,0,R);
      LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
      Draw_SemiCircleNeedle(120,200,45,10000,0,R);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);

/*      uint16_t rpm=8000;
      GPIO_ToggleBits(GPIOA,GPIO_Pin_2);
      MPU9250_ReadRegs(SPI1,MPU6500_ACCEL_XOUT_H, mpu6500A_buf, 6);
      MPU9250_ReadRegs(SPI4,MPU6500_ACCEL_XOUT_H, mpu6500B_buf, 8);

      int i=0; 
      for(i=0; i<3; i++) 
      AccelGyroA[i]=((s16)((u16)mpu6500A_buf[2*i] << 8) + mpu6500A_buf[2*i+1]);
      for(i=0; i<4; i++) 
      AccelGyroB[i]=((s16)((u16)mpu6500B_buf[2*i] << 8) + mpu6500B_buf[2*i+1]);
      temperature = (AccelGyroB[3]-21)/333 + 21;
      
      AccelGyroA[0] -= acc1_offset[0];
      AccelGyroA[1] -= acc1_offset[1];
      AccelGyroA[2] += acc1_offset[2];
      AccelGyroB[0] -= acc2_offset[0];
      AccelGyroB[1] -= acc2_offset[1];
      AccelGyroB[2] += acc2_offset[2];

      AccelGyroA[0] = - AccelGyroA[0];
      AccelGyroA[1] = - AccelGyroA[1];
      AccelGyroA[2] = - AccelGyroA[2];
      AccelGyroB[0] = - AccelGyroB[0];
      AccelGyroB[1] = - AccelGyroB[1];
      AccelGyroB[2] = - AccelGyroB[2];
    
    
      //printf("%d,%d,%d,\r\n",AccelGyro[0],AccelGyro[1],AccelGyro[2]);
      sprintf(buff,"%d,%d,%d,%d,%d,%d,%d,%d \r\n",AccelGyroA[0],AccelGyroA[1],AccelGyroA[2],AccelGyroB[0],AccelGyroB[1],AccelGyroB[2],temperature,rpm);
      //sprintf(IMU2,"%d \r\n",temperature);
      buff_size = strlen(buff);
      DMA2_Stream7->NDTR = buff_size ;
      USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
      DMA_Cmd(DMA2_Stream7,ENABLE);         
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
 
      USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE);
*/


  }
  
}


/************************ (C) COPYRIGHT 071lab *****END OF FILE****/
