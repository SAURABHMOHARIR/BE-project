/***********************************************************************/
/*                                                                     */
/*  FILE        :Keyboard.c                                            */
/*  DESCRIPTION :Keyboard Routine file                                 */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/
#include <p33FJ256GP710.h> 
#include "test.h"
#include "define.h"
#include "UART_Comm.h"
#include "Discon.h"
#include "GUI.h" 
#include "LCD_7981.h"
#include "PC_Comm.h"

#define DEBOUNCE_TIME	20     //EdTR 5 !!!!!!!!!!!!!

//***********************************************************************************
//***********************************************************************************
volatile uBYTE key_detect, old_keycode, key_code, key_dbnc_counter, Temp_KeyBoard;
volatile uBYTE backlight_on;
volatile uWORD ON_TIME = 64000;

volatile uBYTE Dis_Menu, Dis_Sub_Menu, Dis_DN_Cnt, Dis_UP_Cnt, Dis_RT_Cnt;
volatile uBYTE Hor_Scrl_Cnt, Ver_Scrl_Cnt;
volatile uBYTE Hor_Wnd_Scrl, Ver_Wnd_Scrl;
volatile uBYTE MainMenu_Cusr_Scrl;
uBYTE MainMenu_Scrl_Cnt;
volatile uBYTE SubMenu_Ver_Scrl,SubMenu_Scrl_Cnt;

//***********************************************************************************
//***********************************************************************************
uBYTE Scan_Keyboard(void);
void Change_Menu_Count(void);
void Edit_Parameters(void);
void Chk_UP(void);
void Chk_DN(void);
void Release_Key(void);
void Change_PWD(void);
void Edit_Change_PWD(void);


/***********************************************************************************
Function Name(ISR) : INT_Timer4 1ms
Description	  :	Timer ISR for timer4 in output compare mode
Parameters	  : none
Return value  : none
 ***********************************************************************************/

void __attribute__((__interrupt__, auto_psv)) _T4Interrupt(void)//ISR
{
    //intrrrupt on 1ms 1000 counts for 1 sec
    IFS1bits.T4IF = 0; //Clear Timer4 interrupt flag

    Scan_Keyboard();
    if ((key_dbnc_counter == 0) && (old_keycode != NO_KEY))
    {
        Temp_KeyBoard = old_keycode;
        key_dbnc_counter++;
    }
    else
    {
        if (Temp_KeyBoard == old_keycode)
        {
            key_dbnc_counter++;
            if (key_dbnc_counter > DEBOUNCE_TIME)
            {
                key_code = old_keycode;
                key_detect = 1;
                PWD_Cntr = 0;
                key_dbnc_counter = 0;
            }
        }
        else
        {
            key_code = NO_KEY;
            key_dbnc_counter = 0;
        }
    }

   
}
/***********************************************************************************/

/***********************************************************************************
Function Name : Scan_Keyboard
Description	  :	Timer ISR for timer C in output compare mode
Parameters	  : none
Return value  : none
 ***********************************************************************************/
uBYTE Scan_Keyboard(void)
{
    if (key_detect == 1)
        return 0;

    KEYBOARD = PORTE;

   
}
//***********************************************************************************
//***********************************************************************************   

void Get_Key(void)
{
    if (SCR_FLAG.Read_On_Reset)
        return;

    key_detect = 0;

    if (key_code == NO_KEY)
    {
       
        return;
    }

   
    Change_Menu_Count();
   
}
//***********************************************************************************
//***********************************************************************************   

void Change_Menu_Count(void)
{
    uBYTE i;

	
    switch (key_code)
    {
    case DN:
        if (SCR_FLAG.Close_Key_Confirm)
            break;

        if (KEYBRD_FLAG.BYTE.EditMode)
        {
            Edit_Parameters();
            break;
        }

      
                if ((Dis_Sub_Menu > 0) && (Dis_Sub_Menu < 5)) 
					Max_Cnt = I2OC_MENU_CNT - 1;
                if (Dis_Sub_Menu == 5) 
					Menu_DIDO_DN_Key(I2OC_SWDI_CNT);

				SubMenu_Ver_Scrl++;
				if(SubMenu_Ver_Scrl>=NoOf_SubMenu_Disp) 
				{
					SubMenu_Ver_Scrl=(NoOf_SubMenu_Disp-1);
							
					SubMenu_Scrl_Cnt++;
					if(SubMenu_Scrl_Cnt>(Max_Cnt - NoOf_SubMenu_Disp + 1))
						SubMenu_Scrl_Cnt = Max_Cnt - NoOf_SubMenu_Disp + 1;
				}
      
        break;

    case UP:
       

      

                Max_Cnt = 1;
				if(SubMenu_Ver_Scrl != 0)
					SubMenu_Ver_Scrl--;
				else
				{
					if(SubMenu_Scrl_Cnt != 0)
						SubMenu_Scrl_Cnt--;
				}

             
            }
            break;

      
    case RT:
      
                Dis_Sub_Menu++;
          
            break;

      

    case LT:
      
                if (Dis_Sub_Menu != 0)
                    Dis_Sub_Menu--;
        break;

    case ENT:
     
                KEYBRD_FLAG.BYTE.EditMode = ~KEYBRD_FLAG.BYTE.EditMode;

     
        case Conf_Menu:
            if (Dis_Sub_Menu == CONF_GEN)
            {
              

                KEYBRD_FLAG.BYTE.EditMode = ~KEYBRD_FLAG.BYTE.EditMode;
            }
            if ((Dis_Sub_Menu == CONF_PH_CT) || (Dis_Sub_Menu == CONF_ER_CT) 
          
                    UART2_SendCommand(WR_CTVT_SET);
              
            }
           
		
	break;

        
		
}//end change_menu_count
//***********************************************************************************
//***********************************************************************************



