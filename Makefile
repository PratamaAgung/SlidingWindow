all : build

build: 
	echo 'x'

run-sendfile:
	./sendfile data/message.txt 5 100 127.0.0.1 9876

run-receiver:
	./recvfile data/out.txt 5 100 9876