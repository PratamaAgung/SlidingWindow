#include "sendframe.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

/* GLOBAL */
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;
int usedSocket;

void configureSetting(char* IP, int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
}

void sendMessageFrame(int usedSocket, SendFrame frame){
	int msgLength = frame.getFrameNumber();
	sendto(usedSocket, frame.toBytes(), 9, 0, (struct sockaddr*) &serverAddr, addr_size);
	cout << "Frame nu " << frame.getSeqNumber() <<" sent" << endl; 
}

int main(int argc, char* argv[]){
	string fileName = argv[1];
	char* IP = argv[4];
	int portNum = atoi(argv[5]);

	cout << "Reading from file " << fileName << endl;

	//Create  UDP Socket
	usedSocket = socket(PF_INET, SOCK_DGRAM, 0);
	configureSetting(IP, portNum);
	cout << "Open socket to " << IP << ":" << portNum << endl;

	addr_size = sizeof serverAddr;

	sendMessageFrame(usedSocket, SendFrame('c', 1));
	return 0;
}