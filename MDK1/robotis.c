#include <robotis.h>
#include <stdio.h>
#include "gpio.c"
#include "uart.c"








void Setup()
{
	GPIO_InitTypeDef GPIO_Init_Structure;

	// PORTC Initialize
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin7|GPIO_Pin5|GPIO_Pin3;
	GPIO_Init_Structure.GPIO_PU = 0;
	GPIO_Init_Structure.GPIO_Mode = OUT;
	GPIO_Init(PORTC,&GPIO_Init_Structure);

	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin0|GPIO_Pin1|GPIO_Pin2|GPIO_Pin4|GPIO_Pin6;
	GPIO_Init_Structure.GPIO_Mode = IN;
	GPIO_Init(PORTC,&GPIO_Init_Structure);

	

	// PORT B Initialize, PB7 Output for TxD
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin7;
	GPIO_Init_Structure.GPIO_Mode = OUT;
	GPIO_Init(PORTB,&GPIO_Init_Structure);


	// USART Initialize

	USART_InitTypeDef	USART_InitStruct;

	USART_InitStruct.USART_BaudRate = BAUD_115200;
	USART_InitStruct.USART_DataLength = USART_DataLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

	USART_Init(&USART_InitStruct);		// Init UART
	//USART_ITConfig(USART_IT_RXNE, ENABLE);	// Interrupt Enable
	USART_Cmd(ENABLE);					// UART Start
}


void Loop()
{
	GPIO_SetBits(PORTC, GPIO_Pin7);
	USART_SendString((unsigned char*) "On");
	delay_ms(500);
	GPIO_ResetBits(PORTC, GPIO_Pin7);
	USART_SendString((unsigned char*) "Off");
	delay_ms(500);	
}


void main()
{
	Setup();
	while(TRUE)
	{
		//TODO: User Code
		Loop();
	}
}
