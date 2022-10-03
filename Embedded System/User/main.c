#include "stm32f10x.h"
#include "FONT.h"
#include "CFONT.h"
#include "IERG3810_LED.h"
#include "IERG3810_clock_tree_init.h"
#include "IERG3810_USART.h"
#include "stdio.h"
#include <string.h>
struct deckinfo{
u16 color;
int flip;
};
void Delay(u32);
void IERG3810_TFTLCD_Init(void);
void IERG3810_TFTLCD_SetParameter(void);
void IERG3810_TFTLCD_WrReg(u16);
void IERG3810_TFTLCD_WrData(u16);
void IERG3810_TFTLCD_DrawDot(u16, u16, u16 );
void IERG3810_TFTLCD_FillRectangle(u16 color, u16 start_x,u16 length_x, u16 start_y, u16 length_y);
void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit);
void IERG3810_TFTLCD_ShowChar(u16 x,u16 y,u8 ascii, u16 color, u16 bgcolor);
void IERG3810_PS2key_ExtiInit(void);
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup);
void EXTI15_10_IRQHandler(void);
void printscreen(struct deckinfo deck[10]);
void comparing(struct deckinfo deck[10], int number);
int checkwin(struct deckinfo deck[10]);
void EndGame(struct deckinfo deck[10]);
void Reset(struct deckinfo deck[10]);


u32 sheep=0;
u32 timeout=20000;
u16 ps2count=0;
u8 ps2key;
u8 ps2key_prev=0;
u8 task1HeartBeat;
u8 task2HeartBeat;
u8 Gamespeed;
unsigned char win [] = "Win!";
#define DS0_ON GPIOB->BRR = 1<<5;
#define DS1_ON GPIOE->BRR = 1<<5;
#define DS0_OFF GPIOB->BSRR = 1<<5;
#define DS1_OFF GPIOE->BSRR = 1<<5;
int cnt=0;
int update=1;
int lastselect=-1;
int page=0;
#define Yellow	0xFFE0;
/////////////////////////////////////////////////////////////////

void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL=0;
	SysTick->LOAD=72000 - 1;
	SysTick->CTRL|=0x00000003;//011
}

////////////////////////////////////////////////////////////////
void IERG3810_key2_ExtiInit(void){
    //KEY2 at PE2, EXTI-2, IRQ#8
    RCC->APB2ENR |= 1<<6;
    GPIOE->CRL &= 0xFFFFF0FF;
    GPIOE->CRL |= 0x00000800;
    GPIOE->ODR |= 1<<2;
    RCC->APB2ENR |= 0x01; //(RM0008 p.119)
    AFIO->EXTICR[0]&=0xFFFFF0FF; //p.185
    AFIO->EXTICR[0]|=0x00000400; //p.185
    EXTI->IMR |= 1<<2; //p.202
    EXTI->FTSR |= 1<<2; //p.203
    //EXTI->RTSR |= 1<<2; //p.203

    NVIC->IP[8]=0x65; //set priority of this interrupt. (D10337E p.8-4, 8-16)
    NVIC->ISER[0] |= (1<<8); // p.8-3
}
void IERG3810_keyUP_ExtiInit(void){
    //KEY UP at PA0, EXTI-0, IRQ#6
    RCC->APB2ENR |= 1<<2;
    GPIOA->CRL &= 0xFFFFFFF0;
    GPIOA->CRL |= 0x00000008;
    GPIOA->ODR |= 0<<0;
    RCC->APB2ENR |= 0x01; //(RM0008 p.119)
    AFIO->EXTICR[0]&=0xFFFFFFF0; //p.185
    AFIO->EXTICR[0]|=0x00000000; //p.185
    EXTI->IMR |= 1<<0; //p.202
    EXTI->FTSR |= 1<<0; //p.203
    //EXTI->RTSR |= 1<<0; //p.203

    NVIC->IP[6]=0x75; //set priority of this interrupt. (D10337E p.8-4, 8-16)
    NVIC->ISER[0] |= (1<<6); // p.8-3
}
void IERG3810_PS2key_ExtiInit(void){
    //PS2 Keyboard CLK at PC11, EXTI10:15, IRQ#40
    RCC->APB2ENR|=1<<4;
    GPIOC->CRH &= 0xFFFF0FFF;
    GPIOC->CRH |= 0x00008000;
    RCC->APB2ENR |= 0x01; 
    AFIO->EXTICR[2] &= 0xFFFF0FFF; 
    AFIO->EXTICR[2] |= 0x00002000;
    EXTI->IMR |= 1<<11;
    EXTI->FTSR |=1<<11;

    NVIC->IP[40] = 0x95; //set priority of this interrupt
    NVIC->ISER[1] |= (1<<8);

    //PS2 Keyboard DAT at PC10
    RCC->APB2ENR|=1<<4;
    GPIOC->CRH &= 0xFFFFF0FF;
    GPIOC->CRH |= 0x00000800;
}
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
    // Set PRIGROUP AIRCR[10:8]
    u32 temp, temp1;
    temp1=prigroup&0x00000007; //only concern 3 bits
    temp1 <<=8; 
    temp=SCB->AIRCR; //ARM DDI0337E p.8-22
    temp &= 0x0000F8FF;
    temp |= 0x05FA0000;
    temp |= temp1;
    SCB->AIRCR=temp;
}
void EXTI2_IRQHandler(void){
    u8 i;
    for(i=0;i<10;i++){
        DS0_ON;
        Delay(1000000);
        DS0_OFF;
        Delay(1000000);
    }
    EXTI->PR=1<<2; //clear this exception pending bit
}
void EXTI0_IRQHandler(void){
    u8 i;
    for(i=0;i<10;i++){
        DS1_ON;
        Delay(1000000);
        DS1_OFF;
        Delay(1000000);
    }
    EXTI->PR |= 1<<0;
}
void EXTI15_10_IRQHandler(void){
    // student design here
    if(ps2count>0 && ps2count <9){ //check the data bits
        ps2key |= ((GPIOC->IDR & 0x00000400)>>10)<<(ps2count-1);
    }
    ps2count++;
    Delay(10); //We found that the processor is too fast and get error.
    // A short delay can eliminate the error.
    EXTI->PR = 1<<11; //Clear this exception pending bit
	}
