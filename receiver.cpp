#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sendframe.h"
#include "frameack.h"

using namespace std;

// GLOBAL
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;
int udpSocket;

// message (array of sendframe)
// buffer (array of sendframe)

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm  tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%Y-%m-%d %X] ", &tstruct);

    return buf;
}

void configureSetting(int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
} 

void sendACK(int seqNumber, int adWindowSize) {
	FrameAck frameack(seqNumber + 1, adWindowSize);
	int msgLength = frameack.getFrameNumber();
	sendto(udpSocket,frameack.toBytes(),7,0,(struct sockaddr *)&serverStorage,addr_size);
	cout << currentDateTime() << "Frame number " << frameack.getNextSeqNumber() <<" sent" << endl; 
}

void createSocket(int port){
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);   
    configureSetting(port);
	int hasilBind = bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	cout << hasilBind << endl;
}

int main(int argc, char* argv[]){

	int buffersize = 100;
	int WINDOW_SIZE = 5;
	addr_size = sizeof serverStorage;

	int portNum = atoi(argv[1]);
	unsigned char msg[100];
	
	// Membuat UDP socket
	createSocket(portNum);

	SendFrame * recvMesg;
	recvMesg = new SendFrame[buffersize];

	// int * recvWindow;
	// recvWindow = new int[WINDOW_SIZE];

	int i = 0;
	int check;

	while(1) {
		check = recvfrom(udpSocket,msg,9,0,(struct sockaddr *)&serverStorage, &addr_size);
		
		recvMesg[i] = SendFrame(msg);
		cout << currentDateTime() <<"Sequence Number : " << recvMesg[i].getSeqNumber() << "Data : " << recvMesg[i].getData() << endl;
		sendACK(recvMesg[i].getSeqNumber(), WINDOW_SIZE);
		i++;
	}

	close(udpSocket);
	cout << "exit" << endl;
}