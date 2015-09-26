#ifndef _WRAPPER_LINKIT_GPS_H
#define _WRAPPER_LINKIT_GPS_H
#include <LGPS.h>
#define S_ALL					0
#define S_LONGITUDE				1
#define S_LATITUDE				2
#define S_TIME					3
#define S_DATE					4
#define S_SAT					5
#define S_FIX					6
#define S_POSITION				7
#define S_TIME_LAST				8
#define S_TIME_ALT				9
#define S_SAT_LAST				10
#define GPGGA_TIME				1
#define GPGGA_LATITUDE			2
#define GPGGA_LATITUDE_DIR		3
#define GPGGA_LONGITUDE			4
#define GPGGA_LONGITUDE_DIR 	5
#define GPGGA_FIX				6
#define GPGGA_SAT				7
class LinkItGPS {
		public:
			gpsSentenceInfoStruct info;
			int hour;
			int minute;
			int second;
			int second_last;
			int pm;
			int fix;
			char view[10][255];
			unsigned char getComma(unsigned char num, const char *str);
			double getDoubleNumber(const char *s);
			int getIntNumber(const char *s);
			void init();
			void update();
			void pos_degrees(double pos, int dir_type);
			void split_into_dms(float angle, unsigned int * deg, unsigned int* min, float *sec);
			void parseGPGGA(const char* GPGGAstr);
};
#endif