///////////////////////////////////////////////////////////////

void Delay(u32 count){
	
	u32 i;
	for (i=0; i<count; i++);
}

typedef struct
{
		u16 LCD_REG;
		u16 LCD_RAM;
}	LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD      ((LCD_TypeDef *) LCD_BASE)

void IERG3810_TFTLCD_Init(void){ 
	RCC->AHBENR |= 1<<8;     
	RCC->APB2ENR |= 1<<3;			
	RCC->APB2ENR |= 1<<5;			
	RCC->APB2ENR |= 1<<6;			
	RCC->APB2ENR |= 1<<8;			
	
	GPIOB->CRL &= 0xFFFFFFF0;	
	GPIOB->CRL |= 0x00000003;
	
	//PORTD
	GPIOD->CRH &= 0x00FFF000;
	GPIOD->CRH |= 0xBB000BBB;
	GPIOD->CRL &= 0xFF00FF00;
	GPIOD->CRL |= 0x00BB00BB;
	
	
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB;
	GPIOE->CRL &= 0x0FFFFFFF;
	GPIOE->CRL |= 0xB0000000;
	
	
	GPIOG->CRH &= 0xFFF0FFFF;
	GPIOG->CRH |= 0x000B0000;
	GPIOG->CRL &= 0xFFFFFFF0;
	GPIOG->CRL |= 0x0000000B;
	

	FSMC_Bank1->BTCR[6] = 0x00000000; 
	FSMC_Bank1->BTCR[7] = 0x00000000; 
	FSMC_Bank1E->BWTR[6]=0x00000000;  
	
	FSMC_Bank1->BTCR[6] |= 1<<12; 
	FSMC_Bank1->BTCR[6] |= 1<<14; 
	FSMC_Bank1->BTCR[6] |= 1<<4; 
	
	FSMC_Bank1->BTCR[7] |= 0<<28; 
	FSMC_Bank1->BTCR[7] |= 1<<0; 
	FSMC_Bank1->BTCR[7] |= 0xF<<8;
	
	FSMC_Bank1E->BWTR[6]|=0<<28;  
	FSMC_Bank1E->BWTR[6]|=0<<0; 
	FSMC_Bank1E->BWTR[6]|=3<<8;  
	
	FSMC_Bank1->BTCR[6] |= 1<<0; 
	
	IERG3810_TFTLCD_SetParameter();
	//LCD_LIGHT_ON; 
	GPIOB->BSRR |= 0x00000001;
}

