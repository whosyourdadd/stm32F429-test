#include "game.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



u16 Adc_Teset_Buf[2] = {0};
u16 Adc_DMA_Buf[4] = {0};
u8 MsgRecvQueue[5] ={0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
u8 MsgRecv_Count = 0;
u8 MsgRecv_Head = 0;
u8 MsgRecv_Trail = 0;
//u8 PrintString[32];
u8 PrintString[5] ={0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
//u8 PrintString[5] ={0x00, 0x00, 0x00, 0x00, 0x00};
int UART_count ;
int spi_count = 0;
//Mode
uint8_t demoMode = 0;
uint8_t TX_ADDRESS[TX_ADR_WIDTH]  = { 0x34,0x43,0x10,0x10,0x01 };
uint8_t  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};
uint8_t rx_buf[TX_PLOAD_WIDTH];
uint8_t tx_buf[TX_PLOAD_WIDTH];
/*
//Player1
int16_t player1X = 0;
int16_t player1Y = LCD_PIXEL_HEIGHT-10;
uint16_t player1W = 60;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;


//The coins 
uint16_t coinSize = 10;

typedef struct Coin {
	  int16_t coinX;
	  int16_t coinY;
	  int16_t coinVX;
	  int16_t coinVY;
	  uint8_t isRun;

} Coin;





#define MAX_COIN_NUM 10
Coin Coins[MAX_COIN_NUM];
uint16_t hitTimes = 0;
uint16_t missTimes = 0;
uint16_t player1Color = LCD_COLOR_WHITE;

 *
 

void
initCoins(uint8_t i)
{	
		Coins[i].coinX = rand() % (LCD_PIXEL_WIDTH - coinSize);
		Coins[i].coinY = coinSize ;
		Coins[i].coinVX = 0;
		Coins[i].coinVY = (rand()%5 +1);
		Coins[i].isRun = 1;
}

void updateCoin(uint8_t coinID){
		if (Coins[coinID].isRun)
		{
			LCD_SetTextColor( LCD_COLOR_BLACK );
			LCD_DrawFullRect(Coins[coinID].coinX, Coins[coinID].coinY, coinSize ,coinSize);
			Coins[coinID].coinY += Coins[coinID].coinVY; 

				if (Coins[coinID].coinX >= player1X && Coins[coinID].coinX <= player1X + player1W &&
					Coins[coinID].coinY >= player1Y && Coins[coinID].coinY <= player1Y + player1H)
				{	
					player1Color -= 0x007F;
					if(player1Color < 0) {
						player1Color = LCD_COLOR_WHITE;
					}
					hitTimes++;
					Coins[coinID].isRun = 0;
					initCoins(coinID);
				}
				if (Coins[coinID].coinY>LCD_PIXEL_HEIGHT-10)
				{	
					missTimes++;
					Coins[coinID].isRun = 0;
					initCoins(coinID);
				}
		} else { //isRun
			//missTimes++;
			initCoins(coinID);
		} 
}



void
GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ) ){

		player1IsReversed = 0;

		while( STM_EVAL_PBGetState( BUTTON_USER ) );

		player1IsReversed = 1;
	}
}

void
GAME_EventHandler2()
{
}

void
GAME_EventHandler3()
{
}

void
GAME_Update()
{
	//Player1
	LCD_SetTextColor( LCD_COLOR_BLACK );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	if( player1IsReversed )
				player1X -= 5;
			else
				player1X += 5;

			if( player1X <= 0 )
				player1X = 0;
			else if( player1X + player1W >= LCD_PIXEL_WIDTH )
				player1X = LCD_PIXEL_WIDTH - player1W;
			
	//The coins behavior

		for(uint8_t i=0 ;i<MAX_COIN_NUM;i++){
			updateCoin(i);
			if (i == MAX_COIN_NUM)
			{
				i = 0;
			}
		}

}

void
GAME_Render()
{
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawCircle( LCD_PIXEL_WIDTH/ 2, LCD_PIXEL_HEIGHT  / 2 , 50);
	
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DrawLine(  0, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH  , LCD_DIR_HORIZONTAL );
	//Xpos,Ypos,length, direction
	//Win or lose
	uint8_t ascii = 0x30;
		if(hitTimes > 10 || missTimes > 15){
			LCD_Clear(0x0000);
			if(missTimes < 15)
				LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "You Win");
			else
				LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "You Lose");
			missTimes = hitTimes = 0;
			vTaskDelay(700);
			LCD_Clear(0x0000);
			LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "Play Again");
			vTaskDelay(500);
			LCD_Clear(0x0000);
			LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "Ready Go");
			vTaskDelay(300);
			LCD_Clear(0x0000);
		}

	//hitTimes
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayChar(LCD_PIXEL_HEIGHT/2+5,10, hitTimes+48);
	//Display Ball
	LCD_SetTextColor( LCD_LOR_BLUE );
	int i;
	for(i=0;i<MAX_COIN_NUM;i++){
		LCD_DrawFullRect( Coins[i].coinX,Coins[i].coinY , coinSize,coinSize );
		if (i == MAX_COIN_NUM)
			{
				i = 0;
			}
	}
}
*/
/*void SET_NRF_CE(GPIO_InitTypeDef  * GPIOx,unsigned char GPIO_PinValue, unsigned char value){
     if(value == 0){
            GPIO_ResetBits(GPIOx,GPIO_PinValue);//
     }else{
            GPIO_SetBits(GPIOx,GPIO_PinValue);//
     }
}

void SET_NRF_CSN(GPIO_InitTypeDef  * GPIOx,unsigned char GPIO_PinValue, unsigned char value){
     if(value == 0){
            GPIO_ResetBits(GPIOx,GPIO_PinValue);//
     }else{
            GPIO_SetBits(GPIOx,GPIO_PinValue);//
     }
}*/
void Delay_count(int count){
	for(;count <0;count--);
}
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



//=============================================================================================================
uint8_t SPI_RW( SPI_TypeDef* SPIx, u8 WriteByte ){
  
  	//while((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
	  //SPIx->DR = WriteByte;
	  SPI_I2S_SendData(SPIx,WriteByte);
	  //while((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );
	  //SPIx->DR;
	  return  SPI_I2S_ReceiveData(SPIx);
}

uint8_t SPI_ReadByte( SPI_TypeDef* SPIx ){
  //while((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
   //SPIx->DR = 0xFF;
  SPI_I2S_SendData(SPIx,0xff);
  //while((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);
  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );

  //return SPIx->DR;
  return  SPI_I2S_ReceiveData(SPIx);
}
void SPI_WriteByte( SPI_TypeDef* SPIx, u8 WriteByte ){
	  //while((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET );
	  //SPIx->DR = WriteByte;
	  SPI_I2S_SendData(SPIx,WriteByte);
	  //while((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);
	  while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET );
	  //SPIx->DR;
	  //return SPI_I2S_ReceiveData(SPIx);
}
//=============================================================================================================
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
	//CE(0);
	//GPIO_SetBits(NRF_CE_GPIO,NRF_CE_GPIO_Pin)
	//GPIO_ResetBits(NRF_CE_GPIO,NRF_CE_GPIO_Pin);
	//GPIO_ResetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin);
  	//CSN(0);//CSN = 0;                   // Set CSN low, init SPI tranaction
  	//status = SPI_RW(SPI1,reg);    // Select register to write to and read status byte
  	//for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	// SPI_RW(SPI1,*pBuf++); //SPI_RW(*pBuf++);
  	//SPI_WriteByte(NRF_SPI, WriteAddr);
  	status = SPI_RW(NRF_SPI, WriteAddr);
  	for(i=0; i<Bytes; i++)
  		SPI_RW(NRF_SPI,WriteBuf[i]);
    		//SPI_WriteByte(NRF_SPI, WriteBuf[i]);
  	
  	
  	//CSN(1);//CSN = 1;                 // Set CSN high again
  	//GPIO_SetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin);
  	return(status);          // return nRF24L01 status byte
}

