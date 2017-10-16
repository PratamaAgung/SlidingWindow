#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sendframe.h"
#include "frameack.h"

using namespace std;

// GLOBAL
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

int udpSocket;

void configureSetting(int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
} 

void sendACK(int seqNumber, int adWindowSize) {
	FrameAck frameack(seqNumber, adWindowSize);
	int msgLength = frameack.getFrameNumber();
	sendto(udpSocket, frameack.toBytes(), 6, 0, (struct sockaddr*) &serverAddr, addr_size);
	cout << "Frame number " << frameack.getNextSeqNumber() <<" sent" << endl; 
}

int main(int argc, char* argv[]){

	int buffersize = 100;
	int WINDOW_SIZE = 5;
	addr_size = sizeof serverStorage;

	int portNum = 9876;
	unsigned char msg[100];
	
	// Membuat UDP socket
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);   
    configureSetting(portNum);
	int hasilBind = bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	cout << hasilBind << endl;

	// ----------------------------------------------------

	SendFrame * recvMesg;
	recvMesg = new SendFrame[buffersize];

	int i = 0;
	int check;

	while(1) {
		cout << "wait " << i << endl;

		check = recvfrom(udpSocket,msg,9,0,(struct sockaddr *)&serverStorage, &addr_size);
		
		recvMesg[i] = SendFrame(msg);

		cout << "Sequence Number : " << recvMesg[i].getSeqNumber() << endl;
		cout << "Data : " << recvMesg[i].getData() << endl;

		i++;
	}

	close(udpSocket);
	cout << "exit" << endl;
}