void IERG3810_TFTLCD_SetParameter(void){
	
	IERG3810_TFTLCD_WrReg(0x01); 
	IERG3810_TFTLCD_WrReg(0x11); 
	
	IERG3810_TFTLCD_WrReg(0x3A); 
	IERG3810_TFTLCD_WrData(0x55); 
	
	IERG3810_TFTLCD_WrReg(0x29); 
	
	IERG3810_TFTLCD_WrReg(0x36); 
	IERG3810_TFTLCD_WrData(0xCA);
}

void IERG3810_TFTLCD_WrReg(u16 regval){
	
	LCD->LCD_REG=regval;
}
void IERG3810_TFTLCD_WrData(u16 data){
	LCD->LCD_RAM=data;
}

void IERG3810_TFTLCD_DrawDot(u16 x, u16 y,u16 color){

	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x&0xFF);
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0x3F);
	
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0xDF);
	
	IERG3810_TFTLCD_WrReg(0x2C);
	IERG3810_TFTLCD_WrData(color);
}

void IERG3810_TFTLCD_FillRectangle(u16 color,u16 start_x, u16 length_x, u16 start_y, u16 length_y){
	u32 index=0;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(start_x >> 8);
	IERG3810_TFTLCD_WrData(start_x &0xFF);
	IERG3810_TFTLCD_WrData((length_x + start_x -1) >>8);
	IERG3810_TFTLCD_WrData((length_x + start_x -1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(start_y >> 8);
	IERG3810_TFTLCD_WrData(start_y&0xFF);
	IERG3810_TFTLCD_WrData((length_y + start_y -1) >>8);
	IERG3810_TFTLCD_WrData((length_y + start_y -1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2C);
	for(index=0;index<length_x * length_y; index++)
  {
		IERG3810_TFTLCD_WrData(color);
	}
}

void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit)
{	
		if (digit==0 || digit==2 ||digit==3||digit==5||digit==6 ||digit==7 ||digit==8 ||digit==9)//a
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x+10,60,start_y+150,10);
		}
		if (digit==0 || digit==1 ||digit==2 ||digit==3||digit==4||digit==7 ||digit==8 ||digit==9)//b
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x+70,10,start_y+80,80);
		}
		if (digit==0 || digit==1 ||digit==3||digit==4||digit==5 ||digit==6 ||digit==7 ||digit==8 ||digit==9)//c
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x+70,10,start_y,80);
		}
	
	if (digit==0 || digit==2 ||digit==3||digit==5||digit==6 ||digit==8 ||digit==9)//d
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x+10,60,start_y,10);
		}
	
	if (digit==0 || digit==2 ||digit==6||digit==8)//E
	{
		IERG3810_TFTLCD_FillRectangle(color, start_x,10,start_y,80);
	}
		
	if (digit==0 || digit==4 ||digit==5||digit==6 ||digit==8 ||digit==9)//f
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x,10,start_y+80,80);
		}
	
		if ( digit==2 ||digit==3||digit==4||digit==5||digit==6 ||digit==8 ||digit==9)//g
		{
			IERG3810_TFTLCD_FillRectangle(color, start_x+10,60,start_y+70,10);
		}

}


