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
		GAME_EventHandler1();
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


//Main Function
int main(void)
{	
	//RCC_ClocksTypeDef RCC_Clocks;
	/* Configure SysTick */
  	//RCC_GetClocksFreq(&RCC_Clocks);
  	//SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);/* SysTick end of count event each 10ms */
       //GPIO_Input_Config();
       ADC_Config();
       //NVIC_Config();
	prvInit();
       
	if( STM_EVAL_PBGetState( BUTTON_USER ) )
		demoMode = 0;
  
	xTaskCreate( GameTask, (signed char*) "GameTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	//xTaskCreate( GameEventTask1, (signed char*) "GameEventTask1", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	//xTaskCreate( GameEventTask2, (signed char*) "GameEventTask2", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	//xTaskCreate( GameEventTask3, (signed char*) "GameEventTask3", 128, NULL, tskIDLE_PRIORITY + 1, NULL );

	//Call Scheduler
	vTaskStartScheduler();
}