void SPI_Read_Buf( uint8_t ReadAddr, uint8_t *ReadBuf, uint8_t Bytes )
{
  uint8_t i = 0;

  //CE(0);//NRF_CE = 0;
  //CSN(0);//NRF_CSN = 0;
  //GPIO_ResetBits(NRF_CE_GPIO,NRF_CE_GPIO_Pin);
  //GPIO_ResetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin);
  //SPI_WriteByte(NRF_SPI, ReadAddr);
  SPI_RW( NRF_SPI,ReadAddr);
  for(i=0; i<Bytes; i++)
  	ReadBuf[i] = SPI_RW(NRF_SPI,0xff);
    //ReadBuf[i] = SPI_ReadByte(NRF_SPI);

  //CSN(1);//NRF_CSN = 1;
  //GPIO_SetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin);
}


//=============================================================================================================
// CHECK FOR nRF CHIP WORK
//=============================================================================================================
uint8_t nRF_Check( void )
{
  uint8_t i = 0;
  uint8_t SendBuf[5] = {0xFF, 0xff, 0xff, 0xff, 0xff};
  uint8_t RecvBuf[5] = {0};
   //CE(0);
   CSN(0);
   SPI_Write_Buf(WRITE_REGL +TX_ADDR, PrintString, 5);
   //SPI_Write_Buf(0x20 + TX_ADDR, PrintString, 1);
  //SPI_Write_Buf( 0x20+0x10, PrintString, 5);
 //SPI_Read_Buf(TX_ADDR, RecvBuf, 5);
  SPI_Read_Buf(TX_ADDR,MsgRecvQueue ,5);
   //SPI_Read_Buf(TX_ADDR,PrintString ,5);
  //return MsgRecvQueue[0];
  CSN(1);
  // CE(1);
  for(i=0; i<5; i++){
    //if(RecvBuf[i]!=SendBuf[i])  break;
        if(PrintString[i] != MsgRecvQueue[i]){
  	//if( MsgRecvQueue[i] != 0x00){
  	//return MsgRecvQueue[i];
    	break;
    	//return 0;
    }
  }
  //return 0;
  return (i==5) ?  1 : 0;
  //return i;
}

