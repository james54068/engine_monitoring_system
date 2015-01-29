/**
  ******************************************************************************
  * @file    Touch_Panel/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "main.h"
#include "mpu6500.h"
#include "mcu_setting.h"
#include "functions.h"
#include <string.h> 

#include "tm_stm32f4_delay.h"
uint8_t buff_len = 0;
FlagStatus colection_flag = SET;
float collect_buff[3][8192]; 
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup Touch_Panel
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
// void SysTick_Handler(void)
// {  
//     if (TimingDelay != 0)
//     { 
//         TimingDelay--;
//     }
// }

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).  */
/******************************************************************************/

void DMA2_Stream0_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA2_Stream0,DMA_IT_TCIF0)==SET)
  {
    DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);
    DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
    DMA_Cmd(DMA2_Stream0,DISABLE);  
  }
  
}

void DMA2_Stream7_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA2_Stream7,DMA_IT_TCIF7)==SET)
  {
    GPIO_ToggleBits(GPIOA,GPIO_Pin_1);
    DMA_Cmd(DMA2_Stream7,DISABLE);
    // DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
    DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7); 
  }  
}
int timestamp=0;
char words[20];
int i,j=0; 
void TIM4_IRQHandler()
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
      
      // USART1_puts("123\r\n");
      MPU9250_ReadRegs(SPI1,MPU6500_ACCEL_XOUT_H, mpu6500A_buf, 14);

      GPIO_ToggleBits(GPIOA,GPIO_Pin_2);
      for(i=0; i<7; i++) 
      AccelGyroA[i]=((s16)((u16)mpu6500A_buf[2*i] << 8) + mpu6500A_buf[2*i+1]);

      if(colection_flag && j<8192){
        if(j%2){
          collect_buff[0][j]=0.0;
          collect_buff[1][j]=0.0;
          collect_buff[2][j]=0.0;
        }
        else{
          collect_buff[0][j] = AccelGyroA[0]/2048.0;
          collect_buff[1][j] = AccelGyroA[1]/2048.0;
          collect_buff[2][j] = AccelGyroA[2]/2048.0;
        } 
        j++;
      }else{
        j=0;
        colection_flag = RESET;
        GPIO_ToggleBits(GPIOG,GPIO_Pin_13);
      } 
      sprintf(buff,"%d,%d,%d,%d,%d,%d\r\n",AccelGyroA[0],AccelGyroA[1],AccelGyroA[2],AccelGyroA[4],AccelGyroA[5],AccelGyroA[6]);
      USART1_puts(buff);

      // for(i=0;i<6;i++) {
      //   buff[i]=mpu6500A_buf[i];
      // }
      // buff[6]='A';
      // buff[7]='B';
      // buff[8]='C';
      // buff[9]='D';

      // sprintf(buff,"%d\r\n",rpm);
      // // DMA2_Stream7->NDTR=strlen(buff);
      // // DMA_Cmd(DMA2_Stream7,ENABLE);
      // // DMA2_stream7_channel4_init();
      // USART1_puts(buff);
     //  timestamp++;
     // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
     //    USART_SendData(USART1, timestamp);

      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, 'A');
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, 'B');
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, 'C');
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, 'D');
    
    
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[5]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[4]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[3]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[2]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[1]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[0]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[6]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[7]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[8]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[9]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[10]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[11]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[12]);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1, mpu6500A_buf[13]);

      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1,(u8)rpm);
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
      //   USART_SendData(USART1,(u8)(rpm>>8));
     
      // USART1_puts(0x33);
     


      // buff_size = strlen(buff);
      // DMA2_Stream7->NDTR = buff_size ;
      // USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
      // DMA_Cmd(DMA2_Stream7,ENABLE);         
      // while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
 
      // USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE);
    


       //USART1_puts(buff);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  } 
}


void TIM3_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
  {
     TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
     rpm = 0;
   
  }
  else if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
  {
  uint32_t IC2Value;
  GPIO_ToggleBits(GPIOC,GPIO_Pin_8); 
  //RCC_GetClocksFreq(&RCC_Clocks);  
  
  /* Clear TIM2 Capture compare interrupt pending bit */  
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 

  /* Get the Input Capture value */  
  IC2Value = TIM_GetCapture1(TIM3);

  // sprintf(buff,"%d\r\n",IC2Value);
  // USART1_puts(buff);

  rpm = (100000/IC2Value)*60;
  TIM_SetCounter(TIM3,0);
  // IC2Value = 0;

  // TIM_SetAutoreload(TIM3,0);
  //sprintf(rpm_buff,"%d",rpm);
  // USART1_puts(rpm_buff);
  }

} 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
