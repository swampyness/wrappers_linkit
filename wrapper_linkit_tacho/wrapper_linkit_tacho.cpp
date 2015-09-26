#include "wrapper_linkit_tacho.h"

void LinkItTacho::init() {
	pin = 0;
	count_max = 100;
	radius = 13.5;
	kph_ratio = 1.609;
	delta = 180.0 / E_PI;
	pinMode(pin, INPUT_PULLUP);
	///?///attachInterrupt(pin, update, CHANGE);
	count_cur = count_max;
	circumference = 2 * 3.14 * radius;
}

void LinkItTacho::update() {
	switch_cur = digitalRead(pin);
	if (switch_cur == 0) {
		if (count_cur == 0) {
			mph = (56.8 * float(circumference)) / float(timer_cur);
			kph = mph * kph_ratio;
			if (kph > kph_max) kph_max = kph;
			timer_cur = 0;
			count_cur = count_max;
		} else if (count_cur > 0) count_cur -= 1;
	} else if (count_cur > 0) count_cur -= 1;
	if (timer_cur > 2000) {
		mph = 0.0;
		kph = 0.0;
		kph_max = 0.0;
	} else timer_cur++;
}

