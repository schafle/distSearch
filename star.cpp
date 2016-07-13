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
#include <cstdlib>
#include <ctime>
#include <thread>
#include <future>

#include "easylogging++.h"

//#include "query.h"
#include "node.h"
#include "query.h"

#define ELPP_THREAD_SAFE 
 
INITIALIZE_EASYLOGGINGPP

#define numOfBranches 2
#define SENDPORT 3033
#define RECEIVEPORT 3034

void send_messages(std::vector<std::string> children, int portNum, Node currentNode, std::string message){

	for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
		if(!currentNode.send_message(*it, portNum, message ) ){  /* If Unsuccessful print the message */
			LOG(ERROR) << "ERROR while sending query to children";
		}
		LOG(INFO) << "Sent the message to " <<  *it;
	}
	LOG(INFO) << "Done sending query to all the children; waiting for children to send the message back";
}


std::string asyncQuery( std::string index_location, std::string query, int max_pages, std::string uuid, std::string HostName)
{ 
    return run_query( index_location, query, max_pages, uuid, HostName);
}

int main(int argc, char* argv[]){
	if (argc < 2) {
		std::cout << "Please provide some arguments" << std::endl;
		std::cout << "Use -h/--help for reference" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string index_location = ".";   		// Set to current working directory if not passed as input
	std::string HostName;		   		// Must Have
	std::string NodeDetailsFile = "filename.txt";	// Good to have
	int posNum;		   	    		// Must have
	int portNumber = 3033;		   		// Should not be set in most cases; 
	// 3033 is our default sending port
	// and 3034 is our default receiving port
	int i;
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
			portNumber = atoi(argv[i+1]);
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
	Node currentNode = Node( HostName, portNumber, posNum, NodeDetailsFile);

	LOG(INFO) << "Listening for input on port: "<< portNumber;

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
	std::string uuid;
	if(posNum==0){
		
		start = std::clock();

		// create a new thread to send all the messages
		std::thread send (send_messages, children, 3033, currentNode, received_string);
		send.join();
		LOG(INFO) << "Done sending query to all the children; waiting for children to send the message back";
		LOG(INFO) << "Starting async search in index";
		uuid = received_string.substr(0,36);
		
		auto future = std::async( asyncQuery, index_location, received_string.substr(36), 10, uuid, HostName); //Starting async query
		
		std::string resultsFromChildren = currentNode.listenForMultipleReplies(3034, children.size());

		 //LOG(INFO) << uuid << " Received results from children " << resultsFromChildren; 
		 std::string searchResults = future.get();	

		 LOG(INFO) << uuid << " Sending results back to parent " << parent;

		 duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		 //currentNode.send_message("localhost", 3035, std::to_string(duration));
		 currentNode.send_message("localhost", 3035, "Received all messages in " + std::to_string(duration) + " seconds" + "\n"  + searchResults + resultsFromChildren);
		 LOG(INFO) << "Received all messages in: "<< duration << " seconds" ;
	}

	/* Else open a connection and send message back to parent */
	else{
		uuid = received_string.substr(0,36);
		std::string search_results = run_query( index_location, received_string.substr(36), 10, uuid, HostName);  //10 is the default number of hits
		LOG(INFO) << uuid << " Done searching!!";
		LOG(INFO) << uuid << " Sending results back to parent " << parent;
		currentNode.send_message( parent, 3034, search_results);
	}

}
