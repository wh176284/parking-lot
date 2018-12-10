#include "sim_spi1.h"
#include "systick.h"

void Sim_SPI1_Init(void)
{  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/



  /*##-2- Configure peripheral GPIO ##########################################*/
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitTypeDef GPIO_InitStruct;
	
		/* Enable GPIO TX/RX clock */
  SPI1_SCK_GPIO_CLK_ENABLE();
  SPI1_MISO_GPIO_CLK_ENABLE();
  SPI1_MOSI_GPIO_CLK_ENABLE();
  SPI1_NSS_GPIO_CLK_ENABLE();
  
  GPIO_InitStruct.GPIO_Pin       = SPI1_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStruct);	
	
	//GPIO_SetBits(GPIOD , GPIO_Pin_9);

  /* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.GPIO_Pin       = SPI1_MISO_PIN;
  GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_MISO_GPIO_PORT, &GPIO_InitStruct);	

  /* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.GPIO_Pin       = SPI1_MOSI_PIN;
  GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd      = GPIO_PuPd_UP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStruct);	
  
	/* SPI NSS GPIO pin configuration  */
	GPIO_InitStruct.GPIO_Pin       = SPI1_NSS_PIN;
  GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_NSS_GPIO_PORT, &GPIO_InitStruct);
//	
	MOSI1_H;
	SCK1_L;
	NSS1_H;
//	GPIO_SetBits(GPIOD , GPIO_Pin_8);
//	GPIO_SetBits(GPIOD , GPIO_Pin_9);
//	GPIO_SetBits(GPIOD , GPIO_Pin_11);
	__nop();
	
}

void Sim_SPL1_nop(){
	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
//	__NOP();
	//Delay_ms(1);
}

/**
*	 ���ֽ�ģ���дSPI���ݣ�����ģʽ��SPIMODE = 0����CPOL = 0,CPHA = 0
*	 �����ֿ�Ϊ8λ��
*	 ��SPI�����ԣ���д��һ���ֽڵ�ͬʱ��Ҳ���ȡһ���ֽ�
*/
uint8_t Sim_SPI1_ReadWrite_Byte(uint8_t data)
{  
  uint8_t temp=0;
	uint8_t i = 0;
  //SPI_Init();
  //NSS_L;   //SPI NSS�ź���Ϊ��
  SCK1_L;    
	for(i=0;i<8;i++)
	{ 
		//�Ƚ�Ҫ��������ݷŵ�������ȥ
		if(data&0x80) 
		{
			MOSI1_H;          /*�����λΪ�ߣ��������*/
		}
		else
		{
			MOSI1_L;          /*�����λΪ�ͣ��������*/    
		}    
		data <<= 1;
		
		Sim_SPL1_nop(); 
		
		SCK1_H; //���������أ��ͳ����ݣ�ͬʱ��������
		
		temp <<= 1;        /*����һ�ε���������1λ*/
		if(MISO1)
		{
			temp++;          /*���Ӵӻ����յ��ߵ�ƽ�������Լ�һ*/
		}
		
		Sim_SPL1_nop();
		
		SCK1_L;
	} 
    
  //NSS_H;
  //MOSI_H; 
  return temp;
}

void Sim_SPI1_ReadWriteBuf(uint8_t *writebuf,uint8_t *readbuf,uint16_t buflen){
	uint16_t i = 0;
	
	SCK1_L;
	Sim_SPL1_nop();
	NSS1_L;
	
	Sim_SPL1_nop();
	
	for(i=0;i<buflen;i++){
		readbuf[i] = Sim_SPI1_ReadWrite_Byte(writebuf[i]);
	}
	
	Sim_SPL1_nop();
	NSS1_H;
	Sim_SPL1_nop();
	SCK1_L;
}