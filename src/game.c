#include "game.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



u16 Adc_Teset_Buf[2] = {0};
u16 Adc_DMA_Buf[4] = {0};


int UART_count = 0;
int spi_count = 0;
uint8_t demoMode = 0;
  uint8_t SendBuf[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
  uint8_t RecvBuf[5] = {0};
uint8_t TX_ADDRESS[TX_ADR_WIDTH]  = { 0x34,0x43,0x10,0x10,0x01 };
uint8_t  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};
uint8_t rx_buf[TX_PLOAD_WIDTH];
uint8_t tx_buf[TX_PLOAD_WIDTH];

void Delay_count(int count){
	for(;count <0;count--);
}
//=============================================================================================================
// CHECK FOR nRF CHIP WORK
//=============================================================================================================
uint8_t nRF_Check( void )
{
  uint8_t i = 0;

   CE(0);        // CE low, Enable RX TX
   CSN(0);     // CSN low SPI chip select
   SPI_Write_Buf(WRITE_REGL +TX_ADDR, SendBuf, 5);
   SPI_Read_Buf(TX_ADDR,RecvBuf ,5);
  CSN(1);      // CSN  high SPI chip not select
  for(i=0; i<5; i++){
        if(SendBuf[i] != RecvBuf[i]){
    	break;
    }
  }
  return (i==5) ?  1 : 0;
}

void SPI1_SendHandler(void){

	if(nRF_Check()){
		spi_count++;
	}
	

}


//=============================================================================================================
//     some of spi send and recv function section
// 
//=============================================================================================================
uint8_t SPI_RW( SPI_TypeDef* SPIx, u8 WriteByte ){
  
  
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
	 
	  SPI_I2S_SendData(SPIx,WriteByte);
	 
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );
	 
	  return  SPI_I2S_ReceiveData(SPIx);
}

uint8_t SPI_ReadByte( SPI_TypeDef* SPIx ){
 
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
  
  SPI_I2S_SendData(SPIx,0xff);
 
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );

 
  return  SPI_I2S_ReceiveData(SPIx);
}
void SPI_WriteByte( SPI_TypeDef* SPIx, u8 WriteByte ){
	 
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
	 
	  SPI_I2S_SendData(SPIx,WriteByte);
	 
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );
}
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value){
	
	uint8_t status;
  	CSN(0); //CSN = 0;                   	// CSN low, init SPI transaction
  	status = SPI_RW(NRF_SPI,reg);      	// select register
  	SPI_RW(NRF_SPI,value);             	// ..and write value to it..
  	CSN(1);//CSN = 1;                   	// CSN high again

  	return(status);            // return nRF24L01 status byte
}

uint8_t nRF_ReadReg( uint8_t ReadAddr )
{
  uint8_t ReadData;

  CE(0);
  CSN(0);
  SPI_WriteByte(NRF_SPI, ReadAddr);
  ReadData = SPI_ReadByte(NRF_SPI);
  CSN(1);

  return ReadData;
}

uint8_t SPI_Write_Buf(uint8_t WriteAddr, uint8_t *WriteBuf, uint8_t Bytes){
	uint8_t status,i;
  	status = SPI_RW(NRF_SPI, WriteAddr);
  	for(i=0; i<Bytes; i++)
  		SPI_RW(NRF_SPI,WriteBuf[i]);
  	return(status);          // return nRF24L01 status byte
}

void SPI_Read_Buf( uint8_t ReadAddr, uint8_t *ReadBuf, uint8_t Bytes )
{
  uint8_t i = 0;

  SPI_RW( NRF_SPI,ReadAddr);
  for(i=0; i<Bytes; i++)
  	ReadBuf[i] = SPI_RW(NRF_SPI,0xff);
}

