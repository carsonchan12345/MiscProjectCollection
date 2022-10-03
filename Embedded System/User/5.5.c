#include "stm32f10x.h"
#include "IERG3810_clock_tree_init.h"
#include "IERG3810_LED.h"
#include "global.h"
u8 task1HeartBeat=0;
u8 task2HeartBeat=0;
void DS0_turnOff();
void DS0_turnOff2();
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007; 
	temp1 <<= 8; 
	temp = SCB->AIRCR;
	temp &= 0X0000F8FF;
  temp = 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}
void IERG3810_TIM3_Init(u16 arr,u16 psc){
    RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->DIER|=1<<0;
	TIM3->CR1|=0x01;
	NVIC->IP[29]=0x45;
	NVIC->ISER[0]|=(1<<29);
}
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 1<<0)
	{
		if (GPIOB->ODR & 1<<5)
			GPIOB->ODR ^= 1<<5;
		else
			//DS0_turnOff(); 
		//GPIOB->BSRR = 1<<5;
		DS0_turnOff2(); 
	}
	TIM3->SR &= ~(1<<0);
}
void IERG3810_TIM4_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR|=1<<2;//enable TIM4
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1<<0;
	TIM4->CR1|=0x01;
	NVIC->IP[30]=0x45;
	NVIC->ISER[0]|=(1<<30);
}
void TIM4_IRQHandler(void)
{
	if(TIM4->SR & 1<<0)
	{
		 GPIOE->ODR ^= 1<<5;
	}
	TIM4->SR &= ~(1<<0);
}

void DS0_turnOff(){
 
    GPIOB->BSRR = 1<<5;
}
void DS0_turnOff2(){
    
    DS0_turnOff(); 
}
void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL=0;
	SysTick->LOAD=72000 - 1;
	SysTick->CTRL|=0x00000003;//011
}
void IERG3810_TIM3_PwmInit(u16 arr, u16 psc)
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00B00000;
	RCC->APB2ENR|=1<<0;
	AFIO->MAPR&=0xFFFFF3FF;
	AFIO->MAPR|=1<<11;
	RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR1|=7<<12;
	TIM3->CCMR1|=1<<11;
	TIM3->CCER|=1<<4;
	TIM3->CR1=0x0080;
	TIM3->CR1|=0x01;
}
int main(void){
    IERG3810_clock_tree_init();
    IERG3810_LED_Init();
    IERG3810_NVIC_SetPriorityGroup(5); 
    //IERG3810_TIM3_Init(4999,7199); 
		//IERG3810_TIM4_Init(4999,1799);
	IERG3810_SYSTICK_Init10ms();
				

    while(1){
			if (task1HeartBeat >= 20)
			{
				task1HeartBeat=0;	
				GPIOB->ODR^=1<<5;
			}
						if (task2HeartBeat >= 33)
			{
				task2HeartBeat=0;	
				GPIOE->ODR^=1<<5;
			}
    }
}