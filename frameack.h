#ifndef _FRAMEACK_
#define _FRAMEACK_

#include <bits/stdc++.h>

#define ACK 0x6

class FrameAck {
private:
    unsigned int nextSeqNumber;
    unsigned char adWindowSize;
    unsigned char checksum;

public:
    //constructor
    FrameAck() {
        nextSeqNumber = 0;
        adWindowSize = 0;
        checksum = 0;
    };

    FrameAck(unsigned int nextSeqNumber, unsigned char adWindowSize) {
        this->nextSeqNumber = nextSeqNumber;
        this->adWindowSize = adWindowSize;
        this->checksum = getCheckSum();
    };

    FrameAck(unsigned char* msg){
        this->nextSeqNumber = (msg[1] << 24) + (msg[2] << 18) + (msg[3] << 8) + msg[4];
        this->adWindowSize = msg[5];
        this->checksum = msg[6]; 
    }

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

    //isError
    bool isError() {
        return this->checksum == getCheckSum();
    }

    //setter
    void setNextSeqNumber(unsigned int nextSeqNumber) { this->nextSeqNumber = nextSeqNumber;}
    void setAdWindowSize(unsigned int adWindowSize) { this->adWindowSize = adWindowSize;}

    //getter
    unsigned int getNextSeqNumber() { return nextSeqNumber; }
    unsigned int getAdWindowSize() { return adWindowSize; }
    unsigned char getCheckSum() { 
        return ACK + (((this->nextSeqNumber) >> 24) & 0xff) + (((this->nextSeqNumber) >> 16) & 0xff) + (((this->nextSeqNumber) >> 8) & 0xff)
        + (this->nextSeqNumber & 0xff) + this->adWindowSize; 
    }
    unsigned int getFrameNumber(){ return  7;}
};

#endif