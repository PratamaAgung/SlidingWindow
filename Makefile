all : build

build: sendfile.cpp receiver.cpp
	g++ sendfile.cpp -o bin/sendfile -std=c++11 -lpthread
	g++ receiver.cpp -o bin/receiver -std=c++11 -lpthread

run-sendfile:
	./bin/sendfile message.txt 5 100 10.5.25.66 9876

run-receiver:
	./bin/receiver