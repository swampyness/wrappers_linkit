#include "wrapper_linkit_wifi.h"
#include <LTask.h>
#include <LWiFiServer.h>
#include <LWiFi.h>

LWiFiServer server(80);

void LinkItWifi::html_build() {
	html_page = "";
	if (content_id != CONTENT_IMAGE) {
		html_page += html_header;
		html_page += html_banner;
		html_page += html_body;
		html_page += html_footer;
	} else {
//		html_page += html_body;
	}
	client_body = "";
	client_body += client_header;
	client_body += "Content-Length: ";
	if (content_id != CONTENT_IMAGE) {
		client_body += html_page.length();
	} else {
		client_body += data_length;
	}
	client_body += "\n\n";
	client_body += html_page;
}

void LinkItWifi::html_init() {
	html_header = "";
	html_banner = "";
	html_body = "";
	html_footer = "";

	html_header += "<!DOCTYPE HTML>\n";
	html_header += "<html>\n";
	html_header += "<body>\n";

	html_banner += "<h1>Hello World!!!</h1>\n";
	html_banner += "<hr />\n";

	html_footer += "\n</body>\n";
	html_footer += "</html>\n";

	client_header = "";
	client_header += "HTTP/1.1 200 OK\n";
	client_header += "Content-Type: ";
	if (content_id != CONTENT_IMAGE) {
		client_header += "text/html\n";
	} else {
		client_header += "image/jpeg\n";
	}
	client_header += "Connection: close\n";
	//client_header += "Refresh: 2\n";
}

void LinkItWifi::status() {
	Serial.print("SSID: ");
	Serial.println(LWiFi.SSID());
	IPAddress ip = LWiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	Serial.print("subnet mask: ");
	Serial.println(LWiFi.subnetMask());
	Serial.print("gateway IP: ");
	Serial.println(LWiFi.gatewayIP());
	long rssi = LWiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}

void LinkItWifi::init() {
	html_init();
	LWiFi.begin();
	if (DEBUG) {
		Serial.print("WiFi status:");
		Serial.println(LWiFi.status());
		//String fv = LWiFi.firmwareVersion();
		//Serial.print("Firmware version:");
		//Serial.println(fv);
	}
	uint8_t macAddress[VM_WLAN_WNDRV_MAC_ADDRESS_LEN] = { 0 };
	LWiFi.macAddress(macAddress);
	
	if (DEBUG) {
		Serial.print("MAC Address is:");
		for (int i = 0; i < VM_WLAN_WNDRV_MAC_ADDRESS_LEN; ++i) {
			Serial.print(macAddress[i], HEX);
			Serial.print(":");
		}
		Serial.println();
	}

	if (DEBUG) {
		int ap_count = LWiFi.scanNetworks();
		Serial.print("number of WiFi AP found:");
		Serial.println(ap_count);
		for (int i = 0; i < ap_count; i++) {
			Serial.print(i);
			Serial.print(" ");
			Serial.print(LWiFi.SSID(i));
			Serial.println();
		}
		Serial.println("Connecting to AP");
	}
	while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD))) {
		if (DEBUG) Serial.println("Re-connecting to AP");
		delay(1000);
	}

	if (DEBUG) {
		status();
		Serial.println("Server Started");
	}
	server.begin();
}

void LinkItWifi::web() {
	String line_tmp = "";
	String name_tmp = "";
	String val_tmp = "";
	int sep;
	int sep2;
	char read_tmp = 0;
	byte is_first = 1;
	client_connected = 0;
	client = server.available();
	if (client) {
		if (client.connected()) {
			if (DEBUG) Serial.println("client connected");
			//flush();
			while (client.available() && read_tmp != -1) {
				read_tmp = client.read();
				if (read_tmp != -1) line_tmp += read_tmp;
				if (read_tmp == '\n') {
					sep = line_tmp.indexOf("GET") + 4;
					if (sep > 3) {

						sep2 = line_tmp.indexOf(" HTTP") - 0;
						val_tmp = line_tmp.substring(sep, sep2);

						//if (DEBUG) Serial.print(line_tmp);

						if (DEBUG) {
							Serial.print("[Query]    ");
							Serial.println(val_tmp);

							if (val_tmp == "/capture") content_id = CONTENT_IMAGE;
						}
					}
					sep = line_tmp.indexOf(":");
					if (1 == 1 && sep > 2) {
						sep = line_tmp.indexOf(":");
						name_tmp = line_tmp.substring(0, sep);
//						sep = line_tmp.indexOf(":") + 2;
						val_tmp = line_tmp.substring(sep + 2, line_tmp.length() - 1);
						if (DEBUG) {
							//							Serial.println();
							//							Serial.print("name:");
							Serial.print("[");
							Serial.print(name_tmp);
							Serial.print("]   ");
							Serial.println(val_tmp);
							//if (name_tmp == "Cache-Control") is_first = 0;
						}
					}
					line_tmp = "";
				}
			}
			if (is_first != 0) {
				if (content_id != CONTENT_IMAGE) {
					html_build();
					client.print(client_body);
					///flush();
//					if (DEBUG) Serial.print("sent");
					client.stop();
					if (DEBUG) Serial.println("client stop");
				} else {
					client_connected = 1;
				}
			}
		}
	}
}

void LinkItWifi::flush() {
	String line_tmp = "";
	char read_tmp = 0;
	if (DEBUG) Serial.println("\nflushing...");
	client.flush();
	while (client.available() && read_tmp != -1) {
		read_tmp = client.read();
		if (read_tmp != -1) {
			line_tmp += read_tmp;
		}
		if (read_tmp == '\n') {
			if (DEBUG) Serial.print(line_tmp);
			line_tmp = "";
		}
	}
	if (DEBUG) Serial.println("\nflushed");
}



