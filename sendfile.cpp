#include "sendframe.h"
#include "frameack.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

/* GLOBAL */
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;
int usedSocket;
mutex mtx;

int bufferSize;
SendFrame* buffer;

int windowsize;
int lengthFile;
int lowerWindow;
int upperWindow;
bool sent[5];


const std::string currentDateTime() {
    time_t now = time(0);
    struct tm  tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%Y-%m-%d %X] ", &tstruct);

    return buf;
}

void configureSetting(char* IP, int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
}

void sendMessageFrame(int usedSocket, SendFrame frame){
	int msgLength = frame.getFrameNumber();
	sendto(usedSocket, frame.toBytes(), 9, 0, (struct sockaddr*) &serverAddr, addr_size);
	cout << currentDateTime() << "Frame number " << frame.getSeqNumber() <<" sent" << endl; 
}

void fillBuffer(string fileName){
	ifstream fin(fileName);
	int seqNumber = 0;
	while(1){
		char c;
		fin >> noskipws >> c;
		if(!fin.eof()){
			buffer[seqNumber] = SendFrame(c, seqNumber);
		} else {
			break;
		}
		seqNumber++;
	}
	lengthFile = seqNumber;
}

void sendFile(){
	int i = 0;
	while(i < lengthFile){
		if(!sent[i - lowerWindow]){
			sendMessageFrame(usedSocket, buffer[i]);
		}
		i = (i+1 - lowerWindow)%windowsize + lowerWindow;
	}
}

void createSocket(char* IP, int portNum){
	usedSocket = socket(PF_INET, SOCK_DGRAM, 0);
	configureSetting(IP, portNum);
	cout << currentDateTime() << "Open socket to " << IP << ":" << portNum << endl;
}

void receiveACK(){
	unsigned char* msg;
	while(1){
		recvfrom(usedSocket,msg,7,0,(struct sockaddr *)&serverStorage, &addr_size);
		if(msg){
			FrameAck ack(msg);
			unsigned int nextSeq =  ack.getNextSeqNumber();
			cout << currentDateTime() << "Received ACK to" << nextSeq << endl;
			mtx.lock();
			upperWindow += nextSeq + lowerWindow;
			lowerWindow = nextSeq;
			mtx.unlock();
		}
	}
}

int main(int argc, char* argv[]){
	string fileName = argv[1];
	char* IP = argv[4];
	int portNum = atoi(argv[5]);
	windowsize = atoi(argv[2]);
	bufferSize = atoi(argv[3]);

	//init
	lowerWindow = 0;
	upperWindow = windowsize - 1;
	buffer = new SendFrame[bufferSize];
	for(int i = 0; i < 5; i++){
		sent[i] = false;
	}

	//read file
	cout << currentDateTime() << "Reading from file " << fileName << endl;
	fillBuffer(fileName);

	//create socket and send file
	createSocket(IP, portNum);
	addr_size = sizeof serverAddr;
	
	//communicate with receiver
	thread send(sendFile);
	thread receive(receiveACK);

	send.join();
	receive.join();
	
	return 0;
}