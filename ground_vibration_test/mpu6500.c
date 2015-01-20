/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "mpu6500.h"
#include "main.h"
#include "functions.h"

/*====================================================================================================*/
/*====================================================================================================*/



/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_ReadReg
**功能 : 讀暫存器
**輸入 : ReadAddr
**輸出 : ReadData
**使用 : MPU9250_ReadReg(ReadAddr, &DeviceID);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_ReadReg(SPI_TypeDef* SPIx, u8 ReadAddr, u8 *ReadData )
{
	if(SPIx == SPI1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	}
	SPIx_ReadWriteByte(SPIx, 0x80 | ReadAddr);
	*ReadData = SPIx_ReadWriteByte(SPIx, 0xFF);
	if(SPIx == SPI1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteReg
**功能 : 寫暫存器
**輸入 : WriteAddr, WriteData
**輸出 : None
**使用 : MPU9250_WriteReg(WriteAddr, WriteData);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_WriteReg(SPI_TypeDef* SPIx, u8 WriteAddr, u8 WriteData )
{
	if(SPIx == SPI1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	}
	SPIx_ReadWriteByte(SPIx, WriteAddr);
	SPIx_ReadWriteByte(SPIx, WriteData);
	if(SPIx == SPI1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_ReadRegs
**功能 : 連續讀暫存器
**輸入 : ReadAddr, *ReadBuf, Bytes
**輸出 : None
**使用 : MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
**=====================================================================================================*/
/*=====================================================================================================*/
void MPU9250_ReadRegs(SPI_TypeDef* SPIx, u8 ReadAddr, u8 *ReadBuf, u8 Bytes )
{
	u8 i = 0;
	if(SPIx == SPI1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	}
	//Delay_us(1);
	SPIx_ReadWriteByte(SPIx, 0x80 | ReadAddr);
	for(i=0; i<Bytes; i++)
	ReadBuf[i] = SPIx_ReadWriteByte(SPIx, 0xFF);
	if(SPIx == SPI1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else if(SPIx == SPI4)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Config
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Config(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	if(SPIx == SPI1)
	{
	/* SPI1 Clk Init ************************************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* SPI AF ******************************************************************/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	/* CSM PA4 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* SCK PA5 PA6 PA7*/
	// GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 ;
	// GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	// GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	// GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	// GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if(SPIx == SPI4)
	{
	/* SPI4 Clk Init ************************************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	/* SPI AF ******************************************************************/
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);

	/* CSM PE4 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	/* SCK PE5 PE6 PE2*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	}


/* SPI Init ****************************************************************/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Full Duplex
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // Master Mode
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // Data Size 8 bit
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; // Transitioned On The Falling Edge
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; // Latched On the Rising Edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // Software NSS Signal
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // fsck = APB2 90MHz / 2 = 45MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // MSB First
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStruct);
	SPI_Cmd(SPIx, ENABLE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Init
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Init();
**====================================================================================================*/
/*====================================================================================================*/
#define MPU9250_InitRegNum 10
void MPU9250_Init(SPI_TypeDef* SPIx)
{
	u8 i = 0;
	u8 MPU6500_Init_Data[MPU9250_InitRegNum][2] = {
	{0x80, MPU6500_PWR_MGMT_1}, // Reset Device
	{0x01, MPU6500_PWR_MGMT_1}, // Clock Source
	{0x00, MPU6500_PWR_MGMT_2}, // Enable Acc & Gyro
	{0x07, MPU6500_CONFIG}, //
	{0x18, MPU6500_GYRO_CONFIG}, // +-2000dps
	{0x18, MPU6500_ACCEL_CONFIG}, // +-16G
	{0x08, MPU6500_ACCEL_CONFIG_2}, // Set Acc Data Rates
	{0x30, MPU6500_INT_PIN_CFG}, //
	{0x40, MPU6500_I2C_MST_CTRL}, // I2C Speed 348 kHz
	{0x00, MPU6500_USER_CTRL}, // Enable AUX
	// Set Slave to Read AK8963
	//{0x8C, MPU6500_I2C_SLV0_ADDR}, // AK8963_I2C_ADDR ( 7'b000_1100 )
	//{0x00, MPU6500_I2C_SLV0_REG}, // AK8963_WIA ( 0x00 )
	//{0x81, MPU6500_I2C_SLV0_CTRL}, // Enable
	//{0x01, MPU6500_I2C_MST_DELAY_CTRL}
	};
	for(i=0; i<MPU9250_InitRegNum; i++) {
	MPU9250_WriteReg(SPIx,MPU6500_Init_Data[i][1], MPU6500_Init_Data[i][0]);
	Delay_us(1000);
	}
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Check
**功能 : MPU9250 Check
**輸入 : None
**輸出 : Status
**使用 : Status = MPU9250_Check();
**====================================================================================================*/
/*====================================================================================================*/
u8 MPU9250_Check(SPI_TypeDef* SPIx)
{
	u8 DeviceID = 0x00;
	/* MPU6500 */
	DeviceID = 0x00;
	//printf("%d\r\n",DeviceID);
	MPU9250_ReadReg(SPIx,MPU6500_WHO_AM_I, &DeviceID);
	printf("%d\r\n",DeviceID);
	if(DeviceID != MPU6500_Device_ID)
	return ERROR;
	/* AK8975 */
/*	DeviceID = 0x00;
	MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, 0x8C); // Set AK8963_I2C_ADDR = 7'b000_1100
	Delay_1us(10);
	MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_WIA); // Set Write Reg
	MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, 0x81); // Start Read
	Delay_1ms(1);
	MPU9250_ReadReg(MPU6500_EXT_SENS_DATA_00, &DeviceID); // Read Data
	if(DeviceID != AK8963_Device_ID)
	return ERROR;*/
	return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Read
**功能 : 讀取感測器資料
**輸入 : *ReadBuf
**輸出 : None
**使用 : MPU9250_Read(ReadBuf);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Read(SPI_TypeDef* SPIx, u8 *ReadBuf)
{
	MPU9250_ReadRegs(SPIx, MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
}
/*====================================================================================================*/
/*====================================================================================================*/

u8 SPIx_ReadWriteByte(SPI_TypeDef* SPIx, u8 TxData)
{		
	u8 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) 
		{
			retry++;
			if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPIx, TxData); 
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); 
		{
			retry++;
			if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPIx); 
}


