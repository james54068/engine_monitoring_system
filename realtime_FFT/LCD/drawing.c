#include "drawing.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

void DrawThickCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness){

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawFullCircle(x, y, radius);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);
    LCD_DrawFullCircle(x, y, radius-thickness);
    //LCD_DrawUniLine(x,y,x+radius,y+radius);

}

void DrawThickSemiCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness){

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawFullCircle(x, y, radius);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);
    LCD_DrawFullCircle(x, y, radius-thickness);
    LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
    LCD_DrawFullRect(x-radius,y,2*radius+1,radius+1);
    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
    LCD_DrawFullRect(x-radius,y,2*radius+1,thickness);

    //LCD_DrawUniLine(x,y,x+radius,y+radius);

}

void Draw_CircleNeedle(uint16_t x,uint16_t y,uint16_t radius,float max,float min,float variable)
{
  float midpoint;
  float range;
  double angle;
  int32_t point2x;
  int32_t point2y;
  midpoint = (max + min)/2;
  range    = max - min;

  if (variable > midpoint)
  {
  	angle = (variable - midpoint)/(range/2)*180;
  }
  else if (variable < midpoint)
  {
  	angle = -(midpoint - variable)/(range/2)*180;
  }
  else if (variable > max && variable < min )
  {
  	angle = 180;
  }
  else
  {
  	angle = 0;
  }

  point2x = x + radius*sin(angle*3.14/180);
  point2y = y - radius*cos(angle*3.14/180);



  //LCD_DrawUniLine(x-1,y-1,point2x-1,point2y-1);
  LCD_DrawUniLine(x,y,point2x,point2y);
  //LCD_DrawUniLine(x+1,y+1,point2x+1,point2y+1);

}

void Draw_SemiCircleNeedle(uint16_t x,uint16_t y,uint16_t radius,float max,float min,float variable)
{
  float midpoint;
  float range;
  double angle;
  int32_t point2x;
  int32_t point2y;
  midpoint = (max + min)/2;
  range    = max - min;

  if (variable > midpoint)
  {
    angle = (variable - midpoint)/(range/2)*90;
  }
  else if (variable < midpoint)
  {
    angle = -(midpoint - variable)/(range/2)*90;
  }
  else if (variable > max )
  {
    angle = 90;
  }
  else if (variable < min )
  {
    angle = -90;
  }
  else
  {
    angle = 0;
  }

  point2x = x + radius*sin(angle*3.14/180);
  point2y = y - radius*cos(angle*3.14/180);



  //LCD_DrawUniLine(x-1,y-1,point2x-1,point2y-1);
  LCD_DrawUniLine(x,y,point2x,point2y);
  //LCD_DrawUniLine(x+1,y+1,point2x+1,point2y+1);

}


void Circle_Meter(uint16_t x,uint16_t y,uint16_t radius,float max,float min)
{
	char num[100] ;
	// float midpoint = (max + min)/2;
  	float range = max - min;
  	double angle = 0;
  	int32_t point2x;
  	int32_t point2y;
  	uint8_t buff_len;

  	float i;
  	// uint8_t k = 0;
  	uint8_t j = 0;

  	for (i = min + range/10 ; i < max ; i += range/10)
  	{
  		
  		angle += 360/10;
  	 	point2x = x - radius*sin(angle*3.14/180);
  		point2y = y + radius*cos(angle*3.14/180);
  		// LCD_DisplayChar(point2y,point2x,num[j]);
  		// j++;

  		sprintf(num,"%f",(double)i);
  		buff_len = strlen(num);

  		for(j = 0;j < buff_len; j++)
  		{
  			if((buff_len % 2)==0)
  			{
  				LCD_DisplayChar(point2y,point2x-9,num[0]);
  				LCD_DisplayChar(point2y,point2x-3,num[1]);
  				LCD_DisplayChar(point2y,point2x+3,num[2]);
  				LCD_DisplayChar(point2y,point2x+9,num[3]);
  			}
  			else if((buff_len % 2)==1) 
  			{
  				LCD_DisplayChar(point2y,point2x-12,num[0]);
  				LCD_DisplayChar(point2y,point2x-6,num[1]);
  				LCD_DisplayChar(point2y,point2x,num[2]);
  				LCD_DisplayChar(point2y,point2x+6,num[3]);
  				LCD_DisplayChar(point2y,point2x+12,num[4]);
  			}

  		}

  	}

  	DrawThickCircle(x,y,radius-15,5);		

}


void Semi_Circle_Meter(uint16_t x,uint16_t y,uint16_t radius,float max,float min)
{
  char num[100] ;
  float midpoint = (max + min)/2;
    float range = max - min;
    double angle = 0;
    int32_t point2x;
    int32_t point2y;
    uint8_t buff_len;

    float i;
    uint8_t j = 0;


    for (i = min ; i <= max ; i += range/10)
    {
      
      
      point2x = x - (float)(radius*cos((-(angle)*3.14/180)));
      point2y = y + (float)(radius*sin((-(angle)*3.14/180)));
      if(i == midpoint)
        point2y -= 10;
 
      // LCD_DisplayChar(point2y,point2x,num[j]);
      // j++;
      angle += 180/10;

      sprintf(num,"%f",(double)i);
      buff_len = strlen(num);

      for(j = 0;j < buff_len; j++)
      {
        if((buff_len % 2)==0)
        {
          LCD_DisplayChar(point2y,point2x-9,num[0]);
          LCD_DisplayChar(point2y,point2x-3,num[1]);
          LCD_DisplayChar(point2y,point2x+3,num[2]);
          LCD_DisplayChar(point2y,point2x+9,num[3]);
        }
        else if((buff_len % 2)==1) 
        {
          LCD_DisplayChar(point2y,point2x-12,num[0]);
          LCD_DisplayChar(point2y,point2x-6,num[1]);
          LCD_DisplayChar(point2y,point2x,num[2]);
          LCD_DisplayChar(point2y,point2x+6,num[3]);
          LCD_DisplayChar(point2y,point2x+12,num[4]);
        }

      }

    }

    DrawThickSemiCircle(x,y,radius-15,5);   

}

void XY_axis(uint16_t x,uint16_t y,float x_max,float x_min,float y_max,float y_min)
{
  float x_range = x_max - x_min;
  float y_range = y_max - y_min;
  uint8_t num[100]="0";
  LCD_DrawLine(x,y-100,100,LCD_DIR_VERTICAL);
  LCD_DrawLine(x,y,200,LCD_DIR_HORIZONTAL);
  LCD_DisplayChar(y+5,x-10,num[0]);






}