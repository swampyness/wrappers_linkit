#ifndef _WRAPPER_LINKIT_LCD_H
#define _WRAPPER_LINKIT_LCD_H
#include <Arduino.h>
class LinkItLCD {
	public:
		#define ICON_BATTERY_EMPTY		0
		#define ICON_BATTERY_HALF		1
		#define ICON_BATTERY_FULL		2
		#define ICON_AM					3
		#define ICON_PM					4
		#define ICON_SAT				5
		#define ICON_CHARGING			6
		#define CHAR_SQUARE_SUPER		223
		#define CHAR_STAR_SUPER			235
		#define CHAR_SQUARE_SUB			161
		#define CHAR_DOT				165
		#define CHAR_CORNER_TOP_LEFT	162
		#define CHAR_CORNER_BOTOM_RIGHT	163
		#define CHAR_SQUARE_SUB_LARGE	219
		byte I2C_Address;
		byte width;
		byte height;
		int timer;
		void init();
		void clear();
		void print(byte x, byte y, String text);
		void icon(byte x, byte y, byte id);
		void charset();			
	};
#endif

