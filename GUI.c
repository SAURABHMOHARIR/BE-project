/***********************************************************************/
/*                                                                     */
/*  FILE        :GUI.c                      	                       */
/*  DESCRIPTION :Graphic user Interface file                           */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/
#include "define.h"
#include "Discon.h"
#include "GUI.h"
//#include "LCD.h"
#include "LCD_7981.h"
#include "test.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "UART_comm.h"

#define MIN_DIS_CURR	0.04
#define MIN_DIS_VOLT	1


/***********************************************************************************
Function Name : DispAll()
Description	  :	Displays various menus according to key operation
Parameters	  : none
Return value  : none
 ***********************************************************************************/
void DispAll(void)
{

	if(KEYBRD_FLAG.BYTE.EditMode)
		KEYBRD_FLAG.BYTE.ClrScr=0;

    if (SCR_FLAG.Read_On_Reset)
        return;

    if (SET_SKIP)
        KEYBRD_FLAG.BYTE.EditMode = 0;

    if(SCR_FLAG.Close_Key_Confirm)
    {
        Show_CLS_Confirm();
        return;
    }

#if	DEBUG	
    GLCD_Fill_Line(Line4, 28, '#');
#endif

    switch (Dis_Menu)
    {
    case Info_Menu:
        Main_Window();
        break;
    case Curr_Menu:
        Current_Mesr_Window();
        break;
	case Conf_Menu:
        Config_Setting_Window();
        break;
	case BD_Menu:
        BD_Setting_Window(); //bias diff.
        break;
	case THRM_Menu:
        THRM_Setting_Window();
        break;
    case OC_Menu:
        OC_Setting_Window();
        break;
    case OCHS_Menu:
        OCHS_Setting_Window();
        break;
    case Io_Menu:
        Io_Setting_Window();
        break;
    case IoHS_Menu:
        IoHS_Setting_Window();
        break;
    case I2OC_Menu:
        I2OC_Setting_Window();
        break;
    case I2HS_Menu:
        I2HS_Setting_Window();
        break;
    case UC_Menu:
        UC_Setting_Window();
        break;
	case SUPR_Menu:
        SUPR_Setting_Window();
        break;
	case DR_Menu:
        DistRecord_Window();
        break;
	case COMM_Menu:
        COMM_Setting_Window();
        break;
    case Fault_Menu:
        Fault_Record_Window();
        break;
    case Event_Menu:
		EventRecord_Window();
		break;
	case DIDO_Menu:
        DIDO_Window();
        break;
    
    case Calib_Menu:
        Calib_Window();
        break;


    //case Volt_Menu:
    //    Voltage_Mesr_Window();
    //    break;
#if VERSION_B		
    case Power_Menu:
        Power_Mesr_Window();
        break;
    case Harm_Menu:
        Harmonics_Window();
        break;
#endif		
    case REF_Menu:
        REF_Setting_Window();
        break;
    case BEF_Menu:
        BEF_Setting_Window();
        break;
    case OFLX_Menu:
        OFLX_Setting_Window();
        break;
    case Ig_Menu:
        Ig_Setting_Window();
        break;
    case IgHS_Menu:
        IgHS_Setting_Window();
        break;
    
    
    
    case OV_Menu:
        OV_Setting_Window();
        break;
    case UV_Menu:
        UV_Setting_Window();
        break;
    case NDR_Menu:
        NDR_Setting_Window();
        break;
#if VERSION_B		
    case UPWR_Menu:
        UPWR_Setting_Window();
        break;
    case OPWR_Menu:
        OPWR_Setting_Window();
        break;
#endif		

    case OF_Menu: //14May14 UF OF
        OF_Setting_Window();
        break;
    case UF_Menu:
        UF_Setting_Window();
        break;

    case AR_Menu:
        AR_Setting_Window();
        break;
    
    

    default: break;
    }
    if (SET_SKIP!=0)
        Show_Setting_Skip();

    if (KEYBRD_FLAG.BYTE.PWD_Fail)
        Show_PWD_Fail();


    SCR_FLAG.Update_DI = 0;
    update_screen = 0;

}
//***********************************************************************************


	

