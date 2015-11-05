/**
  ******************************************************************************
  * @file    Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
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
#include "game.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_ltdc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "stm32f429i_discovery_ioe.h"

/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t demoMode;
#define ADC1_DR_ADDRESS ((u32)0x4001204C)
#define ADC2_DR_ADDRESS ((u32)0x4001214C)
#define ADC3_DR_ADDRESS ((u32)0x4001224C)




void
prvInit()
{
	//LCD init
	LCD_Init();
	IOE_Config();
	LTDC_Cmd( ENABLE );

	LCD_LayerInit();
	LCD_SetLayer( LCD_FOREGROUND_LAYER );
	LCD_Clear( LCD_COLOR_BLACK );
	LCD_SetTextColor( LCD_COLOR_WHITE );

	//Button
	STM_EVAL_PBInit( BUTTON_USER, BUTTON_MODE_GPIO );

	//LED
	STM_EVAL_LEDInit( LED3 );
}

static void GameEventTask1( void *pvParameters )
{
	while( 1 ){
		//GAME_EventHandler1();
              //UART_Handler();
              SPI1_SendHandler();
	}
}

static void GameEventTask2( void *pvParameters )
{
	while( 1 ){
		GAME_EventHandler2();
	}
}

static void GameEventTask3( void *pvParameters )
{
	while( 1 ){
		GAME_EventHandler3();
	}
}

static void GameTask( void *pvParameters )
{
	while( 1 ){
		//GAME_Update();
		//GAME_Render();
              //UART_Handler();
		Background_draw();
		vTaskDelay( 10 );
	}
}


void ADC_Config( void )
{
  DMA_InitTypeDef DMA_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;

  // ADC Clk Init 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);




   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5; //Open GPIO PC1,3,4,5 to adc read
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  // ADC1 DMA Stream0 Channel_0 Init 
  DMA_InitStruct.DMA_Channel = DMA_Channel_0;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)ADC1_DR_ADDRESS;           // Peripheral address 
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32 )Adc_DMA_Buf;                     // Memory address //test 2015/5
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;                           // Peripheral to Memory
  DMA_InitStruct.DMA_BufferSize = 4;                                                                    // Memory Buffer Size four analog signal input
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               // Peripheral address 遞增 Disable
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                        // Memory address 遞增 Enable
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    // Peripheral Data Size 16bit
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;            // Memory Data Size 16bit
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                                // 循環模式 Enable
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                          // ADC DMA通道 普通優先級
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;                         // DMA FIFO Disable
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStruct);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  
  // ADC Common Init 
  ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;                       // 獨立模式
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;                    // 預分頻4
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        // ADC DMA Mode Disable
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;  // 轉換延遲時間
  ADC_CommonInit(&ADC_CommonInitStruct);

  // ADC1 Init 
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                         // 解析度 12bit
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                   // 掃描模式
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                             // 連續轉換模式
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    // 外部觸發 Disable
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                         // ADC數據右對齊
  ADC_InitStruct.ADC_NbrOfConversion = 4;                                             // 轉換ADC通道數目
   
  ADC_Init(ADC1, &ADC_InitStruct);
 
   // ADC1 Regular Config 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11,  1, ADC_SampleTime_28Cycles); //test 2015/5   PC1   rank 1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13,  2, ADC_SampleTime_28Cycles); //test 2015/5   PC3   rank 2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14,  3, ADC_SampleTime_28Cycles); //test 2015/5   PC4   rank 3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15,  4, ADC_SampleTime_28Cycles); //test 2015/5   PC5   rank 4

  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);

}


void UART_Config( void ){

      GPIO_InitTypeDef GPIO_InitStructure;
      USART_InitTypeDef USART_InitStructure;
      NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // USART1 clock enable 
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// GPIOA clock enable 


      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      // Connect USART pins to AF 
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX



    // USARTx configuration ------------------------------------------------------
    /* USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    //USART_InitStructure.USART_BaudRate = 9600;
     USART_InitStructure.USART_BaudRate = 1200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //NVIC_EnableIRQ(USART1_IRQn);
    /* Here the USART1 receive interrupt is enabled
   * and the interrupt controller is configured 
   * to jump to the USART1_IRQHandler() function
   * if the USART1 receive interrupt occurs
   */
  //USART_ClearFlag(USART1, USART_FLAG_TC);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;    // we want to configure the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     // this sets the subpriority inside the group
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      // the USART1 interrupts are globally enabled
  NVIC_Init(&NVIC_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt 
  USART_Cmd(USART1, ENABLE);
  
}


