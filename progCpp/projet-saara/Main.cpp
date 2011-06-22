#include "Main.h"
#include "Navigation.h" // A placer dans voiceReco.h

using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 8383


void Main::getOrientationUser(){
	IWRSENSDATA sensdata;
	LONG yaw = 0,pitch = 0,roll = 0, myaw = 0;
	if (IWRGetTracking(&yaw, &pitch, &roll) != ERROR_SUCCESS) {
		connexionGyroscope();
	}
    IWRGetSensorData(&sensdata);
    IWRGetMagYaw(&myaw);

	sprintf(buffer, _T("%s|%d;%d;%d\n"), buffer, yaw, pitch, roll);
}

void Main::sendDataUserToLua(SOCKET csock){
	getOrientationUser();

	int iResult;
	iResult = send(csock, buffer, (int)strlen(buffer), 0);
	if (iResult == SOCKET_ERROR){
		int err = WSAGetLastError();
		printf("Erreur: %d\n", err);
	}
}

void Main::DelaySecs(int secs){
   unsigned long int t=GetTickCount()+(1L*secs);
   while(GetTickCount()<t)
   {      /*  Wait !!! */   }
}

void VRPN_CALLBACK Main::handle_tracker(void* userData, const vrpn_TRACKERCB t ){
	if(t.sensor == 0 || t.sensor == 8 || t.sensor == 14){
		struct Datas dataStructType;
		Main::Datas *uData = &dataStructType;
		uData = (Main::Datas *)userData;

		if(t.sensor > 0){
			sprintf(uData->_this->buffer, "%s|%f;%f;%f", uData->_this->buffer, t.pos[0], t.pos[1], t.pos[2]);
			if(t.sensor == 14){uData->_this->sendDataUserToLua(uData->socketClient);}
		}else if(t.sensor == 0){
			sprintf(uData->_this->buffer, "%f;%f;%f", t.pos[0], t.pos[1], t.pos[2]);
		}
	}
}

void Main::connexionSocket(void){
	WSADATA WSAData;
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN sin;
    SOCKADDR_IN csin;
    WSAStartup(MAKEWORD(2,0), &WSAData);

	printf("Demarrage serveur...\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DEFAULT_PORT);
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));

    listen(sock, 0);

	vrpn_Tracker_Remote* vrpnTracker = new vrpn_Tracker_Remote( "Tracker0@localhost");

	int sinsize = sizeof(csin);
	if((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET){
		struct Datas d = {csock, this};
		Datas *socketData = &d;

		vrpnTracker->register_change_handler((Datas *)socketData, Main::handle_tracker);	
		printf("Un client vient de se connecter\n");
		
		while(1){
			DelaySecs(5);
			vrpnTracker->mainloop();
		}
	}
}

int Main::connexionGyroscope(void){
    IWRVERSION ver;
	if (IWROpenTracker() == ERROR_SUCCESS) {
        if (IWRGetVersion(&ver) == ERROR_SUCCESS) {
        }
		if (IWRSetFilterState) IWRSetFilterState(TRUE);
		IWRSetMagAutoCorrect(TRUE);
		printf("Gyroscope connecte\n");
		
		 return 1;
	} else {
		printf("Erreur : le gyroscope n'est pas connecte\n");

		return 0;
	}
}

int Main::initGyroscope(void){
	m_hIwear = LoadLibrary(_T("IWEARDRV.DLL"));
	if (m_hIwear) {
		IWROpenTracker = (PIWROPENTRACKER)GetProcAddress(m_hIwear, _T("IWROpenTracker"));
		IWRZeroSet = (PIWRZEROSET)GetProcAddress(m_hIwear, _T("IWRZeroSet"));
		IWRGetTracking = (PIWRGETTRACKING)GetProcAddress(m_hIwear, _T("IWRGetTracking"));
		IWRGetSensorData = (PIWRGETSENSORDATA)GetProcAddress(m_hIwear, _T("IWRGetSensorData"));
		IWRSetFilterState = (PIWRSETFILTERSTATE)GetProcAddress(m_hIwear, _T("IWRSetFilterState"));
		IWRGetVersion = (PIWRGETVERSION)GetProcAddress(m_hIwear, _T("IWRGetVersion"));
        IWRGetMagYaw = (PIWRGETMAGYAW)GetProcAddress(m_hIwear, _T("IWRGetMagYaw"));
        IWRSetMagAutoCorrect = (PIWRSETMAGAUTOCORRECT)GetProcAddress(m_hIwear, _T("IWRSetMagAutoCorrect"));
		
		return connexionGyroscope();
	}

	return 0;
}

Main::Main(void){
	//A placer dans voiceReco.cpp, si lutilisateur dit : Saara emmène-moi à ... (tag)
	Navigation navigation;
	navigation.searchMediaTag(14);

	if (initGyroscope()){
		connexionSocket();
	}
}

int main(){
	Main main2;

    return 0;
}