void Show_Trip_Close_DI_Val1(struct Trip_Close_DI *sPtr)
{
	unsigned char i,Cnt,temp;

	Cnt = SubMenu_Scrl_Cnt;

	for(i=0; i<3; i++)
	{
		switch(Cnt)
		{
		case 0:
			if(sPtr->TEST_DI >= TOTAL_DI)
    			temp = MAX_TRP_CLS_DI; //None condition
    		else
    			temp = sPtr->TEST_DI;
    	
    		GLCD_Write_Line(Line2+(i*GLCD_TEXT_AREA)+14, 5, Trip_Close_BI[temp]);
		break;

		case 1:
			if(sPtr->SERVICE_DI >= TOTAL_DI)
    			temp = MAX_TRP_CLS_DI; //None condition
    		else
    			temp = sPtr->SERVICE_DI;
    
    		GLCD_Write_Line(Line2+(i*GLCD_TEXT_AREA)+14, 5, Trip_Close_BI[temp]);
		break;
		
		case 2:	
			if(sPtr->RMT_TRIP_DI >= TOTAL_DI)
    			temp = MAX_TRP_CLS_DI; //None condition
    		else
    			temp = sPtr->RMT_TRIP_DI;
    
    		GLCD_Write_Line(Line2+(i*GLCD_TEXT_AREA)+14, 5, Trip_Close_BI[temp]);
		break;
		}
		Cnt++;
	}
		
	if(KEYBRD_FLAG.BYTE.EditMode)
		GLCD_SetCursorPointer(Line2+(SubMenu_Ver_Scrl*GLCD_TEXT_AREA)+19, 5, BLINK_ON);
///

	

void Show_DR_DI(void)
{
    uBYTE i, pos, CNT;
    uWORD DR_STS/*,TMP*/, Sft = 0x0001;

    pos = DR_DI_X_OFF + DR_DI_Y_OFF;

    if (TOTAL_DI > DR_DI_ROW) CNT = DR_DI_ROW;
    else CNT = TOTAL_DI;

    if (update_screen)
    {
        if (KEYBRD_FLAG.BYTE.ClrScr)
        {
            GLCD_clrscr_TEXT();
            KEYBRD_FLAG.BYTE.ClrScr = 0;

            GLCD_Write_Line(pos + 3, 17, DI_DR_Tital);
            GLCD_Fill_Line(pos + GLCD_TEXT_AREA - 3, 26, '-');
        }

        for (i = 0; i < CNT; i++)
        {
            GLCD_Write_Line(pos + ((i + 2) * GLCD_TEXT_AREA) + 2, 6, Ext_DI_Ann[Ver_Scrl_Cnt + i]);
        }


        DR_STS = DR_ON_DI.DI_WORD;

        for (i = 0; i < CNT; i++)
        {
            if (DR_STS & Sft)
                GLCD_Write_Line(pos + ((i + 2) * GLCD_TEXT_AREA) + 12, 4, ON_AR);
            else
                GLCD_Write_Line(pos + ((i + 2) * GLCD_TEXT_AREA) + 12, 4, OFF_AR);
            Sft = Sft << 1;
        }

        Put_Ext_Char(pos + ((Ver_Wnd_Scrl + 2) * GLCD_TEXT_AREA) + 12, RT_ARROW);

        update_screen = 0;
    }

    if (KEYBRD_FLAG.BYTE.EditMode)
        GLCD_SetCursorPointer(Get_CursorPntr(), 4, 1);

}
//***********************************************************************************
//***********************************************************************************		
/*void Show_DR_DI_Sts(void)
{
        uBYTE i,CNT;
        uWORD DR_STS,TMP,Sft=0x0001;
	
        if(TOTAL_DI>DR_TRG_ROW)	CNT = DR_TRG_ROW;
                else	CNT = TOTAL_DI;
		
        DR_STS = DR_ON_DI.DI_WORD;
	
        for(i=0; i<CNT; i++)
        {
                if(DR_STS & Sft)
                        GLCD_Write_Line(DR_SubMenu_X_OFF+(i*GLCD_TEXT_AREA)+17, 4, ON_AR);
                else
                        GLCD_Write_Line(DR_SubMenu_X_OFF+(i*GLCD_TEXT_AREA)+17, 4, OFF_AR);
                Sft = Sft<<1;
        }
	
        Put_Ext_Char(DR_SubMenu_X_OFF+(Ver_Wnd_Scrl*GLCD_TEXT_AREA)+17, RT_ARROW);
}*/
//***********************************************************************************
/
