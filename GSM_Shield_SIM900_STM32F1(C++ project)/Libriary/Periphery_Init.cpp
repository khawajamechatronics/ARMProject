/*==========================================================
This library will be Send and Get byte message from TX/RX, you need create Usart1_IRQHandler
PA[9]- TX
PA[10]- RX
==========================================================*/

#include "stm32f10x.h"
#include "Periphery_Init.h"

/*
// Handler for Usart(get data from GPS shield)
void USART1_IRQHandler(void)
{
			
}
*/

//============------------====== Initiall GpioA, Usart
//   * in_u32BaudRate   - value for calculate BoudRate
void InitPeriphery(uint32_t in_u32BaudRate)
{
   /*
   Port A:
   [9,10] - tx,rx for Usart1
   */
 
//------------------------------------------  GpioA
																		//Clocking port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
																		//PA[9] - output,Alternative function,pish-pull
																		//PA[10] - input,Hi-z
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	GPIOA->CRH |= GPIO_CRH_CNF9_1 |GPIO_CRH_MODE9_0 | GPIO_CRH_CNF10_0; 
   
//----------------------------------------------     USART
																		//Clocking USART and Alternative function
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;
																							//Calculate boud rate
	
  USART1->BRR = 0xA2C;//F_CPU / (float)in_u32BaudRate;                    
																							//On TxD and RxD
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
																							//Start USART module
  USART1->CR1 |= USART_CR1_UE;
																			//Allow interrupt recieve message RX
  USART1->CR1 |= USART_CR1_RXNEIE;
																			//external NVIC handler Usart1
  NVIC_EnableIRQ(USART1_IRQn);																			
}

//=======================================================  Send TX One Char Function
void USART1_Send(char chr)
{
																											//Verification of data transfer is complete
	while(!(USART1->SR & USART_SR_TC));
																												//Send Data
	USART1->DR = chr;
}

//=========================================================  Send TX string Function
void USART1_Send_String(char* str) 
{
 int i = 0;
																								//Perfomed while i < str.lenght
	while(str[i])
																													//Send symbol str[i], next cycle - i+1
		USART1_Send(str[i++]);	
}
