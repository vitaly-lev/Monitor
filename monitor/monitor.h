/*
	<C> <Lion>

	monitor.h

	Programm for Debug Monitor

Input:
				UART1 - 115200 8n1
				UART2 - 115200 8n1
Oupput:
				Display 128x160

	2019 01 11	v1.0 


*/


#include "stm32f1xx_hal_def.h"



#define font_h 10									// height of font
#define font_w 7									// weight of fonr
#define num_of_line 6							// 64/(10+1)=5,8     1 - one pixel between line
																	//									64 - half display
																	//									10 - height of char
																	
#define num_char_in_line 22				// 160/7=22		 			160 - number of chars in line
																	//									 7 - width of char
																	//
#define uart1_x_char_start 0			// start char position of char for uart1 (in char) 0 = first position
#define uart2_x_char_start 0			// start char position of char for uart2




void monitor(UART_HandleTypeDef *huart);

