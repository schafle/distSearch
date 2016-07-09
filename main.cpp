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

#define ELPP_THREAD_SAFE 

INITIALIZE_EASYLOGGINGPP

void send_messages(std::vector<std::string> children, int portNum, Node currentNode, std::string message){

	for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
		if(!currentNode.send_message(*it, portNum, message ) ){  /* If Unsuccessful print the message */
			LOG(ERROR) << "ERROR while sending query to children";
		}
		LOG(INFO) << "Sent the message to " <<  *it;
	}
}

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
	int numOfBranches = 2;
	int startNode = 0;
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
		else if(0 == strcmp(argv[i], "--numbranch"))
		{
			numOfBranches = atoi(argv[i+1]);
			if (numOfBranches < 2){
				std::cerr << "Num of Branches in tree must be greater than 2" << std::endl;
			}
		}
		else if(0 == strcmp(argv[i], "--port"))
		{
			portNum = atoi(argv[i+1]);
		}
		else if(0 == strcmp(argv[i], "--startNode"))
		{
			startNode = atoi(argv[i+1]);
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

	std::vector<std::string>  children = currentNode.get_children(startNode, numOfBranches);

	int numOfChildren = children.size();
	//std::cout << "I have "<< children.size() << " children." << std::endl;
	std::clock_t start;
	double duration;
	if(posNum==startNode){
		start = std::clock();
	}

	// create a new thread to send all the messages
	std::thread send (send_messages, children, 3033, currentNode, received_string);
	send.join();
	
	LOG(INFO) << "Done sending query to all the children; waiting for children to send the message back";
	
	int received_messages_count = 0;

	/* If leaf send its name to parent and thats it*/
	if(currentNode.am_i_leaf(startNode, numOfBranches)){
		LOG(INFO) << "Sending it back to parent " << currentNode.get_parent(startNode, numOfBranches) << std::endl;
		currentNode.send_message(currentNode.get_parent(startNode, numOfBranches), 3034, received_string);
	}
	/* else if root send back to client */	
	else if(posNum == startNode){

		currentNode.listenForMultipleReplies(3034, numOfChildren);
		duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		currentNode.send_message("localhost", 3035, std::to_string(duration));
		LOG(INFO) << "Received all messages in: "<< duration << " seconds" ;
	}	
	/* Else open a connection for collecting result from child and then send back to parent */
	else{
		currentNode.listenForMultipleReplies(3034, numOfChildren);

		// For current implementation we are sending back the string we got from the parent
		// When query collection is implemented we will send back the results for each query
		currentNode.send_message(currentNode.get_parent(startNode, numOfBranches), 3034, received_string); 
	}
	/* Make query irrespective of anything on the current node */
}
