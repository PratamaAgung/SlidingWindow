#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

// // Melakukan konfigurasi koneksi berdasarkan port dan IP tertentu
// void configureSetting(char IP[], int portNum) {
// 	  serverAddr.sin_family = AF_INET;
// 	  serverAddr.sin_port = htons(portNum);
// 	  serverAddr.sin_addr.s_addr = inet_addr(IP);
// 	  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
// }

// Melakukan konfigurasi koneksi berdasarkan port tertentu
void configureSetting(int portNum) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
} 

// // Menerima message
// void rcvMsg(int udpSocket) {
// 	char msg[100];
// 	bool approved[WINDOW_SIZE+1];
// 	setAllFalse(approved, WINDOW_SIZE);
// 	while (true) {
// 		recvfrom(udpSocket,msg,300,0,(struct sockaddr *)&serverStorage, &addr_size);
// 		for(int i=0; i<strlen(msg); i++) printf("%02hhX ", msg[i]);
// 		printf("\n");
// 		TransmitterFrame frame(msg);
// 		//printf("Frame : "); frame.printBytes();
// 		if(!frame.isError()) {
// 			sendACK(frame.getFrameNumber(), udpSocket);
// 			//cout << frame.getData() <<endl;
// 			if(!approved[frame.getFrameNumber()]) {
// 				//cout << frame.getData() <<endl;
// 				buffer.push_back(frame);
// 				//printf("Frame Number : ");
// 				//printf("%d\n", buffer.top().getFrameNumber()); 
// 				//cout << buffer.size() << endl;
// 				approved[frame.getFrameNumber()] = true;
// 				if(isAllTrue(approved, WINDOW_SIZE+1)) {
// 					setAllFalse(approved, WINDOW_SIZE+1);
// 				}
// 			}
// 		} else {
// 			sendNAK(frame.getFrameNumber(), udpSocket);
// 		}
		
// 	}
// } 

void setAllFalse(bool approved[], int length) {
	for(int i=0; i<length; i++) {
		approved[i] = false;
	}
}


int main(int argc, char* argv[]){
	cout << "hello world\n\n";

	int WINDOW_SIZE = 5;
	addr_size = sizeof serverStorage;

	int portNum = 9876;
	char msg[100];
	bool approved[WINDOW_SIZE+1];
	setAllFalse(approved, WINDOW_SIZE);


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

		check = recvfrom(udpSocket,msg,300,0,(struct sockaddr *)&serverStorage, &addr_size);
		// for(int i=0; i<strlen(msg); i++) printf("%02hhX ", msg[i]);
		// printf("\n");
		// TransmitterFrame frame(msg);
		// //printf("Frame : "); frame.printBytes();
		// if(!frame.isError()) {
		// 	sendACK(frame.getFrameNumber(), udpSocket);
		// 	//cout << frame.getData() <<endl;
		// 	if(!approved[frame.getFrameNumber()]) {
		// 		//cout << frame.getData() <<endl;
		// 		buffer.push_back(frame);
		// 		//printf("Frame Number : ");
		// 		//printf("%d\n", buffer.top().getFrameNumber()); 
		// 		//cout << buffer.size() << endl;
		// 		approved[frame.getFrameNumber()] = true;
		// 		if(isAllTrue(approved, WINDOW_SIZE+1)) {
		// 			setAllFalse(approved, WINDOW_SIZE+1);
		// 		}
		// 	}
		// } else {
		// 	sendNAK(frame.getFrameNumber(), udpSocket);
		// }
	}

	close(udpSocket);
	cout << "exit" << endl;
}