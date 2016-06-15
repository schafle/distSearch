/*!
 * \file server.cpp
 *
 * \brief This defines all the methods of the Server
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Wed Jun 15 01:17:42 CDT 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include"../include/server.h"

Server::Server(std::string node_id, int port) {
	this->id    	 = node_id;
	this->server_port = port;
}

/* Printing the error message and exit */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void Server::start_server(){
	std::cout<<"server started"<<std::endl;
}


void Server::stop_server(){
	std::cout<<"Stopping the server"<<std::endl;
}

int main(){
	std::cout << "hello world" << std::endl;
	Server serv("localhost", 2013);
	serv.start_server();
	return 0;
}
