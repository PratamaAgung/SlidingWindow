#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

struct sockaddr_in serverAddr;

// Melakukan konfigurasi koneksi berdasarkan port dan IP tertentu
void configureSetting(char IP[], int portNum) {
	  serverAddr.sin_family = AF_INET;
	  serverAddr.sin_port = htons(portNum);
	  serverAddr.sin_addr.s_addr = inet_addr(IP);
	  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
}

int main(int argc, char* argv[]){
	cout << "hello world\n\n";

	int portNum = 9876;

	// Membuat UDP socket
    int clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    cout << "Membuat socket untuk koneksi ke " << "123.345" << ":" << portNum << endl;
    configureSetting("123.345", portNum);

    
}