void IERG3810_TFTLCD_ShowChar(u16 x,u16 y,u8 ascii, u16 color, u16 bgcolor)
	{
	u8 i,j;
	u8 index;
	u8 height = 16, length = 8;
	if(ascii < 32 || ascii > 127) return;
	ascii -= 32;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData((length + x - 1)>>8);
	IERG3810_TFTLCD_WrData((length + x - 1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData((height + y - 1)>>8);
	IERG3810_TFTLCD_WrData((height + y - 1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2C);
	
	for(j=0; j<height/8; j++){
		
		for(i=0; i<height/2; i++){
			
			for(index=0; index<length;index++)
			{
				
				if((asc2_1608[ascii][index*2+1-j]>>i) & 0x01)
					IERG3810_TFTLCD_WrData(color);
			  else
					IERG3810_TFTLCD_WrData(bgcolor);
				
			}
		}
	}
	
}

void IERG3810_TFTLCD_ShowSentence(u16 x,u16 y,u8* ascii,u8 length, u16 color, u16 bgcolor)
{
		int i;
	  for(i=0x0; i < length; i++){
			IERG3810_TFTLCD_ShowChar(x+ i*8 ,y,ascii[i],color,bgcolor);
		}

}

void IERG3810_TFTLCD_ShowChinChar(u16 x,u16 y,u8 chi, u16 color, u16 bgcolor)
	{
	u8 i,j;
	u8 index;
	u8 height = 16, length = 16;

	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData((length + x - 1)>>8);
	IERG3810_TFTLCD_WrData((length + x - 1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData((height + y - 1)>>8);
	IERG3810_TFTLCD_WrData((height + y - 1) & 0xFF);
	
	IERG3810_TFTLCD_WrReg(0x2C);
	
	for(j=0; j<height/8; j++){
		
		for(i=0; i<height/2; i++){
			
			for(index=0; index<length;index++)
			{
				
				if((chi_1616[chi][index*2+1-j]>>i) & 0x01)
					IERG3810_TFTLCD_WrData(color);
			  else
					IERG3810_TFTLCD_WrData(bgcolor);
				
			}
		}
	}
	
}
	


	



void ShowCard0(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,30, 50, 15, 75);
}

void ShowCard1(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,95, 50, 15, 75);
}

void ShowCard2(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,160, 50, 15, 75);
}


void ShowCard3(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,30, 50, 105, 75);
}

void ShowCard4(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,95, 50, 105, 75);
}

void ShowCard5(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,160, 50, 105, 75);
}

void ShowCard6(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,12, 50, 195, 75);
}

void ShowCard7(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,67, 50, 195, 75);
}

void ShowCard8(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,122, 50, 195, 75);
}

void ShowCard9(u16 color){
	IERG3810_TFTLCD_FillRectangle(color,177, 50, 195, 75);
	
}



u8 sid1[10]={49,49,53,53,49,50,55,51,56,50};
u8 sid2[10]={49,49,53,53,49,50,53,49,54,57};
u8 chi1[10]={0,1,2};
u8 chi2[10]={3,4,5};
unsigned char start [] = "Press 0 multiple times to shuffle and start!";


unsigned char instruction1 [] = "Flip the card by pressing the";
unsigned char instruction2 [] = "corresponding card number!";


