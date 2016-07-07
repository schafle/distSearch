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
#include <ctime>

#include "easylogging++.h"

#include "node.h"

#define numOfBranches 2
#define SENDPORT 3033
#define RECEIVEPORT 3034

#define ELPP_THREAD_SAFE 

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]){

	if (argc < 2) {
		std::cout << "Please provide some arguments" << std::endl;
		std::cout << "Use -h/--help for reference" << std::endl;
		exit(EXIT_FAILURE);
	}
	//[indexDirectory] [positionInCluster] [HostName] [FileName

	std::string index_location = ".";   		// Set to current working directory if not passed as input
	std::string HostName;		   		// Must Have
	std::string NodeDetailsFile = "filename.txt";	// Good to have
	int posNum;		   	    		// Must have
	int portNum = 3033;		   		// Should not be set in most cases; 
	// 3033 is our default sending port
	// and 3034 is our default receiving port

	int i, j;
	//Command line parsing
	for (i=1; i< argc; i=i+2) 
	{
		if(0 == strcmp(argv[i], "--indexdir"))
		{
			index_location = argv[i+1];
		}
		else if(0 == strcmp(argv[i], "--hostname"))
		{	
			HostName = argv[i+1];	
		}
		else if(0 == strcmp(argv[i], "--position"))
		{
			posNum = atoi(argv[i+1]);
		}
		else if(0 == strcmp(argv[i], "--port"))
		{
			portNum = atoi(argv[i+1]);
		}
		else if(0 == strcmp(argv[i], "--filename"))
		{
			NodeDetailsFile = argv[i+1];
		}
		else if(0 == strcmp(argv[i], "--help") | 0 == strcmp(argv[i], "-h"))
		{
			std::cout << "Use of search server:" << std::endl;
			std::cout << "\t./binServer" << std::endl;
			std::cout << "\t\t --indexdir ~/Index/location" << std::endl;
			std::cout << "\t\t --hostname ec2.fulldomain.name.amazon.com" << std::endl;
			std::cout << "\t\t --filename filename.txt" << std::endl;
			std::cout << "\t\t --position 3" << std::endl;
			std::cout << "\t\t --port 3033" << std::endl;
			exit(EXIT_SUCCESS);
		}
		else
		{
			std::cout << "Use of search server:" << std::endl;
			std::cout << "\t./binServer" << std::endl;
			std::cout << "\t\t --indexdir ~/Index/location" << std::endl;
			std::cout << "\t\t --hostname ec2.fulldomain.name.amazon.com" << std::endl;
			std::cout << "\t\t --filename filename.txt" << std::endl;
			std::cout << "\t\t --position 3" << std::endl;
			std::cout << "\t\t --port 3033" << std::endl;
			exit(EXIT_FAILURE);
		}
	}


	/* Create a node object */
	Node currentNode = Node( HostName, portNum, posNum, NodeDetailsFile);

	LOG(INFO) << "Listening for input on port: "<< portNum;

	/* Start listening on receivePort */
	std::string received_string = currentNode.listenOnTheReceivePort(3033); 


	/* Get all the children */

	std::vector<std::string>  children = currentNode.get_children(0, 2);

	int numOfChildren = children.size();
	//std::cout << "I have "<< children.size() << " children." << std::endl;
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
		LOG(INFO) << "Sent the message to child" <<  *it ;
	}
	std::string received_messages;
	int received_messages_count = 0;

	/* If leaf send its name to parent and thats it*/
	if(currentNode.am_i_leaf(0,2)){
		LOG(INFO) << "Sending it back to parent " << currentNode.get_parent(0,2) << std::endl;
		currentNode.send_message(currentNode.get_parent(0,2), 3034, HostName);
	}
	/* else if root send back to client */	
	else if(posNum == 0){

		currentNode.listenForMultipleReplies(3034, numOfChildren);
		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		received_messages = "Received all messages in "+ std::to_string(duration);
		currentNode.send_message("localhost", 3035,received_messages);
		LOG(INFO) << "Received all messages in: "<< duration << " seconds" ;
	}	
	/* Else open a connection for collecting result from child and then send back to parent */
	else{
		currentNode.listenForMultipleReplies(3034, numOfChildren);
		received_messages = HostName;
		currentNode.send_message(currentNode.get_parent(0,2), 3034, received_messages);
	}
	/* Make query irrespective of anything on the current node */
}
