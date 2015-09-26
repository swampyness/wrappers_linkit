#ifndef _JOE_LINKIT_TACHO_H
#define _JOE_LINKIT_TACHO_H
#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#include <Arduino.h>
	class LinkItTacho {
		public:
			int pin;
			int count_max;
			float radius;
			float kph_ratio;
			float delta;
			int count_cur;
			float circumference;
			int timer_cur;
			int switch_cur;
			float mph;
			float kph;
			float kph_max;
			float angle_wheel;
			void init();
			void update();
	};
#endif

