#include "wrapper_linkit_wifi.h"
#include <LTask.h>
#include <LWiFiServer.h>
#include <LWiFi.h>

LWiFiServer server(80);

String LinkItWifi::link(String url, String label) {
	String str_link = "";
	str_link += "<a href ='";
	str_link += url;
	str_link += "'>";
	str_link += label;
	str_link += "</a>";
	str_link += "\n";
	return str_link;
}

String LinkItWifi::span(String span_name, String span_data, String span_class) {
	String temp_span = "";
	temp_span += "<span";
	if (span_name.length() > 1) {
		temp_span += " name='";
		temp_span += span_name;
		temp_span += "'";
	}
	if (span_class.length() > 1) {
		temp_span += " class='";
		temp_span += span_class;
		temp_span += "'";
	}
	temp_span += ">";
	if (span_class == "row") temp_span += "\n";
	temp_span += span_data;
	temp_span += "</span>";
	temp_span += "\n";
	return temp_span;
}

void LinkItWifi::html_build() {
//	html_init();
	html_page = "";

	html_header = "";
	html_header += "<!DOCTYPE HTML>\n";
	html_header += "<html>\n";
	html_header += "<head>\n";
	html_header += "<title>";
	html_header += html_title;
	html_header += "</title>\n";
	html_header += "<style type='text/css'>\n";
	html_header += "body {font-family:arial;font-size:14pt;background-color:white;color:black}\n";
	html_header += ".banner {height:40px}\n";
	html_header += ".footer {height:20px}\n";
	html_header += ".row {display:block}\n";
	html_header += ".cell {display:inline-block;min-height:30px;min-width:150px;vertical-align:top}\n";
	html_header += ".bold {text-weight:bold}\n";
	html_header += ".right {text-align:right}\n";
	html_header += ".center {text-align:center}\n";
	html_header += ".tiny {font-size:8pt}\n";
	html_header += ".light {color:lightgray}\n";
	html_header += ".dark {color:darkgray}\n";
	html_header += ".red {background-color:red}\n";
	html_header += ".green {background-color:green}\n";
	html_header += ".blue {background-color:blue}\n";
	html_header += ".orange {background-color:orange}\n";
	html_header += ".yellow {background-color:yellow}\n";
	html_header += ".magenta {background-color:magenta}\n";
	html_header += ".violet {background-color:violet}\n";
	html_header += ".graph {display:inline-block;width:1px;margin-left:0px}\n";
	html_header += ".board {display:inline-block;height:100px;width:260;background-color:black}\n";
	html_header += "</style>\n";
	html_header += "</head>\n";
	html_header += "<body>\n";

	double temp_sec = millis() - timer_start;
	temp_sec /= 1000.0;
	String temp_timer = "";
	temp_timer += "Processed in ";
	temp_timer += temp_sec;
	temp_timer += " seconds.";
	html_footer = "";
	html_footer += span("", temp_timer, "row footer center tiny");
	html_footer += "</body>";
	html_footer += "\n";
	html_footer += "</html>";
	html_footer += "\n";

	if (content_id != CONTENT_IMAGE) {
		html_page += html_header;
		html_page += html_banner;
		html_page += html_body;
		html_page += html_footer;
	}

	client_header = "";
	client_header += "HTTP/1.1 200 OK";
	client_header += "\n";
	client_header += "Content-Type: ";
	if (content_id != CONTENT_IMAGE) {
		client_header += "text/html";
	} else {
		client_header += "image/jpeg";
	}
	client_header += "\n";
	client_header += "Refresh: 8\n";
	client_header += "Content-Length: ";
	if (content_id != CONTENT_IMAGE) {
		client_header += html_page.length();
	} else {
		client_header += data_length;
	}
	client_header += "\n";
	client_header += "Connection: Closed";
	client_header += "\n";

	client_body = "";
	client_body += client_header;
	client_body += "\n";
	client_body += html_page;
}

void LinkItWifi::html_init() {
	html_header = "";
	html_banner = "";
	html_body = "";
	html_footer = "";

	html_banner += span("", link("/", html_title), "row banner");

	client_header = "";
	client_header += "HTTP/1.1 200 OK\n";
}

String get_ip(IPAddress ip) {
	String temp_ip;
	char c_ip[20];
	sprintf(c_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	temp_ip += c_ip;
	return temp_ip; 
}

void LinkItWifi::status() {
	ap_ssid += LWiFi.SSID();
	ap_signal += LWiFi.RSSI();
	
//	IPAddress ip = LWiFi.localIP();

//	char c_ip[20];
//	sprintf(c_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	server_ip += get_ip(LWiFi.localIP());
	server_subnet_mask += get_ip(LWiFi.subnetMask());
	server_gateway += get_ip(LWiFi.gatewayIP());

	if (DEBUG) {
		Serial.println("AP SSID:" + ap_ssid);
		Serial.println("AP Signal Strength (RSSI):" + ap_signal + " dBm");
		Serial.println("Server IP Address:" + server_ip);
		Serial.println("Server Subnet Mask:" + server_subnet_mask);
		Serial.println("Server Gateway IP:" + server_gateway);
	}
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
	byte capture_flag = 0;
	int sep;
	int sep2;
	char read_tmp = 0;
	client_connected = 0;
	client = server.available();
	if (client) {
		if (client.connected() && client_connected == 0) {
//			if (DEBUG) Serial.println("client connected");
			timer_start = millis();
			while (client.available() && read_tmp != -1 && client_connected == 0) {
				read_tmp = client.read();
				if (read_tmp != -1) line_tmp += read_tmp;
				if (read_tmp == '\n') {
					sep = line_tmp.indexOf("GET") + 4;
					if (sep > 3) {
						sep2 = line_tmp.indexOf(" HTTP") - 0;
						val_tmp = line_tmp.substring(sep, sep2);
						if (DEBUG) {
							//Serial.print(line_tmp);
							Serial.print("[Query] ");
							Serial.println(val_tmp);
						}
						if (val_tmp == "/capture") {
							capture_flag = 1;
							content_id = CONTENT_IMAGE;
							client_connected == 0;
							if (DEBUG) Serial.println("capture query");
						}
					}
					if (0 == 1) {
						sep = line_tmp.indexOf(":");
						if (sep > 2) {
							sep = line_tmp.indexOf(":");
							name_tmp = line_tmp.substring(0, sep);
							//						sep = line_tmp.indexOf(":") + 2;
							val_tmp = line_tmp.substring(sep + 2, line_tmp.length() - 1);
							if (DEBUG) {
								//	Serial.println();
								//	Serial.print("name:");
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
			}
			if (content_id != CONTENT_IMAGE) {
				html_build();
				//if (DEBUG) {
				//	Serial.print("client body:");
				//	Serial.println(client_body);
				//}
				client.print(client_body);
				//client.flush();
				flush();
				client.stop();
//				if (DEBUG) Serial.println("client stop");
			}
		}
	}
}

void LinkItWifi::flush() {
	String line_tmp = "";
	char read_tmp = 0;
//	if (DEBUG) Serial.println("\nflushing...");
	client.flush();
	while (client.available() && read_tmp != -1) {
		read_tmp = client.read();
		if (read_tmp != -1) {
			line_tmp += read_tmp;
		}
		if (read_tmp == '\n') {
//			if (DEBUG) Serial.print(line_tmp);
			line_tmp = "";
		}
	}
//	if (DEBUG) Serial.println("\nflushed");
}



