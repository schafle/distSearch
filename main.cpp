/*!
 * \file main.cpp
 *
 * \brief This class defines the attributes and methods represented by each node.
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Sun Jun 26 15:45:24 CDT 2016
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <netdb.h>
#include <cstdlib>
//#include "query.h"
#include "node.h"

#define numOfBranches 2
#define SENDPORT 3033
#define RECEIVEPORT 3034

int main(int argc, char* argv[]){
	if(argc < 5){ 
		std::cout << "Insufficient arguments to run" << std::endl;
		std::cout << "./executable [indexDirectory] [positionInCluster] [HostName] [FileName]" << std::endl;
		exit (EXIT_FAILURE);
	}

	std::string index_location = argv[1];
	int posNum = atoi(argv[2]);
	std::string HostName = argv[3];
	std::string NodeDetailsFile = argv[4];

	int portNumber = 2006;

	int sockfd, newsockfd, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(SENDPORT);
	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		error("setsockopt(SO_REUSEADDR) failed");
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		/*	pid = fork();
			if (pid < 0)
			error("ERROR on fork");
			if (pid == 0)  {
			close(sockfd);
			std::cout << "incoming message" << std::endl;
			propogate(newsockfd);
			exit(0);
			}*/
		/* Create a node object */
		Node currentNode = Node( HostName, portNumber, posNum, NodeDetailsFile);

		/* Waiting for a query from a client or a parent node */
		std::cout << currentNode.get_input(newsockfd) << std::endl;
		close(newsockfd);
	}
	close(sockfd);
	//std::cout << currentNode.get_children(0,2)[0] << std::endl;
	/* Make query irrespective of anything on the current node */
	//int a = make_query(index_location);

}

