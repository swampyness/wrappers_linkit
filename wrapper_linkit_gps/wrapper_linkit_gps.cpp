#include <wrapper_linkit_gps.h>
#include <cstring>
#include <stdlib.h>
#include <LGPS.h>

void LinkItGPS::init() {
	LGPS.powerOn();
}

void LinkItGPS::update() {
	LGPS.getData(&info);
	parseGPGGA((const char*)info.GPGGA);
//	parseGPGGA((const char*)info.GPGSA);
//	parseGPGGA((const char*)info.GPRMC);
//	parseGPGGA((const char*)info.GPVTG);
//	parseGPGGA((const char*)info.GPGSV); // x
//	parseGPGGA((const char*)info.GLGSV); // x
//	parseGPGGA((const char*)info.GLGSA); // x
//	parseGPGGA((const char*)info.BDGSV); // x
//	parseGPGGA((const char*)info.BDGSA);
//	sprintf(view[S_ALL], "%c", b);
}

void LinkItGPS::pos_degrees(double pos, int dir_type) {
	double deg = 0;
	double min = 0;
	double sec = 0;
	String dir = "";
	double tmppos = pos;
	if (tmppos < 0) tmppos = pos * -1;
	deg = floor(tmppos);
	min = (tmppos - deg) * 60;
	sec = (min - floor(min)) * 60;
	min = floor(min);
//  sec = round(sec, 2);
	String out = "";
	out += deg;
	out += "°";
	out += min;
	out += """";
	out += sec;
	out += "'";

	switch (dir_type) {
	case 0:
		if (pos < 0) {
			dir = "S";
		}
		else {
			dir = "N";
		}
		break;
	case 1:
		if (pos < 0) {
			dir = "W";
		}
		else {
			dir = "E";
		}
		break;
	}
	out += " ";
	out += dir;
//	sprintf(view[S_LATITUDE], "%d°%d""%d' %c", deg, min, sec, dir);
	sprintf(view[S_LATITUDE], "%d %d %d", deg, min, sec);
}

void LinkItGPS::split_into_dms(float angle, unsigned int * deg, unsigned int* min, float *sec) {
	float t;
	unsigned int d, m;
	float s;
	if (angle < 0.0) angle = -angle;
	d = (unsigned int)angle;
	t = (angle - (float)d)*60.0;
	m = (unsigned int)(t);
	s = (t - (float)m)*60.0;
	if (s >= 59.995) {
		s = 0.0;
		m++;
		if (m == 60) {
			m = 0;
			d++;
		}
	}
	*deg = d;
	*min = m;
	*sec = s;
}

void LinkItGPS::parseGPGGA(const char* GPGGAstr) {
	/* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
	* Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
	* Where:
	*  GGA          Global Positioning System Fix Data
	*  123519       Fix taken at 12:35:19 UTC
	*  4807.038,N   Latitude 48 deg 07.038' N
	*  01131.000,E  Longitude 11 deg 31.000' E
	*  1            Fix quality: 0 = invalid
	*                            1 = GPS fix (SPS)
	*                            2 = DGPS fix
	*                            3 = PPS fix
	*                            4 = Real Time Kinematic
	*                            5 = Float RTK
	*                            6 = estimated (dead reckoning) (2.3 feature)
	*                            7 = Manual input mode
	*                            8 = Simulation mode
	*  08           Number of satellites being tracked
	*  0.9          Horizontal dilution of position
	*  545.4,M      Altitude, Meters, above mean sea level
	*  46.9,M       Height of geoid (mean sea level) above WGS84
	*                   ellipsoid
	*  (empty field) time in seconds since last DGPS update
	*  (empty field) DGPS station ID number
	*  *47          the checksum data, always begins with *
	*/
	double latitude;
	double longitude;
	int tmp;
	int num;
	if (GPGGAstr[0] == '$') {
		strcpy(view[S_ALL], GPGGAstr);

		second_last = second;
		tmp = getComma(GPGGA_TIME, GPGGAstr);
		hour = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
		minute = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
		second = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');
		sprintf(view[S_TIME_LAST], "%s", view[S_TIME]);
		sprintf(view[S_TIME_ALT], "%2d:%02d:%02d", hour, minute, second);
		hour += 10;
		if (hour > 23) hour %= 24;
		if (hour < 12) {
			pm = 0;
		} else {
			pm = 1;
			if (hour > 12) hour -= 12;
		}
		sprintf(view[S_TIME], "%2d:%02d:%02d", hour, minute, second);
		tmp = getComma(GPGGA_LATITUDE, GPGGAstr);
		latitude = getDoubleNumber(&GPGGAstr[tmp]);
		tmp = getComma(GPGGA_LONGITUDE, GPGGAstr);
		longitude = getDoubleNumber(&GPGGAstr[tmp]);
		tmp = getComma(GPGGA_LATITUDE_DIR, GPGGAstr);
		sprintf(view[S_LATITUDE], "Lat: %10.4f %c ", latitude, GPGGAstr[tmp]);
		tmp = getComma(GPGGA_LONGITUDE_DIR, GPGGAstr);
		sprintf(view[S_LONGITUDE], "Lon: %10.4f %c ", longitude, GPGGAstr[tmp]);
		tmp = getComma(GPGGA_FIX, GPGGAstr);
		num = getIntNumber(&GPGGAstr[tmp]);
		fix = num;
		sprintf(view[S_FIX], "%d", num);
		strcpy(view[S_SAT_LAST], view[S_SAT]);
	  return;
		tmp = getComma(GPGGA_SAT, GPGGAstr);
		num = getIntNumber(&GPGGAstr[tmp]);
//		sprintf(view[S_SAT], "Sat: %d ", num);
		sprintf(view[S_SAT], "%d ", num);
	}
}

unsigned char LinkItGPS::getComma(unsigned char num, const char *str) {
	unsigned char i, j = 0;
	int len = strlen(str);
	for (i = 0; i < len; i++) {
		if (str[i] == ',') j++;
		if (j == num) return i + 1;
	}
	return 0;
}

double LinkItGPS::getDoubleNumber(const char *s) {
	char buf[10];
	unsigned char i;
	double rev;
	i = getComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = atof(buf);
	return rev;
}

int LinkItGPS::getIntNumber(const char *s) {
	char buf[10];
	unsigned char i;
	double rev = 0;
	i = getComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = atoi(buf);
	return rev;
}

