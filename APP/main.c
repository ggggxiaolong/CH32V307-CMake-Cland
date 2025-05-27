/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"


/* Global typedef */

/* Global define */

/* Global Variable */
void GPIO_INIT(void)
{
	GPIO_InitTypeDef def;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	def.GPIO_Mode = GPIO_Mode_Out_PP;
	def.GPIO_Pin = GPIO_Pin_4;
	def.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &def);
}



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");
	GPIO_INIT();
	// GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);
	while(1)
    {
		printf("Hello World!\r\n");
		Delay_Ms(200);
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0);
		Delay_Ms(200);
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1);
	}
}

