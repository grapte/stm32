#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include "GLCD_Config.h"                // Keil.STM32F746G-Discovery::Board Support:Graphic LCD

#include <stdlib.h>

extern GLCD_FONT GLCD_Font_16x24; 

static void SystemClock_Config(void);

void wait(unsigned int t) {
	t *= 4000000;
	for(volatile unsigned int i; i < t; i++) {}		
}

int sign(x) {
if(x>0)
 return 1;
else if(x<0)
 return -1;
else
 return 0;
}


/*void Bres_line(int x1,int y1, int x2, int y2)
{
int x,y,dx,dy,swap,temp,s1,s2,p,i;

x=x1;
y=y1;
dx=abs(x2-x1);
dy=abs(y2-y1);
s1=sign(x2-x1);
s2=sign(y2-y1);
swap=0;
GLCD_DrawPixel(x1,y1);
if(dy>dx)
 {
 temp=dx;
 dx=dy;
 dy=temp;
 swap=1;
 }
p=2*dy-dx;
for(i=0;i<dx;i++)
 {
 GLCD_DrawPixel(x,y);
 while(p>=0)
  {
  p=p-2*dx;
  if(swap)
   x+=s1;
  else
   y+=s2;
  }
 p=p+2*dy;
 if(swap)
  y+=s2;
 else
  x+=s1;
 }
GLCD_DrawPixel(x2,y2);
}*/

/*void Bres_line(int x0, int y0, int x1, int y1)
{
  int dx, dy, p, x, y;
	
	dx=x1-x0;
	dy=y1-y0;
 
	x=x0;
	y=y0;
 
	p=2*dy-dx;
 
	while (x<x1) {
		wait(1);
		if(p>=0) {
			GLCD_DrawPixel(x,y);
			y=y+1;
			p=p+2*dy-2*dx;
		} else {
			GLCD_DrawPixel(x,y);
			p=p+2*dy;
		}
		x=x+1;
	}
	
}*/

/*void Bres_line(int x1, int y1, int x2, int y2)
{
  int m_new = 2 * (y2 - y1);
  int slope_error_new = m_new - (x2 - x1);
  for (int x = x1, y = y1; x <= x2; x++) {
    GLCD_DrawPixel(x, y);
 
    // Add slope to increment angle formed
    slope_error_new += m_new;
 
    // Slope error reached limit, time to
    // increment y and update slope error.
    if (slope_error_new >= 0) {
       y++;
       slope_error_new  -= 2 * (x2 - x1);
    }
  }
}*/

void Bres_line(int x0, int y0, int x1, int y1) {
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    GLCD_DrawPixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void drawCircle(int xc, int yc, int x, int y) {
  GLCD_DrawPixel(xc+x, yc+y); 
  GLCD_DrawPixel(xc-x, yc+y); 
  GLCD_DrawPixel(xc+x, yc-y); 
  GLCD_DrawPixel(xc-x, yc-y); 
  GLCD_DrawPixel(xc+y, yc+x); 
  GLCD_DrawPixel(xc-y, yc+x); 
  GLCD_DrawPixel(xc+y, yc-x); 
  GLCD_DrawPixel(xc-y, yc-x);
	Bres_line(xc+x, yc+y, xc+x, yc-y);
	Bres_line(xc-x, yc+y, xc-x, yc-y);
	Bres_line(xc+y, yc+x, xc+y, yc-x);
	Bres_line(xc-y, yc+x, xc-y, yc-x);
}
  
// Function for circle-generation 
// using Bresenham's algorithm 
void Bres_circle(int xc, int yc, int r) 
{ 
  int x = 0, y = r; 
  int d = 3 - 2 * r; 
  while (y >= x) 
  { 
    // for each pixel we will 
    // draw all eight pixels 
    drawCircle(xc, yc, x, y); 
    x++; 
 
    // check for decision parameter 
    // and correspondingly  
    // update d, x, y 
    if (d > 0) 
    { 
        y--;  
        d = d + 4 * (x - y) + 10; 
    } 
    else
        d = d + 4 * x + 6; 
    drawCircle(xc, yc, x, y); 
    //wait(1); 
  } 
} 

int main(void)
{
	SystemClock_Config();
	
	GLCD_Initialize();
	
	GLCD_SetBackgroundColor(GLCD_COLOR_NAVY);
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	
	GLCD_ClearScreen();
	
	GLCD_DrawRectangle(100, 100, 100, 100);
	GLCD_DrawPixel(101, 101);

	/*
	for (int x = 0; x < 480; x++){
		for (int y = 0; y < 272; y++){
			if (rand() % 2) {
				GLCD_DrawPixel(x, y);
			}
		}
	}
	*/
	
	GLCD_SetFont(&GLCD_Font_16x24);
	
	GLCD_DrawString(50, 50, (char *) main);
	
	// up right
	Bres_line(25, 25, 50, 0);
	// up left
	Bres_line(25, 25, 0, 0);
	// down right
	Bres_line(25, 25, 0, 50);
	// down left
	Bres_line(25, 25, 50, 50);
	
	Bres_line(25, 25, 450, 50);
	
	//wait(6);
	
	Bres_circle(150, 150, 50);
	
	Touch_Initialize();
	
	TOUCH_STATE tstate;
	// previous x y
	int x=20,y=20;
	
	for(;;) {
		Touch_GetState(& tstate);
		if (tstate.pressed) {
			GLCD_SetForegroundColor(GLCD_COLOR_NAVY);
			Bres_circle(x, y, 20);
			GLCD_SetForegroundColor(GLCD_COLOR_RED);
			
			x = tstate.x, y = tstate.y;
			Bres_circle(x, y, 20);
		}
	}
	
	
	return 0;
}

void SysTick_Handler (void)
{
    HAL_IncTick();
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}
