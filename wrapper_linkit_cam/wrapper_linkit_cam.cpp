#include "wrapper_linkit_cam.h"
#include <LTask.h>
#include <LWiFiServer.h>
#include <LWiFi.h>

void LinkItCam::clear_rx_buffer() {
	while (CAM_SERIAL.available()) CAM_SERIAL.read();
}

void LinkItCam::send_cmd(char cmd[], int cmd_len) {
	for (char i = 0; i < cmd_len; i++) CAM_SERIAL.print(cmd[i]);
}

void LinkItCam::pre_capture() {
	char cmd[] = { 0xaa, 0x01 | cam_address, 0x00, 0x07, 0x00, PIC_FMT };
	unsigned char resp[6];
	CAM_SERIAL.setTimeout(100);
	while (1) {
		clear_rx_buffer();
		send_cmd(cmd, 6);
		if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
		if (resp[0] == 0xaa && resp[1] == (0x0e | cam_address) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
	}
}

void LinkItCam::init() {
	cam_address = (CAM_ADDR << 5);
	CAM_SERIAL.begin(115200);
	char cmd[] = { 0xaa, 0x0d | cam_address, 0x00, 0x00, 0x00, 0x00 };
	unsigned char resp[6];
	CAM_SERIAL.setTimeout(500);
	while (1) {
		//clear_rx_buffer();
		send_cmd(cmd, 6);
		if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
		if (resp[0] == 0xaa && resp[1] == (0x0e | cam_address) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0) {
			if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
			if (resp[0] == 0xaa && resp[1] == (0x0d | cam_address) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break;
		}
	}
	cmd[1] = 0x0e | cam_address;
	cmd[2] = 0x0d;
	send_cmd(cmd, 6);
	pre_capture();
}

void LinkItCam::capture() {
	char cmd[] = { 0xaa, 0x06 | cam_address, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN >> 8) & 0xff ,0 };
	unsigned char resp[6];
	CAM_SERIAL.setTimeout(100);
	while (1) {
		clear_rx_buffer();
		send_cmd(cmd, 6);
		if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
		if (resp[0] == 0xaa && resp[1] == (0x0e | cam_address) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) break;
	}
	cmd[1] = 0x05 | cam_address;
	cmd[2] = 0;
	cmd[3] = 0;
	cmd[4] = 0;
	cmd[5] = 0;
	while (1) {
		clear_rx_buffer();
		send_cmd(cmd, 6);
		if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
		if (resp[0] == 0xaa && resp[1] == (0x0e | cam_address) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) break;
	}
	cmd[1] = 0x04 | cam_address;
	cmd[2] = 0x1;
	while (1) {
		clear_rx_buffer();
		send_cmd(cmd, 6);
		if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
		if (resp[0] == 0xaa && resp[1] == (0x0e | cam_address) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0) {
			CAM_SERIAL.setTimeout(1000);
			if (CAM_SERIAL.readBytes((char *)resp, 6) != 6) continue;
			if (resp[0] == 0xaa && resp[1] == (0x0a | cam_address) && resp[2] == 0x01) {
				pic_total_length = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
				break;
			}
		}
	}
}

void LinkItCam::send(LWiFiClient client) {
	if (DEBUG) {
		Serial.println("Sending cam...");
	}
//void LinkItCam::send() {
	image_data = "";
	//	timer_last = millis();
	///////capture();
	//timer = millis() - timer_last;
	//Serial.print("Captured in ms:");
	//Serial.println(timer);

	//timer_last = millis();


	unsigned int pktCnt = (pic_total_length) / (PIC_PKT_LEN - 6);

	//Serial.print("packet count:");
	//Serial.println(pktCnt);
	//Serial.print("pic total length:");
	//Serial.println(pic_total_length);

	//client.println("HTTP/1.1 200 OK");
	//client.println("Content-Type: image/jpeg");
	//client.println("Connection: close");
	//client.print("Content-Length: ");
	//client.println(pic_total_length);
	////client.println("Refresh: 2");
	//client.println();


	uint16_t cnt;

	if ((pic_total_length % (PIC_PKT_LEN - 6)) != 0) pktCnt += 1;
	char cmd[] = { 0xaa, 0x0e | cam_address, 0x00, 0x00, 0x00, 0x00 };
	//unsigned char pkt[PIC_PKT_LEN];
	CAM_SERIAL.setTimeout(1000);
	for (unsigned int i = 0; i < pktCnt; i++) {
		cmd[4] = i & 0xff;
		cmd[5] = (i >> 8) & 0xff;
		int retry_cnt = 0;
	   retry:
		delay(10);
		clear_rx_buffer();
		send_cmd(cmd, 6);
		cnt = CAM_SERIAL.readBytes((char *)pkt, PIC_PKT_LEN);
		unsigned char sum = 0;
		for (int y = 0; y < cnt - 2; y++) sum += pkt[y];
		if (sum != pkt[cnt - 2]) {
			if (++retry_cnt < 100) goto retry;
			else break;
		}

//		image_data << (const uint8_t *)&pkt[4];

//		strstream s;

//		image_data += String((const uint8_t *)&pkt[4]);
//		image_data += (char *)(uint8_t *)&pkt[4];
		client.write((const uint8_t *)&pkt[4], cnt - 6);
		//if (cnt != PIC_PKT_LEN) break;
	}
	cmd[4] = 0xf0;
	cmd[5] = 0xf0;
	send_cmd(cmd, 6);

	if (DEBUG) {
		Serial.println("Cam sent.");
	}


}
