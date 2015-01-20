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

  // lcd_int();   
  MCU_initialization();
  mpu6500_int();
  NVIC_configuration();
  
/*  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetFont(&Font8x12);
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE); 
  //Circle_Meter(120,160,100,100,-100);
  Semi_Circle_Meter(120,80,70,100,0);
  Semi_Circle_Meter(120,170,70,10000,0);
  XY_axis(20,300,100,0,100,0);*/

  while (1)
  {
    // USART1_puts(0xAA);
/*      float T,R = 0;
      sprintf(IMU2," %d ",temperature);
      LCD_SetLayer(LCD_FOREGROUND_LAYER);
      LCD_SetFont(&Font8x8);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
      LCD_DisplayStringLine(LINE(0), (uint8_t*)IMU2);
      memset(IMU2,0x00,10);  
      T = (float)temperature;
      Draw_SemiCircleNeedle(120,80,45,100,0,T);
      LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
      Draw_SemiCircleNeedle(120,80,45,100,0,T);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
      sprintf(rpm_buff," %d ",rpm); 
      LCD_DisplayStringLine(LINE(13), (uint8_t*)rpm_buff);
      memset(rpm_buff,0x00,10);    
      R = (float)rpm;
      Draw_SemiCircleNeedle(120,170,45,10000,0,R);
      LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
      Draw_SemiCircleNeedle(120,170,45,10000,0,R);
      LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);*/

  }
  
}


/************************ (C) COPYRIGHT 071lab *****END OF FILE****/