// spi master
void SPI_config(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  // enable clock for used IO pins
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOF , ENABLE);//SPI1
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA  , ENABLE);//SPI3
  // enable SPI2 peripheral clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //IRQ

  
  //CE PC5 PA14 PA4
  GPIO_InitStruct.GPIO_Pin = NRF_CE_GPIO_Pin   ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(NRF_CE_GPIO, &GPIO_InitStruct);
  // CSN PC4 PA15 PA3
  GPIO_InitStruct.GPIO_Pin =  NRF_CSN_GPIO_Pin  ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  //GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(NRF_CSN_GPIO, &GPIO_InitStruct);
   //IRQ PB0 PA13 PA2
  GPIO_InitStruct.GPIO_Pin = NRF_IRQ_GPIO_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(NRF_IRQ_GPIO, &GPIO_InitStruct);

   /* IRQ EXTI  */
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, NRF_IRQ_CONFIG );
  //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, NRF_IRQ_CONFIG );
   EXTI_InitStruct.EXTI_Line = NRF_IRQ_LINE ;
   EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
   EXTI_InitStruct.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStruct);
   //configure pins used by SPI1
   // PC10 = SCK
   // PC11 = MISO
   // PC12 = MOSI
  GPIO_InitStruct.GPIO_Pin = SPI_SCK_GPIO_Pin| SPI_MISO_GPIO_Pin | SPI_MOSI_GPIO_Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  //GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
  // connect SPI2 pins to SPI alternate function
  GPIO_PinAFConfig(SPI_GPIO, SPI_SCK_PinSource ,  SPI_GPIO_AF );//SCK
  GPIO_PinAFConfig(SPI_GPIO, SPI_MISO_PinSource, SPI_GPIO_AF );//MISO
  GPIO_PinAFConfig(SPI_GPIO, SPI_MOSI_PinSource, SPI_GPIO_AF );//MOSI

  CSN(1);//CSN == 1 SPI unable
  /* configure SPI1 in Mode 0
   * CPOL = 0 --> clock is low when idle
   * CPHA = 0 --> data is sampled at the first edge
   */
  SPI_I2S_DeInit(NRF_SPI);
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // transmit in master mode, NSS pin has to be always high
  //SPI_InitStruct.SPI_Mode = SPI_Mode_Slave; // transmit in master mode, NSS pin has to be always high
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // clock is low when idle
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // data sampled at first edge
  //SPI_InitStruct.SPI_NSS = SPI_NSS_Soft|SPI_NSSInternalSoft_Set ; // set the NSS management to internal and pull internal NSS high
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ; // set the NSS management to internal and pull internal NSS high
  //SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // SPI frequency is APB2 frequency / 4
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(NRF_SPI, &SPI_InitStruct);
  SPI_Cmd(NRF_SPI, ENABLE); // enable SPI1

  // SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE); // make SPI1 receive interrupt enable  
  //NVIC_InitTypeDef NVIC_InitStructure;
  //NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn; // Configure SPI1 interrupt
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Set the priority group of SPI1 interrupt
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Set the subpriority inside the group
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // Globally enable SPI1 interrupt
  //NVIC_Init(&NVIC_InitStructure);

}

//Main Function
int main(void)
{	
	//RCC_ClocksTypeDef RCC_Clocks;
	/* Configure SysTick */
  	//RCC_GetClocksFreq(&RCC_Clocks);
  	//SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);/* SysTick end of count event each 10ms */
       //GPIO_Input_Config();
       //SystemInit();
       ADC_Config();
       SPI_config();
       //UART_Config();
       //NVIC_Config();
	prvInit();
       //RX_Mode();
       //init_NRF24L01() ;
	if( STM_EVAL_PBGetState( BUTTON_USER ) )
		demoMode = 0;
       //while(nRF_Check() == 0);
       
          //spi_count += nRF_Check();
	xTaskCreate( GameTask, (signed char*) "GameTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( GameEventTask1, (signed char*) "GameEventTask1", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	//xTaskCreate( GameEventTask2, (signed char*) "GameEventTask2", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	//xTaskCreate( GameEventTask3, (signed char*) "GameEventTask3", 128, NULL, tskIDLE_PRIORITY + 1, NULL );

	//Call Scheduler
	vTaskStartScheduler();
}




