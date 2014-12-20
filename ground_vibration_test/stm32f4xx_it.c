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
#include "main.h"
#include "mpu6500.h"
#include "mcu_setting.h"
#include "functions.h"
#include <string.h> 
    
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
void SysTick_Handler(void)
{  
    if (TimingDelay != 0)
    { 
        TimingDelay--;
    }
}

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
    //GPIO_ToggleBits(GPIOA,GPIO_Pin_2);
    DMA_Cmd(DMA2_Stream7,DISABLE);
    //DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
    DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7); 
  }  
}

void TIM4_IRQHandler()
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
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
      USART1_puts(buff);
     


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


void TIM2_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
     TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
     rpm = 0;
  }

  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
  uint32_t IC2Value;

  //RCC_GetClocksFreq(&RCC_Clocks);  
  
  /* Clear TIM2 Capture compare interrupt pending bit */  
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);  

  /* Get the Input Capture value */  
  IC2Value = TIM_GetCapture1(TIM2);

  rpm = (1000000/IC2Value)*60;
  sprintf(rpm_buff,"%d",rpm);
  // USART1_puts(rpm_buff);
  }
  

  // else
  // {
  //   rpm = 0;
  // }  
} 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
