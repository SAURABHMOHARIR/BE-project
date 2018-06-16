/***********************************************************************/
/*                                                                     */
/*  FILE        :UART_comm.c                                           */
/*  DESCRIPTION :UART function file.                                   */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/
//In case of WORD data, if MSB byte is received 1st then it need to swap
//while mapping in union(structure of WORD and array of char) 
/***********************************************************************/
#include <p33FJ256GP710.h> 
#include "define.h"
#include "UART_Comm.h"
#include "PC_Comm.h"
#include "discon.h"
#include "GUI.h"


/***********************************************************************************
Function Name(ISR) : INT_UART1_Rcv
Description	  :	ISR for UART1 receive.
Parameters	  : none
Return value  : none
***********************************************************************************/


//***********************************************************************************
//***********************************************************************************

void __attribute__((__interrupt__,no_auto_psv)) _U2RXInterrupt(void)
{
 	
	IFS1bits.U2RXIF = 0;	//EdTR added clear interrupt flag
	UART2_Rx_Buff[UART2_Rx_Count++] = U2RXREG; //EdTR u0rb;
	
	if(!UART2_FLAG.StartOfReception)
	{
		if(UART2_Rx_Buff[0]==0x23)
			UART2_FLAG.StartOfReception = 1;
		else
			UART2_Rx_Count = 0;
	}		
	
	if(UART2_Rx_Count > (UART2_Rx_Buff[CMD2] + 4))// && ((UART2_Rx_Buff[CMD2]+3)=='!'))
	{
		UART2_FLAG.ResponseReceived = 1;
		UART2_FLAG.StartOfReception = 0;
		UART2_Timeout=0;
	}	
	
	if(UART2_Rx_Count>=(RX_DATA-1))
		UART2_Rx_Count=0;
		
	if(UART2_FLAG.StartOfReception)
		UART2_Timeout=0;	
	
	
	
}
// ***********************************************************************************
/***********************************************************************************
Function Name : UART2_SendByte
Description	  :	Transmits single byte on UART2.
Parameters	  : byte to trasmit
Return value  : none
***********************************************************************************/
void UART2_SendByte(uBYTE TxChar)
{
	U2TXREG = (uWORD)TxChar;		//EdTR	u0tb = 
	while(!U2STAbits.TRMT);				//EdTR	while(ti_u0c1==0);
}
/***********************************************************************************/

/***********************************************************************************
Function Name : UART2_SendString
Description	  :	Transmits string of byte byte on UART2.
Parameters	  : Pointer to string, No. of bytes to send
Return value  : none
***********************************************************************************/
void UART2_SendString(uBYTE *strng ,uBYTE char_count)
{
	while( char_count >0 )
	{
		UART2_SendByte( *strng );
		strng++;
		char_count--;		
	}
	
}
/***********************************************************************************/
/***********************************************************************************
Function Name : Put_Header
Description	  :	put header in Tx string
Parameters	  : header type
Return value  : none
***********************************************************************************/
void Put_Header(uBYTE cmd_id)
{
	UART2_Tx_Buff[CMD0] = STX;
	UART2_Tx_Buff[CMD1] = cmd_id;
	UART2_Tx_Count += 2;
}

/***********************************************************************************
Function Name : Put_Cmd_Header
Description	  :	put header in Tx string
Parameters	  : header type
Return value  : none
***********************************************************************************/
void Put_Cmd_Header(uBYTE Cmd_id)
{
	UART2_Tx_Buff[CMD0] = STX;
	UART2_Tx_Buff[CMD1] = Cmd_id;
	UART2_Tx_Buff[CMD2] = 'C';  //length
	UART2_Tx_Count += 3;
}

/***********************************************************************************
Function Name : Calculate_Checksum
Description	  :	calculate checksum
Parameters	  : string to calculate checksum, no. of bytes
Return value  : checksum
***********************************************************************************/
uBYTE Calculate_Checksum(uBYTE *str, uBYTE cnt)
{
		uBYTE	loop,cksum;
        loop=cksum=0;   
	
	for(loop=0;loop<cnt;loop++,str++)
	{
		cksum=cksum+(*str);
	}
	return cksum;
}
/***********************************************************************************/

/***********************************************************************************
Function Name : UART2_SendCommand
Description	  : Send command alongwith data.
Parameters	  : Command ID
Return value  : none
***********************************************************************************/
void UART2_SendCommand(uBYTE cmmnd_id)
{
	
	
	Command_Sent = cmmnd_id;
	UART2_Tx_Count = 0;
	Put_Header(cmmnd_id);
	
	
	UART2_Tx_Buff[UART2_Tx_Count] = Calculate_Checksum(UART2_Tx_Buff, UART2_Tx_Count);
	UART2_Tx_Count++;      
	UART2_Tx_Buff[UART2_Tx_Count++] = ETX;
	UART2_SendString(UART2_Tx_Buff, UART2_Tx_Count);
	Clear_Buff(UART2_Rx_Buff, RX_DATA);
	UART2_Rx_Count = 0;
	UART2_FLAG.ResponseReceived = 0;
	UART2_FLAG.StartOfReception = 0;
	uart_cnt = 0;
	UART2_FLAG.CommandDelay = 1;
	PC_Comm.MC_Command_Busy = 1;
	UART2_FLAG.Wait_For_Response = 1;
	
}