//***********************************************************************************
//***********************************************************************************

void Change_XCBR_Setting(void)
{
    uBYTE CTLMOD[3] = {0, 1, 4};
    uBYTE R_CTLMOD[5] = {0, 1, 2, 0, 2};
    uBYTE i;
    switch (Dis_DN_Cnt)
    {
    case 0x00:
        Temp_XCBR.Enable = ~Temp_XCBR.Enable;
        Release_Key();
        break;
    case 0x01:
        Temp_XCBR.Timeout = Change_WordVal(Temp_XCBR.Timeout, 300, 10);
        break;
    case 0x02:
        Temp_XCBR.Pulse_Len = Change_Val_1D(Temp_XCBR.Pulse_Len, 600, 1);
        break;
    //case 0x03:
    //    Temp_XCBR.Event_Dly = Change_Val_2D(Temp_XCBR.Event_Dly, 10000, 0);
    //    break;
    case 0x03:
        i = R_CTLMOD[Temp_XCBR.Ctl_Model];
        i = Change_ByteVal(i, 2, 0);
        Temp_XCBR.Ctl_Model = CTLMOD[i];
        Release_Key();
        break;
    case 0x04:
        Release_Key();
        if (Temp_XCBR.RESET_SERVER == 0)
            Temp_XCBR.RESET_SERVER = 1;
        else
            Temp_XCBR.RESET_SERVER = 0;
        break;
    default: break;
    }
}
//***********************************************************************************
//***********************************************************************************
void Change_IP_Setting(void)
{
	switch(Dis_DN_Cnt)
	{
	case 0:
		if((Dis_RT_Cnt==0)||(Dis_RT_Cnt==4)||(Dis_RT_Cnt==8)||(Dis_RT_Cnt==12))
			DIS_SERVER_IP[Dis_RT_Cnt] = Change_ByteVal(DIS_SERVER_IP[Dis_RT_Cnt], 0x32, 0x30);
		else
			DIS_SERVER_IP[Dis_RT_Cnt] = Change_ByteVal(DIS_SERVER_IP[Dis_RT_Cnt], 0x39, 0x30);
		Release_Key();
	break;
	
	case 1:
		if((Dis_RT_Cnt==0)||(Dis_RT_Cnt==4)||(Dis_RT_Cnt==8)||(Dis_RT_Cnt==12))
			DIS_NET_MASK[Dis_RT_Cnt] = Change_ByteVal(DIS_NET_MASK[Dis_RT_Cnt], 0x32, 0x30);
		else
			DIS_NET_MASK[Dis_RT_Cnt] = Change_ByteVal(DIS_NET_MASK[Dis_RT_Cnt], 0x39, 0x30);
		Release_Key();
	break;
	case 2:
		if((Dis_RT_Cnt==0)||(Dis_RT_Cnt==4)||(Dis_RT_Cnt==8)||(Dis_RT_Cnt==12))
			PRI_DIS_SNTP_SER[Dis_RT_Cnt] = Change_ByteVal(PRI_DIS_SNTP_SER[Dis_RT_Cnt], 0x32, 0x30);
		else
			PRI_DIS_SNTP_SER[Dis_RT_Cnt] = Change_ByteVal(PRI_DIS_SNTP_SER[Dis_RT_Cnt], 0x39, 0x30);
		Release_Key();
	break;
	case 3:
		if((Dis_RT_Cnt==0)||(Dis_RT_Cnt==4)||(Dis_RT_Cnt==8)||(Dis_RT_Cnt==12))
			SEC_DIS_SNTP_SER[Dis_RT_Cnt] = Change_ByteVal(SEC_DIS_SNTP_SER[Dis_RT_Cnt], 0x32, 0x30);
		else
			SEC_DIS_SNTP_SER[Dis_RT_Cnt] = Change_ByteVal(SEC_DIS_SNTP_SER[Dis_RT_Cnt], 0x39, 0x30);
		Release_Key();
	break;
	case 4:
		if((Dis_RT_Cnt==0)||(Dis_RT_Cnt==4)||(Dis_RT_Cnt==8)||(Dis_RT_Cnt==12))
			DIS_GATEWAY[Dis_RT_Cnt] = Change_ByteVal(DIS_GATEWAY[Dis_RT_Cnt], 0x32, 0x30);
		else
			DIS_GATEWAY[Dis_RT_Cnt] = Change_ByteVal(DIS_GATEWAY[Dis_RT_Cnt], 0x39, 0x30);
		Release_Key();
	break;
	case 5:
		if(Dis_RT_Cnt==0)
		{
			if(DIS_TIME_ZONE[Dis_RT_Cnt]=='-')
				DIS_TIME_ZONE[Dis_RT_Cnt]='+';
			else
				DIS_TIME_ZONE[Dis_RT_Cnt]='-';
		}
			
		if(Dis_RT_Cnt==1)
			DIS_TIME_ZONE[Dis_RT_Cnt] = Change_ByteVal(DIS_TIME_ZONE[Dis_RT_Cnt], 0x31, 0x30);
		if(Dis_RT_Cnt==2)
			DIS_TIME_ZONE[Dis_RT_Cnt] = Change_ByteVal(DIS_TIME_ZONE[Dis_RT_Cnt], 0x39, 0x30);
		if(Dis_RT_Cnt==4)
			DIS_TIME_ZONE[Dis_RT_Cnt] = Change_ByteVal(DIS_TIME_ZONE[Dis_RT_Cnt], 0x36, 0x30);
		if(Dis_RT_Cnt==5)
			DIS_TIME_ZONE[Dis_RT_Cnt] = Change_ByteVal(DIS_TIME_ZONE[Dis_RT_Cnt], 0x39, 0x30);
		
		Release_Key();
	break;
	}
}
