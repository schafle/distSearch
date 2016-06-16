/*!
 * \file client.cpp
 *
 * \brief This defines all the methods of the Node
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Wed Jun 15 00:11:03 CDT 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include"../include/client.h"

Client::Client(std::string node_id, int port) {
	this->id    	 = node_id;
	this->client_port = port;
}

/* Printing the error message and exit */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void Client::start_client(){
	std::cout<<"client started"<<std::endl;
}


void Client::stop_client(){
	std::cout<<"Stopping the server"<<std::endl;
}

int main(){
	std::cout << "hello world" << std::endl;
	Client serv("localhost", 2013);
	serv.start_client();
	serv.stop_client();
	return 0;
}
