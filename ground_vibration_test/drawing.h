#ifndef __DRAWING_H
#define __DRAWING_H

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include <string.h>

void DrawThickCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness);
void DrawThickSemiCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness);
void Draw_CircleNeedle(uint16_t x,uint16_t y,uint16_t radius,float max,float min,float variable);
void Draw_SemiCircleNeedle(uint16_t x,uint16_t y,uint16_t radius,float max,float min,float variable);
void Circle_Meter(uint16_t x,uint16_t y,uint16_t radius,float max,float min);
void Semi_Circle_Meter(uint16_t x,uint16_t y,uint16_t radius,float max,float min);
void XY_axis(uint16_t x,uint16_t y,float x_max,float x_min,float y_max,float y_min);

#endif