#include "wrapper_linkit_lcd.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

void LinkItLCD::init() {
	lcd.init();
	byte battery_empty[9] = {
		B01110,
		B10001,
		B10001,
		B10001,
		B10001,
		B10001,
		B10001,
		B11111
	};
	byte battery_half[9] = {
		B01110,
		B11011,
		B10001,
		B10001,
		B11111,
		B11111,
		B11111,
		B11111
	};
	byte battery_full[9] = {
		B01110,
		B11111,
		B11111,
		B11111,
		B11111,
		B11111,
		B11111,
		B11111
	};
	byte time_am[9] = {
		B01110,
		B11011,
		B11111,
		B11011,
		B00000,
		B11011,
		B11111,
		B10101
	};
	byte time_pm[9] = {
		B11110,
		B11011,
		B11110,
		B11000,
		B00000,
		B11011,
		B11111,
		B10101
	};
	byte sat[9] = {
		B00100,
		B00000,
		B10101,
		B11111,
		B10101,
		B00100,
		B01010,
		B11011
	};
	byte charging[9] = {
		B00001,
		B00010,
		B00100,
		B01111,
		B10110,
		B00100,
		B01000,
		B10000
	};
	byte blank[9] = {
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000
	};
	lcd.home();
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.backlight();
	for (int i = 0; i < 18; i++) lcd.createChar(i, blank);
	lcd.createChar(ICON_BATTERY_EMPTY,	battery_empty);
	lcd.createChar(ICON_BATTERY_HALF,	battery_half);
	lcd.createChar(ICON_BATTERY_FULL,	battery_full);
	lcd.createChar(ICON_AM,				time_am);
	lcd.createChar(ICON_PM,				time_pm);
	lcd.createChar(ICON_SAT,			sat);
	lcd.createChar(ICON_CHARGING,		charging);
}

void LinkItLCD::clear() {
	lcd.clear();
}

void LinkItLCD::print(byte x, byte y, String text) {
	lcd.setCursor(x, y);
	lcd.print(text);
}

void LinkItLCD::icon(byte x, byte y, byte id) {
	lcd.setCursor(x, y);
	lcd.write(id);
}

void LinkItLCD::charset() {
	lcd.clear();
	lcd.setCursor(0, 0);
	for (int i = 0; i < 20; i++) {
		lcd.write(235 + i);
		lcd.print(235 + i);
	}
}

