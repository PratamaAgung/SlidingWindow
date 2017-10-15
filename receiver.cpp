#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

void configureSetting(int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
} 

int main(int argc, char* argv[]){
	cout << "hello world\n\n";

	int WINDOW_SIZE = 5;
	addr_size = sizeof serverStorage;

	int portNum = 9876;
	char msg[100];


	// Membuat UDP socket
    int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
    // cout << "Membuat socket untuk koneksi ke " << "1" << ":" << portNum << endl;
    // configureSetting("1", portNum);

    configureSetting(portNum);
	int hasilBind = bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	cout << "buat baru" << endl;
	
	cout << hasilBind << endl;
	int i = 0;

	int check;

	while(1) {
		cout << "wait " << i << endl;
		i++;

		check = recvfrom(udpSocket,msg,9,0,(struct sockaddr *)&serverStorage, &addr_size);
		bitset<8> bit(msg[6]);
		char data = msg[6];
		cout << bit << endl;
		cout << data << endl;
	}

	close(udpSocket);
	cout << "exit" << endl;
}