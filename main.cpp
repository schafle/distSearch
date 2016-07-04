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
#include <ctime>

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

	int portNumber = 3033;

	/* Create a node object */
	Node currentNode = Node( HostName, portNumber, posNum, NodeDetailsFile);

	std::cout << "Listening for input on port: "<< portNumber << std::endl;

	/* Start listening on receivePort */
	std::string received_string = currentNode.listenOnTheReceivePort(3033); 


	/* Get all the children */

	std::vector<std::string>  children = currentNode.get_children(0, 2);

	int numOfChildren = children.size();
	std::cout << "I have "<< children.size() << " children." << std::endl;
	std::clock_t start;
	double duration;
	if(posNum==0){
		start = std::clock();
	}
	/* Iterate ovr all the children and Forward query */
	for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {

		if(!currentNode.send_message(*it, 3033, received_string ) ){  /* If Unsuccessful print the message */
			error("ERROR while sending query to children");
		}
		std::cout << "Sent the message to " <<  *it << std::endl;
	}
	std::string received_messages;
	int received_messages_count = 0;

	/* If leaf send its name to parent and thats it*/
	if(currentNode.am_i_leaf(0,2)){
		std::cout << "Sending it back to parent " << currentNode.get_parent(0,2) << std::endl;
		currentNode.send_message(currentNode.get_parent(0,2), 3034, HostName);
	}
	/* else if root send back to client */	
	else if(posNum == 0){

		std::cout << "children.size()==>" << children.size() << std::endl;
	//	while(received_messages_count != children.size()){
			//received_string = currentNode.listenForMultipleReplies(3034);
			currentNode.listenForMultipleReplies(3034, numOfChildren);
		//	std::cout << "Received from " << received_string << std::endl;
	//		received_messages_count += 1;
			//received_messages += HostName;
	//	}
		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		received_messages = "Received all messages in "+ std::to_string(duration);
		currentNode.send_message("localhost", 3035,received_messages);
		std::cout<<"Received all messages in: "<< duration << "seconds\n";
	}	
	/* Else open a connection for collecting result from child and then send back to parent */
	else{
		//while( received_messages_count != children.size()){
			//received_string = currentNode.listenForMultipleReplies(3034);
			currentNode.listenForMultipleReplies(3034, numOfChildren);
		//	received_messages_count += 1;
			//std::cout << received_messages_count << " messages received" << std::endl;
		//}
		received_messages = HostName;
		currentNode.send_message(currentNode.get_parent(0,2), 3034, received_messages);
	}
	/* Make query irrespective of anything on the current node */
}
