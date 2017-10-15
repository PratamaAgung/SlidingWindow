#ifndef _FRAMEACK_
#define _FRAMEACK_

#include <bits/stdc++.h>

#define ACK 0x6
#define defValue 0;

class FrameAck {
private:
    unsigned int nextSeqNumber;
    unsigned char adWindowSize;
    unsigned char checksum;

public:
    //constructor
    FrameAck() {
        nextSeqNumber = defValue;
        adWindowSize = defValue;
        checksum = defValue;
    };

    FrameAck(unsigned int nextSeqNumber, unsigned char adWindowSize) {
        this->nextSeqNumber = nextSeqNumber;
        this->adWindowSize = adWindowSize;
        this->checksum = 0;
    };

    unsigned char * toBytes() {
        unsigned char* send = new unsigned char[7];
        send[0] = ACK;
        send[1] = ((this->nextSeqNumber) >> 24) & 0xff;
        send[2] = ((this->nextSeqNumber) >> 16) & 0xff;
        send[3] = ((this->nextSeqNumber) >> 8) & 0xff;
        send[4] = this->nextSeqNumber & 0xff;
        send[5] = this-> adWindowSize;
        send[6] = this-> checksum;
        return send;
    }

    //setter
    void setNextSeqNumber(unsigned int nextSeqNumber) { this->nextSeqNumber = nextSeqNumber}
    void setAdWindowSize(unsigned int adWindowSize) { this->adWindowSize = adWindowSize}

    //getter
    unsigned int getNextSeqNumb() { return nextSeqNumber; }
    unsigned int getAdWindowSize() { return adWindowSize; }
    unsigned char getCheckSum() { return checksum; }
};

#endif