//=============================================================================================================
//        Copy from network
//=============================================================================================================
/*
void init_NRF24L01(void)
{
             //Delay_count(1);
 	CE(0);//RF24L01_CE_0 ;    // chip enable
 	CSN(1);//RF24L01_CSN_1;   // Spi disable 
 	SCK(0);//RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REGL + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REGL + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REGL + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REGL + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REGL + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REGL + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REGL + RF_SETUP, 0x07);   //设置发射速率为1MHZ，发射功率为最大值0dB
	SPI_RW_Reg(WRITE_REGL + CONFIG, 0x0f);   // IRQ收发完成中断响应，16位CRC	，主接收}
}

void RX_Mode(void)
{
	CE(0);//CE=0;
  	SPI_Write_Buf(WRITE_REGL + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device

  	SPI_RW_Reg(WRITE_REGL + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REGL + EN_RXADDR, 0x01);  // Enable Pipe0
  	SPI_RW_Reg(WRITE_REGL + RF_CH, 40);        // Select RF channel 40
  	SPI_RW_Reg(WRITE_REGL + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(WRITE_REGL + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REGL + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..

  	CE(0);//CE = 1; // Set CE pin high to enable RX device

  //  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}
void TX_Mode(void)
{
	CE(0);//CE=0;
  	SPI_Write_Buf(WRITE_REGL + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  	SPI_Write_Buf(WRITE_REGL + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
  	//SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // Writes data to TX payload

  	SPI_RW_Reg(WRITE_REGL + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REGL + EN_RXADDR, 0x01);  // Enable Pipe0
  	//nRF_WriteReg(NRF_WRITE+SETUP_RETR, 0x05); // 設置自動重發間隔時間: 250us + 86us; 最大自動重發次數: 5 次
  	SPI_RW_Reg(WRITE_REGL + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans... 
  	SPI_RW_Reg(WRITE_REGL + RF_CH, 40);        // Select RF channel 40
  	//nRF_WriteReg(NRF_WRITE+RF_SETUP, 0x0f);   // 設置 TX 發射參數, 0db增益, 2Mbps, 低噪聲增益開啟
  	SPI_RW_Reg(WRITE_REGL + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg(WRITE_REGL + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
	CE(1);//CE=1;

}
*/
//=============================================================================================================
// LCD draw  
//=============================================================================================================
void Background_draw(void){
	u8 X_String[12] = {0};
	u8 Y_String[12] = {0};
	u8 L_String[12] = {0};
	u8 R_String[12] = {0};
	u8 UART_String[12] = {0};
	u8 SPI_String[12] = {0};
	u8 X_delta=20 ,Y_delta=20;
	static  u16 output_X =100 ,output_Y = 100;
	LCD_Clear(0x0000);
	Get_StringValue("PC1:",X_String,Adc_DMA_Buf[0],4,9);
	Get_StringValue("PC3:",Y_String,Adc_DMA_Buf[1],4,9);
	//Get_StringValue("PC4:",L_String,Adc_DMA_Buf[2],4,9);
	//Get_StringValue("PC5:",R_String,Adc_DMA_Buf[3],4,9);
	Get_StringValue("SPI:",SPI_String,spi_count,4,10);
	Get_StringValue("UART:",UART_String,UART_count,5,10);

	LCD_SetTextColor( LCD_COLOR_BLUE );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-150,X_String );//LCD_PIXEL_HEIGHT 320 

	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-120,Y_String );

	//LCD_SetTextColor( LCD_COLOR_BLUE );
	//LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-90,L_String );

	//LCD_SetTextColor( LCD_COLOR_RED );
	//LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-60 ,R_String );

	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-120 ,SPI_String );	

	LCD_SetTextColor( LCD_COLOR_BLUE );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-90 ,UART_String);

	LCD_SetTextColor( LCD_COLOR_BLUE );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-60 ,SendBuf);

	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-30 ,RecvBuf);
	//LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-60 ,"Hello");
	//LCD_PIXEL_WIDTH  240
	//LCD_PIXEL_HEIGHT 320 
	if(Adc_DMA_Buf[0] >33000){	
		output_X  -=	X_delta;
	}else if(Adc_DMA_Buf[0] <10000){
		output_X  +=	X_delta;
	}
	if(Adc_DMA_Buf[1] >33000){	
		output_Y  +=	Y_delta;
	}else if(Adc_DMA_Buf[1] <10000){	
		output_Y  -=	Y_delta;
	}
	if(output_X > LCD_PIXEL_WIDTH){//X axis bound protect
		output_X = LCD_PIXEL_WIDTH;
	}else if(output_X <= 0){
		output_X = 0;
	}
	if(output_Y > LCD_PIXEL_HEIGHT){//Y axis bound protect
		output_Y = LCD_PIXEL_HEIGHT;
	}else if(output_Y <= 0){
		output_Y = 0;
	}
	LCD_SetTextColor( LCD_COLOR_GREEN );
	LCD_DrawFullRect(output_X ,output_Y , 10, 10 );//output lcd
	vTaskDelay(300);

	

	
}

void Get_StringValue(u8 *head_String , u8 *CombineString , u16 InputValue , u16 string_head , u16 string_end){
	int i  =0;   
	u16 temp = InputValue;
	while(head_String[i] != '\0'){
		CombineString[i] = head_String[i];
		i++;
		if(i>5) break ;
	}

	for(i=string_end ; i>=string_head ; i-- ){
		CombineString[i] = (temp%10)+48;
		temp /=10;
	} 
	CombineString[string_end +1] = '\0';


}
