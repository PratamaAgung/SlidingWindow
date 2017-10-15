#ifndef __SENDFRAME__
#define __SENDFRAME__

#include <bits/stdc++.h>

#define SOH 0x1
#define STX 0x2
#define ETX 0x3
#define defValue 0

class SendFrame{
	private:
		unsigned int seqNumber;
		unsigned char data;
		unsigned char checksum;
	public:
		SendFrame(){
			seqNumber = defValue;
			data = defValue;
			checksum = defValue;
		}

		SendFrame(unsigned char data, unsigned int seqNumber){
			this->data = data;
			this->seqNumber = seqNumber;
			this->checksum = 0;
		}

		unsigned char* toBytes(){
			unsigned char* send = new unsigned char[9];
			send[0] = SOH;
			send[1] = ((this->seqNumber) >> 24) & 0xff;
			send[2] = ((this->seqNumber) >> 16) & 0xff;
			send[3] = ((this->seqNumber) >> 8) & 0xff;
			send[4] = this->seqNumber & 0xff;
			send[5] = STX;
			send[6] = this-> data;
			send[7] = ETX;
			send[8] = this->checksum;
			return send;
		}
};

#endif