#ifndef GAME_H
#define GAME_H
#include "stm32f4xx_ltdc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "stm32f429i_discovery_ioe.h"
/*
#define ADC_Channel  3
#define ADC_Sample  16
#define Vref   3
#define Adc12Bit_Resolution 2047
*/

//#define NRF_SPI_RCC            RCC_APB1Periph_SPI2
//#define NRF_SPI                SPI2
//#define SPI_RCC_APB2Periph     RCC_APB2Periph_GPIOB
#define SPI_SCK_GPIO_Pin	GPIO_Pin_5
#define SPI_MISO_GPIO_Pin	GPIO_Pin_6
#define SPI_MOSI_GPIO_Pin	GPIO_Pin_7
#define SPI_GPIO               	GPIOA
#define SPI_SCK_PinSource       GPIO_PinSource5
#define SPI_MISO_PinSource    GPIO_PinSource6
#define SPI_MOSI_PinSource    GPIO_PinSource7
#define SPI_GPIO_AF    		GPIO_AF_SPI1
#define NRF_SPI                  	SPI1
/*****CE*****/
#define NRF_CE_GPIO                  GPIOF    //Orange
#define NRF_CE_GPIO_Pin          GPIO_Pin_11
/*****CSN*****/
//#define NRF_CSN_APB2Periph     RCC_APB2Periph_GPIOB
#define NRF_CSN_GPIO              GPIOF
#define NRF_CSN_GPIO_Pin       GPIO_Pin_12
/*****IRQ*****/
//#define NRF_IRQ_APB2Periph     RCC_APB2Periph_GPIOF
#define NRF_IRQ_GPIO           GPIOF
#define NRF_IRQ_GPIO_Pin       GPIO_Pin_13
#define NRF_IRQ_CONFIG         GPIO_PinSource13
#define NRF_IRQ_LINE		   EXTI_Line13

#define CE(x)    ((x)?(GPIO_SetBits(NRF_CE_GPIO,NRF_CE_GPIO_Pin)):(GPIO_ResetBits(NRF_CE_GPIO,NRF_CE_GPIO_Pin)))
#define CSN(x)   ((x)?(GPIO_SetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin)):(GPIO_ResetBits(NRF_CSN_GPIO,NRF_CSN_GPIO_Pin)))
#define SCK(x)   ((x)?(GPIO_SetBits(SPI_GPIO,SPI_SCK_GPIO_Pin)):(GPIO_ResetBits(SPI_GPIO,SPI_SCK_GPIO_Pin)))
//#define MOSI(x)  ((x)?(GPIO_SetBits(SPI_GPIO,SPI_MISO_GPIO_Pin)):(GPIO_ResetBits(SPI_GPIO,SPI_MISO_GPIO_Pin)))
//#define MISO(x)  ((x)?(GPIO_SetBits(SPI_GPIO,SPI_MOSI_GPIO_Pin)):(GPIO_ResetBits(SPI_GPIO,SPI_MOSI_GPIO_Pin)))
#define IRQ(x)   ((x)?(GPIO_SetBits(NRF_IRQ_GPIO,NRF_IRQ_GPIO_Pin)):(GPIO_ResetBits(NRF_IRQ_GPIO,NRF_IRQ_GPIO_Pin)))

#define RX_ADR_WIDTH 5
#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
//#define TX_PLOAD_WIDTH  20  // 20 bytes TX payload
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
// SPI(nRF24L01) commands
//#define READ_REG        0x00  // Define read command to register
#define WRITE_REGL       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

extern u16 Adc_DMA_Buf[4] ;

void
GAME_EventHandler1();
/*
void
GAME_EventHandler2();

void
GAME_EventHandler3();

void
GAME_Update();

void
GAME_Render();
*/
//void SET_NRF_CSN(GPIO_InitTypeDef  * GPIOx,unsigned char GPIO_PinValue, unsigned char value);
//void SET_NRF_CE(GPIO_InitTypeDef *GPIOx,unsigned char GPIO_PinValue, unsigned char value);
void Delay_count(int count);
void init_NRF24L01(void);
uint8_t nRF_Check( void );
uint8_t nRF_ReadReg( uint8_t ReadAddr );
uint8_t SPI_RW( SPI_TypeDef* SPIx, u8 WriteByte );
uint8_t SPI_ReadByte( SPI_TypeDef* SPIx );
void SPI_WriteByte( SPI_TypeDef* SPIx, u8 WriteByte );
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
void SPI_Read_Buf( uint8_t ReadAddr, uint8_t *ReadBuf, uint8_t Bytes );
void UART_Handler(void);
void SPI1_SendHandler(void);
void Background_draw(void);
//void Get_ADC_StringValue(u8 *X_String , u8 *Y_String , u8 *L_String, u8 *R_String , u16 string_head , u16 string_end);
void Get_StringValue(u8 *head_String , u8 *CombineString , u16 AdcDma_Buf , u16 string_head , u16 string_end);


#endif
