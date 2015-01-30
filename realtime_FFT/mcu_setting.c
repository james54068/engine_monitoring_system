#include <string.h>
#include "mcu_setting.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

u8        mpu6500A_buf[16];
u8        mpu6500B_buf[16];
char      buff[50];

int8_t    buff_size;
__IO  int16_t   AccelGyroA[7];
__IO  int16_t   AccelGyroB[7];
__IO  int8_t    temperature;
__IO  uint16_t  rpm = 0;

char      IMU2[10];
char      rpm_buff[10];

void MCU_initialization(void)
{
    SysTick_cfg();
    GPIO_Configuration();
    USART1_Configuration();  
    Timer4_Initialization();
    Timer3_Initialization();
    Timer3_Channel_init();
    // DMA2_stream7_channel4_init();
  
}

 void SysTick_cfg(void)
{
  if (SysTick_Config(SystemCoreClock/1000000))
  { 
    /* Capture error */ 
    while (1);
  }
} 


void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      
}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
    /* GPIOA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration for Push Button ----------------------------*/
    /*Toggle output*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*check connection*/
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX

}

void LED_Initialization(void){

  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE); //LED3/4 GPIO Port

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED is connected to PG13/PG14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void NVIC_configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*preemption:2 sub:8*/

  /*DMA2 Stream7 Interrupt */
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream7_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /*TIM4 global Interrupt */
  NVIC_InitStruct.NVIC_IRQChannel =  TIM4_IRQn ;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;  
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStruct);  

}


void USART1_Configuration(void)
{
    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 115200 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 921600;//921600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void Timer4_Initialization(void)
{

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM4);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 2500 - 1 ;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 9 - 1; // Prescaled by 1800 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Down;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
}

void Timer3_Initialization(void)
{

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* -- Timer Configuration --------------------------------------------------- */
  TIM_DeInit(TIM2);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_TimeBaseStruct.TIM_Period = 0xFFFF ;  //250ms  --> 4Hz
  TIM_TimeBaseStruct.TIM_Prescaler = 90 - 1; // Prescaled by 1800 -> = 0.1M(10us)
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; // Div by one -> 90 MHz (Now RCC_DCKCFGR_TIMPRE is configured to divide clock by two)
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
  //TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
}

void Timer3_Channel_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* TIM2 chennel1 configuration : PA.00 */  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;  
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
  /* Connect TIM pin to AF1 */  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /*TIM input capture structure*/
  TIM_ICInitTypeDef TIM_ICInitStructure;

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  
  TIM_ICInitStructure.TIM_ICFilter = 0x0;  
  
  TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);  
  
  /* Select the TIM2 Input Trigger: TI2FP2 */  
  TIM_SelectInputTrigger(TIM2, TIM_TS_ETRF);  
  
  /* Select the slave Mode: Reset Mode */  
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);  
  TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  
  
  /* TIM enable counter */  
  TIM_Cmd(TIM2, ENABLE);  
  
  /* Enable the CC2 Interrupt Request */  
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);  

}

void DMA2_stream0_channel3_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  /* GPIOA clock enable */

  DMA_DeInit(DMA2_Stream0);
  DMA_InitTypeDef DMA_InitStruct;
  DMA_StructInit(&DMA_InitStruct);

  DMA_InitStruct.DMA_Channel = DMA_Channel_3;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)mpu6500A_buf;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = 16;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA2_Stream0,&DMA_InitStruct);
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);

}

void DMA2_stream7_channel4_init(void)
{
  /* GPIOA clock enable */

  DMA_DeInit(DMA2_Stream7);
  DMA_InitTypeDef DMA_InitStruct;
  DMA_StructInit(&DMA_InitStruct);

  DMA_InitStruct.DMA_Channel = DMA_Channel_4;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&buff;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = 18;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DMA2_Stream7,&DMA_InitStruct);
  DMA_Cmd(DMA2_Stream7,ENABLE);
  DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
}


void USART1_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s);
        s++;
    }
}

void send_byte(uint8_t b)
{
  /* Send one byte */
  USART_SendData(USART1, b);

  /* Loop until USART2 DR register is empty */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// int _write (int fd, char *ptr, int len)
// {
//    // Write "len" of char from "ptr" to file id "fd"
//    // * Return number of char written.
//    // * Need implementing with UART here. 
//   //fd = 1;
//   int i = 0;
//   for ( i = 0; i<len ;i++)
//   {
//     send_byte(*ptr);
//     ptr++;
//   }
//   return len;
// }



 