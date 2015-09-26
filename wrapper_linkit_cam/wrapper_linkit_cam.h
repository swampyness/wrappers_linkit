#ifndef _WRAPPER_LINKIT_CAM_H
#define _WRAPPER_LINKIT_CAM_H
#include <Arduino.h>
#include <LTask.h>
#include <LWiFiServer.h>
#include <LWiFi.h>
class LinkItCam {
	public:
		#define DEBUG				1
		#define PIC_FMT_VGA			0x07 // 640x480
		#define PIC_FMT_CIF			0x05 // 320x240
		#define PIC_FMT_OCIF		0x03 // 160x120
		//#define PIC_FMT			PIC_FMT_OCIF
		//#define PIC_FMT			PIC_FMT_CIF
		#define PIC_FMT				PIC_FMT_VGA
		#define CAM_ADDR			0
		#define CAM_SERIAL			Serial1
		//#define PIC_PKT_LEN		128 // 0x100
		//#define PIC_PKT_LEN		256
		//#define PIC_PKT_LEN		512
		//#define PIC_PKT_LEN		1024
		//#define PIC_PKT_LEN		2048
		//#define PIC_PKT_LEN		4096
		//#define PIC_PKT_LEN		8192
		//#define PIC_PKT_LEN		8192
		#define PIC_PKT_LEN			12288
		byte cam_address;
		//const byte cam_address = (CAM_ADDR << 5);
		String image_data;
		unsigned char pkt[PIC_PKT_LEN];
		unsigned long pic_total_length;
			
		void init();
		void capture();
		void send(LWiFiClient client);
private:
		void pre_capture();
		void clear_rx_buffer();
		void send_cmd(char cmd[], int cmd_len);

	};
#endif

