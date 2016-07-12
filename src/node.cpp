/*!
 * \file node.cpp
 *
 * \brief This class defines the attributes and methods represented by each node.
 * 	  Each node can either be a server or a client or both a server and a client 
 * 	  Depending on the scenario this class will be inherited by Server or Client class 
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Tue Jun 21 18:27:43 CDT 2016
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <netdb.h>
#include <iostream> 
#include <cstdlib>

#include "easylogging++.h"

#include "node.h"

#define HUNDMB 640001

/* Constructor */
Node::Node(std::string hostName, int portNumber, int PosNum, string filename) {
	hostName = hostName;
	portNumber = portNumber;
	available_nodes = get_list_of_all_nodes(filename);
	position_in_cluster = PosNum;
}

/* 
 * This function returns list of all nodes available in the cluster
 * Should be called only once
 */

std::vector<std::string> Node::get_list_of_all_nodes(std::string filename){
	std::vector<std::string> nodes;
	std::string line;
	ifstream myfile (filename.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			nodes.push_back(line);
		}
		myfile.close();
	}

	else LOG(ERROR) << "Unable to open file "<< filename ;

	//std::vector<std::string> IPs{"ip0", "localhost", "ip2", "ip3", "ip4", "ip5", "ip6", "ip7"}; 
	return nodes;
}


/* Here goes the logic for get parent method */
std::string Node::get_parent(int initialNodeIndex, int numOfBranches){
	int curNodeIndex = position_in_cluster;
	std::vector<std::string> IPs = available_nodes;
	int adjustedIndex=(curNodeIndex-initialNodeIndex+IPs.size())%IPs.size();
	std::string parentIP = IPs[(adjustedIndex-1)/numOfBranches];
	return parentIP;

}

/* Here goes the logic for get children method */
std::vector<std::string> Node::get_children(int starting_node, int num_of_branches){

	int pos_of_current_node = position_in_cluster;
	std::vector<std::string> IPs = available_nodes;
	std::vector<std::string> children_IPs;
	int adjustedIndex=(pos_of_current_node - starting_node + IPs.size())%IPs.size();
	for(int i=0;i<num_of_branches;i++){
		int childIndex= num_of_branches*adjustedIndex+1+i;
		if(childIndex<IPs.size()){
			std::string child=IPs[childIndex];
			children_IPs.push_back(child);
		}
		else{
			break;
		}
	}
	return children_IPs;
}

bool Node::am_i_leaf(int starting_node, int num_of_branches){

	int pos_of_current_node = position_in_cluster;
	std::vector<std::string> IPs = available_nodes;
	int adjustedIndex=(pos_of_current_node - starting_node + IPs.size())%IPs.size();
	if(adjustedIndex*num_of_branches+1>IPs.size()-1){
		return true;
	}
	return false;
}


ssize_t readmultiple(int fd, char *ptr, size_t n)
{
	size_t nleft;
	ssize_t nread;

	nleft = n;
	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				/* Loop back and call read again. */
				nread = 0;
			else
				/* Some other error; can't handle. */
				return -1;
		} else if (nread == 0)
			/* EOF. */
			break;

		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}

void *task1 (int connFd)
{
	LOG(INFO) << "Thread created with ID " << pthread_self();
	char test[HUNDMB];
	bzero(test, HUNDMB);
	bool loop = false;
	bzero(test, HUNDMB);


	size_t nleft;
	ssize_t nread;
	char *ptr = test;
	nleft = HUNDMB;
	while (nleft > 0) {
		if ((nread = read(connFd, test, nleft)) < 0) {
			if (errno == EINTR)
				/* Loop back and call read again. */
				nread = 0;
			else
				/* Some other error; can't handle. */
				std::cout << "Some other error; can't handle" << std::endl; //return; // -1;
		} else if (nread == 0)
			/* EOF. */
			break;

		nleft -= nread;
		ptr += nread;
	}
	//return n - nleft;
	//read(connFd, test, 1048576);

	string received (test);
	LOG(INFO) << "Results for Query " << received.substr(0,36) << " is processed; size of the result is "<< received.size() << " Bytes";        

	LOG(INFO) << "Terminating thread " << pthread_self() << " and closing the connection";
	close(connFd);
}

