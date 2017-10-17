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
int buffersize;
int WINDOW_SIZE;

// message (array of sendframe)
vector<SendFrame> recvMsg;

// buffer (array of sendframe)
SendFrame * bufferMsg;

// receiver window
int lowerWindow; // SeqNumber yang ditunggu paling kiri (awal)
int upperWindow;
bool* recvWindow;

// Header Fungsi Array
void geserArray(bool* arrayBool,int len, int geser);
void setAllFalse(bool* arrayBool, int len);
bool isAllFalse(bool* arrayBool, int len);
int searchConsecutiveTrue(bool* arrayBool, int len);

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

void processMsg(){

}

int i = 0;
void waitForMsg(){
	unsigned char msg[10];
	
	while(1) {
		recvfrom(udpSocket,msg,9,0,(struct sockaddr *)&serverStorage, &addr_size);
		SendFrame tempFrame(msg); 

		// cout << "lower window : " << lowerWindow << endl;
		cout << currentDateTime() <<"Sequence Number : " << tempFrame.getSeqNumber() << " - Data : " << tempFrame.getData() << endl;

		int tempSeq = tempFrame.getSeqNumber() ;
		if(tempSeq == lowerWindow){
			if(isAllFalse(recvWindow, WINDOW_SIZE)){
				sendACK(lowerWindow, WINDOW_SIZE);
				lowerWindow++;
				upperWindow++;				
			}else{
				recvWindow[tempSeq-lowerWindow] = true;
				int newLower = searchConsecutiveTrue(recvWindow, WINDOW_SIZE);
				lowerWindow += newLower;
				geserArray(recvWindow, WINDOW_SIZE, newLower);
				sendACK(lowerWindow-1, WINDOW_SIZE);
			}

			bufferMsg[i] = SendFrame(msg);			
		}else if(tempSeq > lowerWindow){
			if(tempSeq < lowerWindow + WINDOW_SIZE){
				if(lowerWindow == 0){
					recvWindow[tempSeq - lowerWindow] = true;
				}else{
					recvWindow[tempSeq - lowerWindow] = true;
					sendACK(lowerWindow-1, WINDOW_SIZE);
				}
				bufferMsg[i] = SendFrame(msg);
			}
		}else{ // SeqNumber < lowerWindow
			sendACK(lowerWindow-1, WINDOW_SIZE);
		}
		// sendACK(bufferMsg[i].getSeqNumber(), WINDOW_SIZE);
		i++;
	}
}

int main(int argc, char* argv[]){

	if(argc != 2){
		cout << "Port number" << endl;
		return 0;	
	}

	buffersize = 100;
	WINDOW_SIZE = 5;
	addr_size = sizeof serverStorage;

	//init
	lowerWindow = 0;
	upperWindow = WINDOW_SIZE - 1;
	recvWindow = new bool[WINDOW_SIZE];
	setAllFalse(recvWindow, WINDOW_SIZE);

	// Set Port
	int portNum = atoi(argv[1]);
	
	// Membuat UDP socket
	createSocket(portNum);

	// buat buffer message
	bufferMsg = new SendFrame[buffersize];

	//communicate with receiver
	thread receiveMsg(waitForMsg);

	receiveMsg.join();

	close(udpSocket);
	cout << "exit" << endl;
}

// Fungsi Array
void geserArray(bool* arrayBool,int len, int geser){
	for(int i = 0; i < len-geser;i++){
		arrayBool[i] = arrayBool[i+geser];
	}
	for(int i = len-geser; i < len; i++){
		arrayBool[i] = false;
	}
}

int searchConsecutiveTrue(bool* arrayBool, int len){
	int ret = 0;
	while(arrayBool[ret] && ret < len){
		ret++;
	}
	return ret;
}

bool isAllFalse(bool* arrayBool, int len){
	for(int i = 0; i < len; i++){
		if(arrayBool[i]){
			return false;
		}
	}
	return true;
}

void setAllFalse(bool* arrayBool, int len){
	for(int i = 0; i < len; i++){
		arrayBool[i] = false;	
	}
}