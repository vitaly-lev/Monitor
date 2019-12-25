/*
	<C> <Lion>

	monitor.c

	Programm for Debug Monitor 

Input:
				UART1 - 115200 8n1
				UART2 - 115200 8n1
Oupput:
				Display 128x160

	2019 01 11	v1.0 


*/

#include "stm32f1xx_hal.h"
#include <string.h>
#include "monitor.h"
#include "..\st7735\st7735.h"
#include "..\st7735\fonts.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern char rxbyte1;
extern char rxbyte2;

char txstring1[2] = "";
char txstring2[2] = "";


//2 places: uart1, uart2
//Display 128x160
//font 7*10 



uint8_t uart1_x_start = 0;				// start pixel position of char for uart1 (in pixel)
uint8_t uart2_x_start = 0;				// start pixel position of char for uart2 (i````	`																																			n pixel)
uint8_t uart1_y_start = 118;
uint8_t uart2_y_start = 53;

uint8_t uart1_x = uart1_x_char_start;									// current position of char for uart1
uint8_t uart1_y;
uint8_t uart2_x = uart2_x_char_start;									// current position of char for uart2
uint8_t uart2_y;
uint8_t uart1_nc=0;									//Number of receive chars

typedef struct
{
	
	char line[num_char_in_line+2];
	uint8_t num_char;
	
}display;

display d1[num_of_line];									// Display memory for uart1
display d2[num_of_line];								// Display memory for uart2


char n_string[num_char_in_line+1];



