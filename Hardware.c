/***********************************************************************/
/*                                                                     */
/*  FILE        :hardware.c                                            */
/*  DESCRIPTION :Hardware intialisation file                           */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/

#include <p33FJ256GP710.h> 
#include "test.h"
#include "define.h"
#include "UART_Comm.h"
#include "discon.h"
#include "LCD_7981.h"

void Init_PortPins(void);
void Init_Hardware(void);
void Init_UART2(void);
void Init_UART1(void);
void Init_Timer2(void);
void Init_Timer3(void);
void Init_Timer4(void);
void Init_Variables(void);
void Initialise_GLCD(void);
void GLCD_clrscr_GRAPHICS(void);

volatile uBYTE Display_mode;
extern struct UART_FLAG UART2_FLAG;
/***********************************************************************************
Function Name : Init_Hardware
Description	  :	Initialise hardware.
Parameters	  : none
Return value  : none
***********************************************************************************/
void Init_Hardware(void)
{
	CORCONbits.IPL3 = 1;             //Interrupt disabled
	Init_PortPins();
	
	Init_Timer2();  //Ext Watchdog
	
	Init_UART2();	//MC-MC
	Init_UART1();	//MC-PC
	
	Init_Timer3();  //PWM backlight
	Init_Timer4();  //Keyboard
	
	Init_Variables();
	Initialise_GLCD();
	
	BKLIT_ON=1;	
	OC1RS=BKLT_PWM_HIGH;
	
	CORCONbits.IPL3 = 0;           //Interrupt enabled
}


/***********************************************************************************
Function Name : Init_PortPins
Description	  :	Configure the port pins of R8C2B CPU.
Parameters	  : none
Return value  : none
***********************************************************************************/
void Init_PortPins(void)
{	
	GLCD_TRIS 	= 0;		//All output
	E_TRIS		= 0;
	RW_TRIS		= 0;
	RS_TRIS		= 0;
	backlit_TRIS= 0;
	CS_TRIS		= 0; 
	RST_TRIS	= 0;

	GLCD_ODC 	= 0xFF00;	//Set ODC pins  1111 1111 0011 1110
	E_ODC 		= 1;
	RW_ODC 		= 1;
	RS_ODC 		= 1;
	CS_ODC 		= 1;
	RST_ODC 	= 1;
	
	RST_TMSi 	= 0;     	//RESET to TMS  
	RST_TMS 	= 1; 		//!!!!!!!!!!!!!!!!
	TMS_PRG_HOLDi=0;   	//EdTR pd5_3 //for bootloader programm mode in DSP
	TMS_PRG_HOLD= 1;
	
	
	KEY_PORTi 	= 0xFF;		//All input
	AD1PCFGH 	= 0xFF00;   //Pins set as digital
	
	LED1i=LED2i = 0;      	//OP
	LED1=0;LED2 = 0;

	EXT_WDi =0;			//Ext Watchdog as output
}
/***********************************************************************************
End of function Init_PortPins
***********************************************************************************/


/***********************************************************************************
Function Name : Init_UART1
Description	  :	Enables Peripheral Modules before use. mc-pc
Parameters	  : none
Return value  : none
***********************************************************************************/
void Init_UART1(void)
{
	U1BRG 			 = 103; //9600 @ 16 MIPS //EdTR 25Sep14 0x11;	//57600 @16MIPS    25; //38400 @ 16MIPS // EdTR 23Apr14 103;  //9600 @ 16 MIPS   
	IFS0bits.U1RXIF  = 0;		 //interrupt request flags are cleared 							 
	//EdTR U1MODE & U1STA are selected according to following commented settings
	U1MODE= 0x02;  //EdTR
	U1STA = 0x04;  //EdTR
	IEC0bits.U1RXIE  = 1;		 // Enable Recieve Interrupts	

}
//***********************************************************************************
//***********************************************************************************


/***********************************************************************************
Function Name : Init_UART2
Description	  :	Enables Peripheral Modules before use. mc-mc
Parameters	  : none
Return value  : none
***********************************************************************************/
void Init_UART2(void)
{
 	U2BRG 			 = 31;     //31250=>31 @ 16MIPS
	IFS1bits.U2RXIF  = 0;		//interrupt request flags are cleared 
	//EdTR U2MODE & U2STA are selected according to following commented settings
	U2MODE = 0x2;  //EdTR
	U2STA  = 0x200;
	IEC1bits.U2RXIE  = 1;		//Enable Recieve Interrupts
       		
}

//************************************************************************************
//**************************************
//	Timer4 Setting Keyboard
//**************************************
//************************************************************************************
void Init_Timer4(void)
{
	T4CON 		= 0;		//Turn off Timer2 by clearing control register
    TMR4 		= 0;		//Start Timer1 at zero
    
    PR4 		= 2000;		//2000 1ms delay (Prescaler 1:8) @ 16MIPS 
    
    T4CON 		= 0x8010;	//Configure Timer2 (timer on, continue in IDLE, not gated, 8:1 prescaler, internal clock)
    
    IFS1bits.T4IF= 0;		//Clear Timer1 interrupt flag
    IEC1bits.T4IE= 1;		//Enable Timer1 interrupt
}

