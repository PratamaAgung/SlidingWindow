all : build

build: src/sendfile.cpp src/receiver.cpp
	g++ src/sendfile.cpp -o sendfile -std=c++11 -lpthread
	g++ src/receiver.cpp -o recvfile -std=c++11 -lpthread

run-sendfile:
	./sendfile data/message.txt 5 100 127.0.0.1 9876

run-receiver:
	./recvfile data/out.txt 5 100 9876