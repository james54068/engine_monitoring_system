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
  initial_AccGyro(1);
  Delay_us(5000000);
  initial_AccGyro(2);
  Delay_us(5000000);
  NVIC_configuration();

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

  while (1)
  {
      

  }
  
}


/************************ (C) COPYRIGHT 071lab *****END OF FILE****/