void SPI1_SendHandler(void){

	//SPI_WriteByte(SPI1, WRITE_REGL +TX_ADDR);
	//CSN(1);
	if(spi_count == 0){
	//while(SPI_I2S_GetFlagStatus(NRF_SPI,SPI_I2S_FLAG_RXNE) == RESET );
	//MsgRecvQueue[0] = SPI_I2S_ReceiveData(NRF_SPI);
		//spi_count += SPI_RW(NRF_SPI,WRITE_REGL+TX_ADDR);
		
		//vTaskDelay( 10 );
		spi_count += nRF_Check();
	}
	
	/*static int count = 0;
	static u8 state_flag  = 0;
	if(count++ > 10000){
		count = 0;
		if(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_TXE)){
			switch(state_flag){
				case 0x00:
					SPI_I2S_SendData(SPI1,49);
					state_flag = 0x01;
					break;
				case 0x01:
					SPI_I2S_SendData(SPI1,50);
					state_flag = 0x02;
					break;
				case 0x02:
					SPI_I2S_SendData(SPI1,51);
					state_flag = 0x00;
					break;

			}
			
		}
	}*/
}
/*
void SPI1_IRQHandler(void)
{
    uint8_t rcv_tmp = 0;
     static uint8_t cnt = 0;  
     if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)){
       	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_FLAG_RXNE);
            	rcv_tmp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
            	spi_count++;
            if(rcv_tmp > 48 && rcv_tmp < 52){
                    spi_count = 0; 
                    PrintString[cnt] =  rcv_tmp;       
                    cnt++;
                    cnt %= 5;
            }
     }
}   
*/

void Usart1_Send(char *s){
    while(*s) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *s);
        s++;
    }
}

 /*
void USART1_IRQHandler(){
     
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        // 如果收到 A 字元，則發送 ok 字串
        //if((char)USART_ReceiveData(USART1) == 'A')
        //{
        //   Usart1_Send("ok \n");           
        //}
        //if(MsgRecv_Count  <32){
        //	MsgRecvQueue[MsgRecv_Count]=USART_ReceiveData(USART1);
        //	MsgRecv_Count++;
        	MsgRecv_Head = (MsgRecv_Head+1)%32;
        	MsgRecvQueue[MsgRecv_Head]=USART_ReceiveData(USART1);
        //}
        	
    }
} 
*/
void USART1_IRQHandler(void){
	static u8 buff[10] = {0};
	static u8 cnt = 0;
	int i;
	int chk = 0;
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		UART_count++;
		//static uint8_t cnt = 0; // this counter is used to determine the string length
		//char t = USART1->DR; // the character from the USART1 data register is saved in t
		buff[cnt] = USART_ReceiveData(USART1);
		if(   buff[cnt]  >48  && buff[cnt]  < 52){
		cnt++;
		}
		if(cnt > 3 ){
			cnt = 0;
			UART_count=0;
			for(i=0;i<3;i++){
				MsgRecvQueue[i ] = buff[i];
			}
			
		}
	}
}


void UART_Handler(void){
    	static u16 Delay_count = 0;
    	static u8 cmd = 49;
    	int i;
    	switch(Delay_count){
    	   	
    		case 0x00:
    			if(cmd < 51){
                        			cmd++;
                   		}else{
                        			cmd=49;
                   		}
    			//Usart1_Send("123");   	
                   		//Usart1_Send(&cmd);
    			Delay_count = 0;
    		   	break;
    		 default:
    		 	Delay_count = 0x00;
    		 	//Delay_count++;
    		 	break;

    	}
    	PrintString[0] = MsgRecv_Head +48;
    	/*if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET){
    		MsgRecv_Head = (MsgRecv_Head+1)%10;
        		MsgRecvQueue[MsgRecv_Head]=USART_ReceiveData(USART1);
        	}*/
        		//MsgRecv_Head = (MsgRecv_Head+1)%32;
        		//MsgRecvQueue[MsgRecv_Head]=USART_ReceiveData(USART1);
    	/*if(MsgRecv_Count > 10){
    		for(i=0;i<32;i++){
    			PrintString[i] = MsgRecvQueue[i];
    		}
    		MsgRecv_Count=0;
    	}*/

}

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
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-60 ,PrintString);

	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-30 ,MsgRecvQueue);
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