void monitor(UART_HandleTypeDef *huart)
{
int8_t i=0,j=0,k=0;

	if (huart->Instance == USART1)
	{
	HAL_UART_Receive_IT(&huart1,(uint8_t *) &rxbyte1, 1);
	txstring1[0] = rxbyte1;
		
			
		if (rxbyte1 != 0x0A  && rxbyte1 != 0x0D && d1[0].num_char < num_char_in_line)
		{
			d1[0].line[uart1_x] = rxbyte1;				//write symbol to display memory
			d1[0].num_char++;										//write number of char in line to display memory
			
			ST7735_WriteString(uart1_x*(font_w), uart1_y_start, txstring1, Font_7x10, ST7735_GREEN, ST7735_BLACK);
			uart1_x++;
			d1[0].line[uart1_x] = 0;							//write symbol "end of line" to display memory
			return;
		}
		else
			if (d1[0].num_char < num_char_in_line)
				d1[0].line[d1[0].num_char] = 0;							//if CR or LF write symbol "end of line" to display memory
		
		

		if (d1[0].num_char >= num_char_in_line || rxbyte1 == 0x0A || rxbyte1 == 0x0D)
		{
			
			
			for (i=num_of_line-2; i > 0; i--)
			{
				for (j=0; j<=d1[i].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d1[i+1].line[j] = d1[i].line[j];	// переписуєм попередню лінійку у лінійку вище
				}
				
				
				if (d1[i+1].num_char > d1[i].num_char)
				{
					for (k = d1[i].num_char; k<=d1[i+1].num_char; k++)
					{
						d1[i].line[k] = ' ';
					}
					d1[i].line[k] = 0;
			
					ST7735_WriteString(uart1_x_start, uart1_y_start-font_h*(i+1), d1[i].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				}
				
				d1[i+1].num_char = d1[i].num_char;
				d1[i+1].line[j] = 0;
				
				ST7735_WriteString(uart1_x_start, uart1_y_start-font_h*(i+1), d1[i+1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				
			}
	
/*	
int z;			
				for (z=0; z<6; z++)
			
				{
					sprintf(debug_string, "d1[%i].num_char=%i", z, d1[z].num_char);
					ST7735_WriteString(uart1_x_start, z*10, debug_string, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				}

*/
			
			
			// переписуєм 0 лінійку у 1 лінійку
			// якщо кількість символів у 0 лінійці більша або рівна кількості символів у 1 лінійці
			// переписати 0 лінійку у першу
			
			if (d1[1].num_char <= d1[0].num_char)
			{
				for (j=0; j<=d1[0].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d1[1].line[j] = d1[0].line[j];	// переписуєм попередню лінійку у лінійку вище
				}
				d1[1].num_char = d1[0].num_char;
				d1[1].line[j] = 0;
				/*d1[0].num_char = 0;*/
				
				ST7735_WriteString(uart1_x_start, uart1_y_start-font_h, d1[1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);

			}
			else
			{
				for (j=0; j<=d1[0].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d1[1].line[j] = d1[0].line[j];	// переписуєм попередню лінійку у лінійку вище
				}

				for (k = d1[0].num_char; k<=d1[1].num_char; k++)
				{
					d1[1].line[k] = ' ';
				}
				d1[1].line[k] = 0;
				ST7735_WriteString(uart1_x_start, uart1_y_start-font_h, d1[1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				d1[1].num_char = d1[0].num_char;
			}
			
			ST7735_WriteString(uart1_x_start, uart1_y_start, n_string, Font_7x10, ST7735_GREEN, ST7735_BLACK);
			
			if (d1[0].num_char < num_char_in_line)
			{
				for (i=0; i<num_char_in_line; i++)
					d1[0].line[i] = 0;
				
				uart1_x = uart1_x_char_start;															//x start position of line
				d1[0].num_char = 0;
			}
			else
			{
				if (rxbyte1 != 0x0A && rxbyte1 != 0x0D)
				{
					d1[0].line[0] = rxbyte1;
					d1[0].line[1] = 0;
					ST7735_WriteString(uart1_x_start, uart1_y_start, d1[0].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
					d1[0].num_char = 1;
					uart1_x = uart1_x_char_start + 1;
				}
				else
				{
					d1[0].line[0] = 0;
					d1[0].num_char = 0;
					ST7735_WriteString(uart1_x_start, uart1_y_start, d1[0].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
					uart1_x = uart1_x_char_start;
				}
			}
		}
	}
	else 
		
	
	if (huart->Instance == USART2)
	{
	HAL_UART_Receive_IT(&huart2,(uint8_t *) &rxbyte2, 1);
	txstring2[0] = rxbyte2;
		
		if (rxbyte2 != 0x0A  && rxbyte2 != 0x0D && d2[0].num_char < num_char_in_line)
		{
			d2[0].line[uart2_x] = rxbyte2;				//write symbol to display memory
			d2[0].num_char++;										//write number of char in line to display memory
			
			ST7735_WriteString(uart2_x*(font_w), uart2_y_start, txstring2, Font_7x10, ST7735_GREEN, ST7735_BLACK);
			uart2_x++;
			d2[0].line[uart2_x] = 0;							//write symbol "end of line" to display memory
			return;
		}
		else
			if (d2[0].num_char < num_char_in_line)
				d2[0].line[d2[0].num_char] = 0;							//if CR or LF write symbol "end of line" to display memory
		
		

		if (d2[0].num_char >= num_char_in_line || rxbyte2 == 0x0A || rxbyte2 == 0x0D)
		{
			
			
			for (i=num_of_line-2; i > 0; i--)
			{
				for (j=0; j<=d2[i].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d2[i+1].line[j] = d2[i].line[j];	// переписуєм попередню лінійку у лінійку вище
				}
				
				
				if (d2[i+1].num_char > d2[i].num_char)
				{
					for (k = d2[i].num_char; k<=d2[i+1].num_char; k++)
					{
						d2[i].line[k] = ' ';
					}
					d2[i].line[k] = 0;
			
					ST7735_WriteString(uart2_x_start, uart2_y_start-font_h*(i+1), d2[i].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				}
				
				d2[i+1].num_char = d2[i].num_char;
				d2[i+1].line[j] = 0;
				
				ST7735_WriteString(uart2_x_start, uart2_y_start-font_h*(i+1), d2[i+1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				
			}
			
/*	
int z;			
				for (z=0; z<6; z++)
			
				{
					sprintf(debug_string, "d2[%i].num_char=%i", z, d2[z].num_char);
					ST7735_WriteString(uart1_x_start, z*10, debug_string, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				}

*/
			
			
			// переписуєм 0 лінійку у 1 лінійку
			// якщо кількість символів у 0 лінійці більша або рівна кількості символів у 1 лінійці
			// переписати 0 лінійку у першу
			
			if (d2[1].num_char <= d2[0].num_char)
			{
				for (j=0; j<=d2[0].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d2[1].line[j] = d2[0].line[j];	// переписуєм попередню лінійку у лінійку вище
				}
				d2[1].num_char = d2[0].num_char;
				d2[1].line[j] = 0;
				/*d2[0].num_char = 0;*/
				
				ST7735_WriteString(uart2_x_start, uart2_y_start-font_h, d2[1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);

			}
			else
			{
				for (j=0; j<=d2[0].num_char; j++)	// j - позиція символа попередньої лінійки
				{
					d2[1].line[j] = d2[0].line[j];	// переписуєм попередню лінійку у лінійку вище
				}

				for (k = d2[0].num_char; k<=d2[1].num_char; k++)
				{
					d2[1].line[k] = ' ';
				}
				d2[1].line[k] = 0;
				ST7735_WriteString(uart2_x_start, uart2_y_start-font_h, d2[1].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
				d2[1].num_char = d2[0].num_char;
			}
			
			ST7735_WriteString(uart2_x_start, uart2_y_start, n_string, Font_7x10, ST7735_GREEN, ST7735_BLACK);
			
			if (d2[0].num_char < num_char_in_line)
			{
				for (i=0; i<num_char_in_line; i++)
					d2[0].line[i] = 0;
				
				uart2_x = uart2_x_char_start;															//x start position of line
				d2[0].num_char = 0;
			}
			else
			{
				if (rxbyte2 != 0x0A && rxbyte2 != 0x0D)
				{
					d2[0].line[0] = rxbyte2;
					d2[0].line[1] = 0;
					ST7735_WriteString(uart2_x_start, uart2_y_start, d2[0].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
					d2[0].num_char = 1;
					uart2_x = uart2_x_char_start + 1;
				}
				else
				{
					d2[0].line[0] = 0;
					d2[0].num_char = 0;
					ST7735_WriteString(uart2_x_start, uart2_y_start, d2[0].line, Font_7x10, ST7735_GREEN, ST7735_BLACK);
					uart2_x = uart2_x_char_start;
				}
			}
		}
	}
		
}
