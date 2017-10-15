#include "../frameack.h"

using namespace std;

int main(){
	FrameAck coba(3250000, 5);
	bitset<32> bit(3250000);
	cout<<bit<<endl << endl; 
	unsigned char* result = coba.toBytes();
	for(int i = 0; i < 6; i++){
		bitset<8> bit(result[i]);
		cout << bit << endl;
	}
	unsigned int seq = (result[1]<<24) + (result[2]<<16) + (result[3] << 8) + result[4];
	cout << seq <<endl;
	
	return 0;
}