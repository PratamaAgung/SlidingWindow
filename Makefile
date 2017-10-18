all : build

build: sendfile.cpp receiver.cpp
	g++ sendfile.cpp -o bin/sendfile -std=c++11 -lpthread
	g++ receiver.cpp -o bin/receiver -std=c++11 -lpthread

run-sendfile:
	./bin/sendfile message.txt 5 100 127.0.0.1 9876

run-receiver:
	./bin/receiver 9876 5