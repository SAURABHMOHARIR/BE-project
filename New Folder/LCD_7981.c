/***********************************************************************/
/*                                                                     */
/*  FILE        :LCD_7981.c                   	                       */
/*  DESCRIPTION :Raystar LCD interfacing routines                      */
/*  LCD CONTROLLER : LC7981                                            */
/*  CPU GROUP   :dsPIC33F                                              */
/*                                                                     */
/* 																	   */
/***********************************************************************/

#include <p33FJ256GP710.h> 
#include "define.h"
#include "LCD_7981.h"
#include "math.h"
#include "test.h"
#include "discon.h"

//**************************************************************************************
//**************************************************************************************
// Sets address pointer for display RAM memory
//**************************************************************************************
void GLCD_SetAddressPointer(uWORD Location)
{
	if(Location>=80)
		return;
	switch(Location/20)
	{
		case 0:
			lcdCommand(0x80 + Location);
			break;
		
		case 1:
			lcdCommand(0xC0 + Location - 20);
			break;
			
		case 2:
			lcdCommand(0x94 + Location - 40);
			break;
		
		case 3:
			lcdCommand(0xD4 + Location - 60);
			break;
	}		
}	
//**************************************************************************************
// Function : cbg_onscr
// Description : On all pixels of lcd screen
// Notice: Can be used only in graphic mode
//**************************************************************************************
/*void GLCD_ONscr(void)
{
	uBYTE x,y;
	SetGraphicMode();
	GLCD_SetAddressPointer(0x0000);
 
 	for (y=0; y<LCD_HEIGHT; y++)
	{
		for (x=0; x<GLCD_CHAR_PER_LINE; x++)
		{
			GLCD_WriteDisplayData(0xFF);
		}
	}
}*/
//**************************************************************************************
//**************************************************************************************
// Sets Cursor pointer
// Set Location, Pattern, and Blink properties
// Location -> location address,(MSB byte - Y coordinate, LSB byte - X coordinate) 
// Pattern -> line at which cursor to be display. (pass 1 for 1 line, 8 for 8 line corsor)
// Blink -> 1 - blink ON, 0 - blink OFF
//**************************************************************************************
void GLCD_SetCursorPointer(uWORD Location,uBYTE pattern,uBYTE blink)
{
	GLCD_SetAddressPointer(Location);
	if(blink)
		lcdCommand(0x0F);
	else
		lcdCommand(0x0E);
}	
//**************************************************************************************	
void GLCD_CursorOFF(void)
{
	lcdCommand(0x0C);
}
//**************************************************************************************	
//**************************************************************************************	
//go to specified location
//x -> byte size (8 pixel)
//y -> bit size (1 pixel)
//**************************************************************************************	
void GLCD_gotoxy_GRAPHICS(uBYTE x, uBYTE y)
{
//	int location, home; 
//	int line;
//	home = 0x0000;
//	line = GLCD_PIXEL_WIDTH/8; 
//
//	location = home + (((int)y - 1) * line) + x - 1;
//	// Setting address area for graphic mode
//	GLCD_SetAddressPointer(location);
}
//**************************************************************************************	
//**************************************************************************************	
// Sets display coordinates for Text
// Left upper corner is x-1,y-1
//**************************************************************************************	
void GLCD_TextGoTo(uBYTE x, uBYTE y)
{
	uWORD address;
	address = (x-1)*20+(y-1); //GLCD_TEXT_HOME +  (x-1) + (GLCD_TEXT_AREA * (y-1));
	GLCD_SetAddressPointer(address);
}
//**************************************************************************************	
//**************************************************************************************	
// Write character string to line
//**************************************************************************************	
void GLCD_Write_Line(uWORD line, uBYTE count,const uBYTE *Pntr)
{
	GLCD_SetAddressPointer(line);
  
 	while(count)
  	{
		GLCD_WriteChar(*Pntr++);
		count--;
  	}
}
//**************************************************************************************	
// Write character string to line
//**************************************************************************************	
/*void GLCD_Write_Line_ExtCG(uWORD line, uBYTE count, uBYTE *Pntr)
{
  GLCD_SetAddressPointer(line);
  
  while(count)
  	{
		GLCD_WriteChar_ExtCG(*Pntr++);
		count--;
  	}
}*/
//**************************************************************************************	
// Write character string to line
//**************************************************************************************	
void GLCD_Write_Line_ExtCG(uWORD line, uBYTE count, uBYTE *Pntr)
{
  GLCD_SetAddressPointer(line);
  
  while(count)
  	{
		GLCD_WriteChar_ExtCG(*Pntr++);
		count--;
  	}
}
//**************************************************************************************	
// Write graphics with specified width/height at location given by x,y
//**************************************************************************************	
void Gph_Plot_Symbol(uBYTE x, uBYTE y, uBYTE WDT, uBYTE HGT,const uBYTE *Pntr)
{
//	uBYTE i,j;
//	
//	for (i = 0; i < WDT; i++)
//	{
//    	for (j = 0; j < HGT; j++)
//		{
//        	if (*Pntr & (1<<j))
//            	lcd_graphics_plot_pixel(x, y+j, PIXEL_ON);
//         	else   
//            	lcd_graphics_plot_pixel(x, y+j, PIXEL_OFF);
//		}
//   	 	Pntr++; 
//      	x++;
//	}
}
//**************************************************************************************	
// Write repeated character to a line
//**************************************************************************************	
void GLCD_Fill_Line(uWORD Line, uBYTE Cnt, uBYTE Character)
{
  GLCD_SetAddressPointer(Line);
  
  while(Cnt)
  	{
		GLCD_WriteChar(Character);
		Cnt--;
  	}
}
//**************************************************************************************	
/