void ShowAllClosedCard(){
	IERG3810_TFTLCD_ShowSentence(5,300,instruction1,30, 0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowSentence(20,275,instruction2,26, 0xFFFF, 0x0000);
	ShowCard0(0xFFFF);
	ShowCard1(0xFFFF);
	ShowCard2(0xFFFF);
	ShowCard3(0xFFFF);
	ShowCard4(0xFFFF);
	ShowCard5(0xFFFF);
	ShowCard6(0xFFFF);
	ShowCard7(0xFFFF);
	ShowCard8(0xFFFF);
	ShowCard9(0xFFFF);
	IERG3810_TFTLCD_ShowChar(50, 50,48,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(115, 50,49,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(180, 50,50,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(50, 140,51,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(115, 140,52,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(180, 140,53,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(30, 230,54,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(85, 230,55,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(140, 230,56,0x0000, 0xFFFF);
	IERG3810_TFTLCD_ShowChar(192, 230,57,0x0000, 0xFFFF);
	
}


	
void IERG3810_TFTLCD_ShowChinSentence(u16 x,u16 y,u8* chi,u8 length, u16 color, u16 bgcolor)
{
		int i;
	  for(i=0; i < length; i++){
			IERG3810_TFTLCD_ShowChinChar(x+ i*16 ,y,chi[i],color,bgcolor);
		}

}


void ShowStartingPage(){
	
	IERG3810_TFTLCD_FillRectangle(0,0xFFFF,0,400,400);
	IERG3810_TFTLCD_ShowSentence(25,75,sid1,10, 0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowSentence(25,50,sid2,10, 0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowChinSentence(125,75,chi1, 3,0xFFFF, 0x0000);
	IERG3810_TFTLCD_ShowChinSentence(125,50,chi2, 3,0xFFFF, 0x0000);
	IERG3810_TFTLCD_FillRectangle(0x7FFF,50, 50, 195, 75);
	IERG3810_TFTLCD_FillRectangle(0xF800,75, 50, 170, 75);
	IERG3810_TFTLCD_FillRectangle(0xFFE0,125, 50, 195, 75);
	IERG3810_TFTLCD_FillRectangle(0xF81F,150, 50, 170, 75);
	
	IERG3810_TFTLCD_FillRectangle(0x07E0,25, 175, 120, 25);
	
	IERG3810_TFTLCD_ShowSentence(30,125,start,21, 0x0000, 0x07E0);
}

void keyboard_control(struct deckinfo deck[10]){

			if(ps2count >= 11)
			{
				//EXTI->IMR &= ~(1<<11); optional, suspend interrupt
				//--student design program here
				if(ps2key_prev==0xF0&&ps2key==0x70){ 
						
					
					comparing(deck, 0);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x69){ 
						
					
					comparing(deck, 1);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x72){ 
						
					
					comparing(deck, 2);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x7A){ 
						
					
					comparing(deck, 3);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x6B){ 
						
					
					comparing(deck, 4);
					update=1;
			  }
				else if(ps2key_prev==0xF0&&ps2key==0x73){ 
						
					
					comparing(deck, 5);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x74){ 
						
					
					comparing(deck, 6);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x6C){ 
						
					
					comparing(deck, 7);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x75){ 
						
					
					comparing(deck, 8);
					update=1;
				}
				else if(ps2key_prev==0xF0&&ps2key==0x7D){ 
						
					
					comparing(deck, 9);
					update=1;
				}
				ps2key_prev = ps2key;
				ps2count=0;
				ps2key=0;
				EXTI->PR = 1 << 11; //Clear this exception pending bit
				//EXTI->IMR |=(1<11) ; optional, resume interrupt
			}//end of "if PS2 keyboard received data correctly"
			timeout--;
			if(timeout == 0)//clear PS2 keyboard data when timeout
			{
				timeout = 20000;
				ps2key=0;
				ps2count=0;
	
			}//end of "clear PS2 keyboard data when timeout"
}



int cardshuffle (struct deckinfo deck[10])
{
int shuffletime=0;
int cnttmp=0;
u16 colortmp;
	IERG3810_TFTLCD_FillRectangle(0x0000,0x0000,240,0,320);
printscreen(deck);
while(shuffletime<10){
	cnttmp++;
if(ps2count >= 11){
if(ps2key_prev==0xF0&&ps2key==0x70){
						colortmp=deck[cnttmp%10].color;
						deck[cnttmp%10].color=deck[shuffletime].color;
						deck[shuffletime].color=colortmp;
						shuffletime++;
				}

				ps2key_prev = ps2key;
				ps2count=0;
				ps2key=0;
				EXTI->PR = 1 << 11; //Clear this exception pending bit
				//EXTI->IMR |=(1<11) ; optional, resume interrupt
			}//end of "if PS2 keyboard received data correctly"
			timeout--;
			if(timeout == 0)//clear PS2 keyboard data when timeout
			{
				timeout = 20000;
				ps2key=0;
				ps2count=0;
	
			}
}
update=1;
page=1;
return 0;
}

void printscreen(struct deckinfo deck[10])
{
	if (update==1 && page==0)
	{
		ShowStartingPage();
	}
	
else if (update==1 && page==1){
	IERG3810_TFTLCD_FillRectangle(0x0000,0x0000,240,0,320);
		ShowAllClosedCard();
		if (deck[9].flip==1){ShowCard9(deck[9].color);}
		if (deck[8].flip==1){ShowCard8(deck[8].color);}
		if (deck[7].flip==1){ShowCard7(deck[7].color);}
		if (deck[6].flip==1){ShowCard6(deck[6].color);}
		if (deck[5].flip==1){ShowCard5(deck[5].color);}
		if (deck[4].flip==1){ShowCard4(deck[4].color);}
		if (deck[3].flip==1){ShowCard3(deck[3].color);}
		if (deck[2].flip==1){ShowCard2(deck[2].color);}
		if (deck[1].flip==1){ShowCard1(deck[1].color);}
		if (deck[0].flip==1){ShowCard0(deck[0].color);}
			update=0;
		}
	else if (update==1 && page==2)
	{		IERG3810_TFTLCD_FillRectangle(0x0000,0x0000,240,0,320);
		ShowAllClosedCard();
		if (deck[9].flip==1){ShowCard9(deck[9].color);}
		if (deck[8].flip==1){ShowCard8(deck[8].color);}
		if (deck[7].flip==1){ShowCard7(deck[7].color);}
		if (deck[6].flip==1){ShowCard6(deck[6].color);}
		if (deck[5].flip==1){ShowCard5(deck[5].color);}
		if (deck[4].flip==1){ShowCard4(deck[4].color);}
		if (deck[3].flip==1){ShowCard3(deck[3].color);}
		if (deck[2].flip==1){ShowCard2(deck[2].color);}
		if (deck[1].flip==1){ShowCard1(deck[1].color);}
		if (deck[0].flip==1){ShowCard0(deck[0].color);}
		EndGame(deck);
		update=0;
	}
}

void comparing(struct deckinfo deck[10], int number)
{
	
if (lastselect==-1 && deck[number].flip==0){//no selected
	
	lastselect=number;
	deck[number].flip=1;
	}
else if (deck[number].flip==1 || lastselect==number)//selected and same
{
	return;}
else if(deck[lastselect].color!=deck[number].color)//not same color
	{	deck[number].flip=1;
		update=1;
		printscreen(deck);
		task1HeartBeat=0;
		while(task1HeartBeat<33){;}
		//GPIOE->ODR ^=1<<5;
		deck[lastselect].flip=0;
		deck[number].flip=0;
			lastselect=-1;	
}
	else {//color match and flip
		deck[number].flip=1;
	deck[lastselect].flip=1;
		lastselect=-1;	
	}
}

int checkwin(struct deckinfo deck[10])
{int i;
for (i=0;i<10;i++){
if (deck[i].flip==0)
	return 0;
}
return 1;
}

unsigned char end [] = "Finished !!! Please check ";
unsigned char end1 [] = "your result below:";
unsigned char Result [10]= "";
int time=0;

void EndGame(struct deckinfo deck[10]){
	int tmp=0;
	IERG3810_TFTLCD_FillRectangle(0x95D1,0, 1000, 150, 75);
	IERG3810_TFTLCD_ShowSentence(12,195,end,26, 0xFFFF, 0x95D1);
	IERG3810_TFTLCD_ShowSentence(20,175,end1,17, 0xFFFF, 0x95D1);
	sprintf(Result,"%d",time);
	strcat(Result," s");
	IERG3810_TFTLCD_ShowSentence(20,155,Result ,8, 0xF800, 0x95D1);
	USART_print(1,Result);
	DS0_ON;
	while(1){
	if(ps2count >= 11){
	if(ps2key_prev==0xF0&&ps2key==0x5A){
		Reset(deck);
		break;
				}

				ps2key_prev = ps2key;
				ps2count=0;
				ps2key=0;
				EXTI->PR = 1 << 11; //Clear this exception pending bit
				//EXTI->IMR |=(1<11) ; optional, resume interrupt
			}//end of "if PS2 keyboard received data correctly"
			timeout--;
			if(timeout == 0)//clear PS2 keyboard data when timeout
			{
				timeout = 20000;
				ps2key=0;
				ps2count=0;
	
			}
}
}

void Reset(struct deckinfo deck[10]){
	int i;
	task2HeartBeat=0;
	for (i=0; i<10; i++){
		deck[i].flip=0;
	}
	update=1;
	lastselect=-1;
	page=0;
	time=0;
	DS0_OFF;
	DS1_OFF;
	cardshuffle(deck);


}
int main(void){
	int digit=10;
	
	u16 color[10]={0xFFE0,0xFFE0,0x7FFF,0x7FFF,0xF800,0xF800,0x07E0,0x07E0,0xF81F,0xF81F};
	struct deckinfo deck[10]={{0xFFE0,0},{0xFFE0,0},{0x7FFF,0},{0x7FFF,0},{0xF800,0},{0xF800,0},{0x07E0,0},{0x07E0,0},{0xF81F,0},{0xF81F,0}};
	//shuffle the array
	
	IERG_USART1_init(72,9600);
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_PS2key_ExtiInit();
	IERG3810_TFTLCD_Init();
	IERG3810_TFTLCD_FillRectangle(0x0000,0x0000,240,0,320);
	IERG3810_SYSTICK_Init10ms();
	DS0_OFF;
	DS1_OFF;
	while(task1HeartBeat<33){;}
	cardshuffle(deck);
	task2HeartBeat=0;
	while(1)
	{	
			if (task2HeartBeat>=120)//control time
			{
				time+=1;
				task2HeartBeat=0;
			}
			printscreen(deck);
      keyboard_control(deck); 
			if (checkwin(deck)==1)
			{	update=1;
				page=2;
			}
	
		
		
}}