#include "sendframe.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

/* GLOBAL */
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;
int usedSocket;
SendFrame* buffer;
int lengthFile;


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
		sendMessageFrame(usedSocket, buffer[i]);
		i++;
	}
}

void createSocket(char* IP, int portNum){
	usedSocket = socket(PF_INET, SOCK_DGRAM, 0);
	configureSetting(IP, portNum);
	cout << currentDateTime() << "Open socket to " << IP << ":" << portNum << endl;
}


int main(int argc, char* argv[]){
	string fileName = argv[1];
	char* IP = argv[4];
	int portNum = atoi(argv[5]);
	int windowsize = atoi(argv[2]);
	int bufferSize = atoi(argv[3]);

	buffer = new SendFrame[bufferSize];
	cout << currentDateTime() << "Reading from file " << fileName << endl;
	fillBuffer(fileName);

	createSocket(IP, portNum);
	addr_size = sizeof serverAddr;
	sendFile();
	
	return 0;
}