void multiple(TCPStream* stream){
	std::string received;
	ssize_t len;
	char* line = new char[HUNDMB];
	LOG(INFO) << "Thread created with ID: " << std::this_thread::get_id();
	if ((len = stream->receive(line, HUNDMB)) > 0) {
		received = string(line);
		LOG(INFO) << "Results for Query " << received.substr(0,36) << " is processed; size of the result is "<< received.size() << " Bytes";        
		LOG(INFO) << "Terminating thread " << std::this_thread::get_id() << " and closing the connection";
		delete stream;
	}
}
void Node::listenForMultipleReplies(int portNum, int numOfChildren){
	std::thread threadA[numOfChildren];
	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = new TCPAcceptor(portNum);
	if (acceptor->start() == 0) {
		for (int i = 0; i < numOfChildren; ++i){
			LOG(INFO) << "Listening for input on port: " << portNum;
			stream = acceptor->accept();
			if (stream != NULL) {
				LOG(INFO) << "Connection Successful";
				threadA[i] = std::thread(multiple, stream);
			}
		}
	}

	for(int i = 0; i < numOfChildren; i++)
	{
		threadA[i].join();
		LOG(INFO) << "Joined "<< i+1 << "threads successfully!!"; 
	}
}

std::string Node::listenOnTheReceivePort(int portNum){

	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = NULL;
	acceptor = new TCPAcceptor(portNum);
	std::string received;
	if (acceptor->start() == 0) {
		stream = acceptor->accept();
		if (stream != NULL) {
			ssize_t len;
			char* line = new char[HUNDMB];
			if ((len = stream->receive(line, HUNDMB)) > 0) {
				received = string(line);
				LOG(INFO) << "Query " << received.substr(0,36) << " received; size of the message is "<< received.size() << " Bytes";
			}
			delete stream;
		}
	}
	delete acceptor;
	return received;
}

/* Get message from Parent */
bool Node::get_message(std::string HostName, int PortNumber){
	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = NULL;
	std::string received;
	acceptor = new TCPAcceptor( 3034, HostName.c_str());
	char* line = new char[HUNDMB];
	if (acceptor->start() == 0) {
		stream = acceptor->accept();
		if (stream != NULL) {
			ssize_t len;
			while ((len = stream->receive(line, sizeof(line))) > 0) {
				line[len] = 0;
				LOG(INFO) << "Query " << received.substr(0,36) << " received; size of the message is "<< received.size();
			}
			//std::cout << "Got the message from parent: " <<line << std::endl;
		}
	}

	delete stream;
	delete acceptor;

	return true;	
}

/* Send message to anyone */
bool Node::send_message(std::string HostName, int PortNumber, std::string message){
	/* Establish the connection */
	TCPConnector* connector = new TCPConnector();
	TCPStream* stream = connector->connect(HostName.c_str(),PortNumber);
	if (stream) {
		stream->send(message.c_str(), message.size());
		delete stream;
		return true;
	}
	else{
		LOG(ERROR) << "Could not create connection with "<< HostName;
	}	
	delete stream;
	return false; 

}

/* Wait for children to send message */
std::string Node::receive_message_from_children(std::string childHostName, int PortNumber){
	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = NULL;
	acceptor = new TCPAcceptor( 8013, childHostName.c_str());
	char* line = new char[HUNDMB];
	if (acceptor->start() == 0) {
		stream = acceptor->accept();
		if (stream != NULL) {
			ssize_t len;
			while ((len = stream->receive(line, HUNDMB)) > 0) {
				line[len] = 0;
			}
		}
	}
	delete stream;
	return line;
}

/* Send message to parent */
int Node::send_message_to_parent(std::string ParentHostName, int PortNumber, std::string message){
	/* Establish the connection */
	LOG(INFO) << "Sending answer to parent "<< ParentHostName;
	TCPConnector* connector = new TCPConnector();
	//TCPStream* stream = connector->connect(ParentHostName.c_str(),PortNumber);
	TCPStream* stream = connector->connect(ParentHostName.c_str(), 8013);
	if (stream) {
		stream->send(message.c_str(), message.size());
		LOG(INFO) << "sent the reply to parent " << ParentHostName;
		return 1;
	}
	delete stream;
	return 0;
}


std::string Node::get_input(int sock)
{
	//Reads incoming requests
	int n;
	char buffer[HUNDMB];

	bzero(buffer, HUNDMB);
	n = read(sock,buffer, HUNDMB);
	if (n < 0) error("ERROR reading from socket");
	std::string str= std::string(buffer);
	return str;
}
