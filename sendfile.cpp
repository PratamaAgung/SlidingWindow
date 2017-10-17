#include "sendframe.h"
#include "frameack.h"
#include "windowstatus.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <thread>
#include <mutex>

#define TIME_OUT 2000

using namespace std;

/* GLOBAL */
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;
int usedSocket;
mutex mtx;

vector<SendFrame> bufferData;
int bufferSize;
SendFrame* buffer;

int windowsize;
int lengthFile;
int lowerWindow;
int upperWindow;
WindowStatus* status;
bool finish;

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

void sendToBuffer() {
	int dataLength = bufferData.size();
	if(dataLength < bufferSize) {
		for(int i=0; i<dataLength; i++) {
			buffer[i] = bufferData.front();
			bufferData.erase(bufferData.begin());
		}
	}
	else {
		for(int i=0; i<bufferSize; i++) {
			buffer[i] = bufferData.front();
			bufferData.erase(bufferData.begin());
		}
	}
}

void fillBuffer(string fileName){
	ifstream fin(fileName);
	int seqNumber = 0;
	while(true){
		char c;
		fin >> noskipws >> c;
		if(!fin.eof()){
			bufferData.push_back(SendFrame(c, seqNumber));
		} else {
			break;
		}
		seqNumber++;
	}
	lengthFile = seqNumber;
}

void sendFile(){
	int i = 0;
	while(!finish){
		mtx.lock();
		if(i >= lowerWindow && (i-lowerWindow) < windowsize && status[i - lowerWindow].getStatus() == 0){
			sendMessageFrame(usedSocket, buffer[i]);
			status[i - lowerWindow].setStatus(1);
			status[i - lowerWindow].setTime(clock());
		}
		i = ((i+1 - lowerWindow)%windowsize + lowerWindow < lengthFile)?((i+1 - lowerWindow)%windowsize + lowerWindow):(lowerWindow);
		mtx.unlock();
	}
}

void createSocket(char* IP, int portNum){
	usedSocket = socket(PF_INET, SOCK_DGRAM, 0);
	configureSetting(IP, portNum);
	cout << currentDateTime() << "Open socket to " << IP << ":" << portNum << endl;
}

void receiveACK(){
	unsigned char* msg;
	while(!finish){
		recvfrom(usedSocket,msg,7,0,(struct sockaddr *)&serverStorage, &addr_size);
		if(msg){
			FrameAck ack(msg);
			unsigned int nextSeq =  ack.getNextSeqNumber();
			cout << currentDateTime() << "Received ACK to " << nextSeq << endl;
			mtx.lock();
			if (windowsize > ack.getAdWindowSize()){
				windowsize = ack.getAdWindowSize();
			}
			if(nextSeq > lowerWindow && nextSeq <= upperWindow + 1){
				for(int i = windowsize-1; (windowsize - (nextSeq - lowerWindow == 0))?(i != -1):(i >= windowsize - (nextSeq - lowerWindow)); i--){
					status[i].setStatus(0);
				}
				lowerWindow = (nextSeq < lengthFile)?(nextSeq):(lengthFile-1);
				upperWindow = (lowerWindow + windowsize - 1< lengthFile)?(lowerWindow + windowsize - 1):(lengthFile-1);
				if(nextSeq >= lengthFile){
					finish = true;
					sendMessageFrame(usedSocket, SendFrame(0, -1));
				} else if (nextSeq%bufferSize == 0){
					sendToBuffer();
				}
			}
			mtx.unlock();
		}
	}
}

void timeOutManager(){
	while(!finish){
		mtx.lock();
		for(int i = 0; i < windowsize; i++){
			if(float(clock() - status[i].getTime())/(CLOCKS_PER_SEC/1000) > TIME_OUT){
				status[i].setStatus(0);
				// status[i].setTime(clock());
			}
		}
		mtx.unlock();
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
	status = new WindowStatus[windowsize];
	for(int i = 0; i < windowsize; i++){
		status[i] = WindowStatus();
	}
	// bufferDone = false;
	finish = false;

	//read file
	cout << currentDateTime() << "Reading from file " << fileName << endl;
	fillBuffer(fileName);

	sendToBuffer();

	//create socket and send file
	createSocket(IP, portNum);
	addr_size = sizeof serverAddr;
	
	//communicate with receiver
	thread send(sendFile);
	thread receive(receiveACK);
	thread timeout(timeOutManager);

	send.join();
	receive.join();
	timeout.join();
	
	return 0;
}