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

void send_messages(std::vector<std::string> children, int portNum, Node currentNode, std::string message){

	for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
		if(!currentNode.send_message(*it, portNum, message ) ){  /* If Unsuccessful print the message */
			error("ERROR while sending query to children");
		}
		std::cout << "Sent the message to " <<  *it << std::endl;
	}
}


void receive_messages(std::vector<std::string> children, int portNum, Node currentNode){

	for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {

		if(!currentNode.get_message(*it, 3034) ){ 
			error("ERROR while receiving reply from children");
		}
		std::cout << "Received reply from " <<  *it << std::endl;
	}
}

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

	std::vector<std::string> children = currentNode.get_list_of_all_nodes(NodeDetailsFile); //currentNode.get_children(0, 2);
	std::string parent = children[0];
	children.erase (children.begin(), children.begin()+1);
	std::string received_messages;
	int received_messages_count = 0;
	pthread_t threadA[2];
	std::clock_t start;
	double duration;
	if(posNum==0){
		start = std::clock();
		/* Iterate ovr all the children and Forward query */
		//for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
		//	if(!currentNode.send_message(*it, 3033, received_string ) ){  /* If Unsuccessful print the message */
		//		error("ERROR while sending query to children");
		//	}
		//	std::cout << "Sent the message to " <<  *it << std::endl;
		//}

		// create a new thread to start listening for new messages
		// std::thread receive(receive_messages, children, 3034, currentNode);
		// create a new thread to send all the messages
		 std::thread send (send_messages, children, 3033, currentNode, received_string);
		 std::cout << "Waiting for children to send the message back" << std::endl;
		 
		// send.join();
		// receive.join();

		//while(received_messages_count != children.size()){
		//received_string = currentNode.get_message(children[received_messages_count],3034);
		//received_string = currentNode.listenOnTheReceivePort(3034);
		//currentNode.listenForMultipleReplies(3034, numOfChildren)
		// received_messages_count += 1;
		// }

		//This for loop represents that the root node is making connection serially to each node in cluster and 
		//getting data from them. This can be done in two ways. 1. The serial way --> The for looop
		//2. Parallel way --> create a thread to get data from each new connection, like subparents in tree
		//For now we are going ahead with 1 since creating multiple threads will/can cause problems if not handled properly.
		//Also need to find out how is it being done in the systems implementing star topology 
		/*for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {

		  if(!currentNode.get_message(*it, 3034) ){ 
		  error("ERROR while receiving reply from children");
		  }
		  std::cout << "Receive reply from " <<  *it << std::endl;
		  } */
		currentNode.listenForMultipleReplies(3034, children.size()); /* The parallel way */
		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		received_messages = "Received all messages in "+ std::to_string(duration);
		currentNode.send_message("localhost", 3035,received_messages);
		std::cout<<"Received all messages in: "<< duration << "seconds" << std::endl;
	}
	/* Else open a connection and send message back to parent */
	else{
		std::cout << "Sending it back to parent "<< parent << std::endl;
		currentNode.send_message(parent, 3034, HostName);
	}

}
