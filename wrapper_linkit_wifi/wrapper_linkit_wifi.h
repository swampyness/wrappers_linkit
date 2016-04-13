#ifndef _WRAPPER_LINKIT_WIFI_H
#define _WRAPPER_LINKIT_WIFI_H
#include <Arduino.h>
#include <LTask.h>
#include <LWiFiServer.h>
#include <LWiFi.h>
class LinkItWifi {
	public:
		#define WIFI_AP			"your_wifi_name"
		#define WIFI_PASSWORD	"your_wifi_password"
		#define WIFI_AUTH		LWIFI_WPA			// LWIFI_OPEN LWIFI_WPA LWIFI_WEP			
		#define DEBUG			1
		#define CONTENT_HTML	0
		#define CONTENT_IMAGE	1
		#define CONTENT_FRAME	2		
		LWiFiClient client;
		String html_body;
		String html_title;
		String server_ip;
		String server_mac;
		String server_subnet_mask;
		String server_gateway;
		String ap_ssid;
		String ap_signal;
		String ap_count;
		String ap_list[20];
		String client_ip;
		int data_length;
		int server_available;
		int client_connected;
		int content_id;
		String client_body;
		void init();
		void web();
		void status();
		void html_build();
		void html_init();
		String link(String url, String label);
		String span(String span_name, String span_data, String span_class);
	private:
		long timer_start;
		String html_header;
		String html_banner;
		String html_footer;
		String html_page;
		String client_header;
		void client_init();
		void flush();
};
#endif

