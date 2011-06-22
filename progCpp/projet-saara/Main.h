#include "stdafx.h"

#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include "vrpn_Tracker.h"

#pragma comment(lib, "ws2_32.lib")

#define IWEARDRV_EXPLICIT
#include <iweardrv.h>

class Main{
public:
	HMODULE m_hIwear;
	char buffer[800];

	Main(void);
	int connexionGyroscope(void);
	int initGyroscope(void);
	void connexionSocket(void);
	void DelaySecs(int secs);
	static void VRPN_CALLBACK handle_tracker(void* userData, const vrpn_TRACKERCB t);
	struct Datas {
		SOCKET socketClient;
		Main *_this;
	};
	void getOrientationUser();
	void sendDataUserToLua(SOCKET csock);
};