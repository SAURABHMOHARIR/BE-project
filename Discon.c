/***********************************************************************/
/*                                                                     */
/*  FILE        :Discon.c                      	                       */
/*  DESCRIPTION :display configuration file                            */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/
#include "define.h"
#include "LCD_7981.h"
#include "discon.h"
#include "GUI.h"
#include "UART_Comm.h"
#include<p33FJ256GP710.h>

//*******************************************************************************
// - ASCII for left arrow
//~ - ASCII for right arrow
//ò - ASCII for THETA

u
//*******************************************************************************
//*******************************************************************************

//*******************************************************************************
const uBYTE Info_Ann[5][Sub_Menu_Width] = 
									{
//       				              123456789012345678901234								  
									{" CROMPTON  GREAVES  "},
									{"  Transformer Diff. "},
									{"       Relay        "},		
#if VERSION_I									
									{"     VER:TI03       "},
#endif							
#if VERSION_B									
									{"     VER:TX01       "},  
#endif									
									{"  INITIALISING...   "}
									};
									
const uBYTE SRNO_Ann[2][8] = 
							   {
//								 123456789012345678901234
                               {"MODEL :"}, 
							   {"SR.NO.:"}
							   };


const uBYTE menu_display_para[main_menu_para][main_menu_len] = 
								  {
//									  123456789012345678901234
									{"     INFORMATION    "},//0
                                    {"     MEASUREMENT    "},//1
									{"    CONFIGURATION   "},//2   
									{"  BIAS DIFFRENTIAL  "},//3
									{"  THERMAL OVERLOAD  "},//4 
									{"  3I> OVER CURRENT  "},//5 
									{" 3I>> OVER CURRENT  "},//6  
									{"   Io> DERIVED E/F  "},//7 
									{"  Io>> DERIVED E/F  "},//8 
									{"   I2> NEG.SEQ OC   "},//9 
									{"   I2>> NEG.SEQ OC  "},//10
									{"    UNDER CURRENT   "},//11
									{"    SUPERVISIONS    "},//12
									{"    DIST. RECORDS   "},//13
#if (COMM_MENU_DIS==1)
									{"    COMMUNICATION   "},//14
#endif
									{"    FAULT RECORD    "},//15
									{"    EVENT RECORD    "},//16
									{"    BI-BO CONFIG.   "},//16	
									{"     CALIBRATION    "}//17
								 };

const uBYTE menu_display_para_small[main_menu_para][main_menu_len] = 
								  {
//									  123456789012345678901234
									{"     Information    "},//0
                                    {"     Measurement    "},//1
									{"    Configuration   "},//2
									{"  Bias Diffrential  "},//3
									{"  Thermal Overload  "},//4 
									{"  3I> Over Current  "},//5 
									{" 3I>> Over Current  "},//6  
									{"   Io> Derived E/F  "},//7 
									{"  Io>> Derived E/F  "},//8 
									{"   I2> Neg.Seq OC   "},//9 
									{"   I2>> Neg.Seq OC  "},//10
									{"    Under Current   "},//12
									{"    Supervisions    "},//13
									{"    Dist. Record    "},//14
#if (COMM_MENU_DIS==1)
									{"    Communication   "},//15
#endif
									{"    Fault Record    "},//15
									{"    Event Record    "},//16
									{"    Bi-Bo Config.   "},//17
									{"     Calibration    "}//18

								 };


