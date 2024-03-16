FROM gcc:latest

COPY . /usr/src/server
WORKDIR /usr/src/server

RUN mkdir -p build/ && cd build/ && rm -rf * 
RUN cd build/ && g++ -I../include -o server ../src/server.cpp ../src/tcpserver.cpp

EXPOSE 8080