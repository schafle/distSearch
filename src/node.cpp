
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

#include "node.h"

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

	else cout << "Unable to open file";

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
		std::string child=IPs[childIndex];
		if(childIndex<IPs.size()){
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

/* Get message from Parent */
std::string Node::get_message_from_parent(std::string ParentHostName, int PortNumber){
	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = NULL;
	acceptor = new TCPAcceptor( 8012, ParentHostName.c_str());
	char line[256];
	if (acceptor->start() == 0) {
		stream = acceptor->accept();
		if (stream != NULL) {
			ssize_t len;
			while ((len = stream->receive(line, sizeof(line))) > 0) {
				line[len] = 0;
			}
			std::cout << "Got the message from parent: " <<line << std::endl;
		}
	}

	delete stream;
	return 0;	
}

/* Send message to Children */
int Node::send_message_to_children(std::string childHostName, int PortNumber, std::string message){
	/* Establish the connection */
	std::cout << "Sending query to "<< childHostName << std::endl;
	TCPConnector* connector = new TCPConnector();
	TCPStream* stream = connector->connect(childHostName.c_str(),PortNumber);
	if (stream) {
		stream->send(message.c_str(), message.size());
		std::cout << "sent the query to " << childHostName << std::endl;
		return 1;
	}

	delete stream;
	return 0;
}

/* Wait for children to send message */
std::string Node::receive_message_from_children(std::string childHostName, int PortNumber){
	TCPStream* stream = NULL;
	TCPAcceptor* acceptor = NULL;
	acceptor = new TCPAcceptor( 8013, childHostName.c_str());
	char line[1000];
	if (acceptor->start() == 0) {
		stream = acceptor->accept();
		if (stream != NULL) {
			ssize_t len;
			while ((len = stream->receive(line, sizeof(line))) > 0) {
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
	std::cout << "Sending answer to "<< ParentHostName << std::endl;
	TCPConnector* connector = new TCPConnector();
	//TCPStream* stream = connector->connect(ParentHostName.c_str(),PortNumber);
	TCPStream* stream = connector->connect(ParentHostName.c_str(), 8013);
	if (stream) {
		stream->send(message.c_str(), message.size());
		std::cout << "sent the reply to " << ParentHostName << std::endl;
		return 1;
	}
	delete stream;
	return 0;
}


std::string Node::get_input(int sock)
{
	//Reads incoming requests
	int n;
	char buffer[256];

	bzero(buffer,256);
	n = read(sock,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	std::string str= std::string(buffer);
	//int separator  = str.find(";");
	//int initialNodeIndex = std::atoi(str.substr(0, separator).c_str());
	//std::string query = str.substr(separator, sizeof(str));
	//queryChildren(str, initialNodeIndex);

	return str;
}

/* 
   void queryChildren(std::string query, int initialNodeIndex)
   {
//Gets the current node and list of nodes from file
//Could make improvement by not reading the entire list of nodes
std::ifstream infile;
std::string line;
infile.open("filename.txt");
std::getline(infile,line);
int curNodeIndex = std::atoi(line.c_str());
std::getline(infile,line);
int size = std::atoi(line.c_str());
std::string *A = new std::string[size];
int i=0;
while(std::getline(infile,line)){
A[i]=line;
i++;
}
int numofChildren=0;
int adjustedIndex=(curNodeIndex-initialNodeIndex+size)%size;

std::cout << "adjustedIndex: " << adjustedIndex << std::endl;

//Sends messages to the children
for(int i=0;i<numOfBranches;i++){
int childIndex=(numOfBranches*adjustedIndex+1+i);
if(childIndex<size){
std::string child=A[(childIndex+initialNodeIndex)%size];
std::cout << "Sending to node: " <<(childIndex+initialNodeIndex)%size<< std::endl;
sendMessage(query,child,SENDPORT);
numofChildren++;
}
else{
break;
}
}

std::string parentIP = A[((adjustedIndex-1)/numOfBranches+initialNodeIndex)%size];

std::cout << "parentIP: " << parentIP << std::endl;
//Opens new socket to wait for children to return
int sockfd, newsockfd;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) error("ERROR opening socket");
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(RECEIVEPORT);
int enable = 1;
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
error("setsockopt(SO_REUSEADDR) failed");
if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
listen(sockfd,5);
clilen = sizeof(cli_addr);

std::string returnMessage = "";
int recievedMessages=0;

//Waits for all children to return and concats the return messages
while(recievedMessages<numofChildren){
newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, 
&clilen);
if (newsockfd < 0) error("ERROR on accept");
bzero(buffer,256);
n = read(newsockfd,buffer,255);
if (n < 0) error("ERROR reading from socket");

char temp[255];
memset(temp, '\0', 256);
strncpy(temp, buffer, 255);
std::string str= "";
str.assign(temp);

returnMessage = returnMessage + str;
recievedMessages++;
}

//Processes all recieved messages
returnMessage = returnMessage + std::to_string(curNodeIndex);

//Return the results if node is the starting node, else send to parent
if(initialNodeIndex==curNodeIndex){
	std::cout << "Finished: " << returnMessage << std::endl;
}
else{
	sendMessage(returnMessage, parentIP, RECEIVEPORT);
}

close(newsockfd);
close(sockfd);
}
*/
/*
   Sends Message
 */
/* bool sendMessage(std::string message, std::string nodeIP, int portno)
   {
   int sockfd, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   char buffer[256];

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
   error("ERROR opening socket");
   const char *cstr=nodeIP.c_str();
   server = gethostbyname(cstr);
   if (server == NULL) {
   fprintf(stderr,"ERROR, no such host\n");
   exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
   error("ERROR connecting");
   bzero(buffer,256);
   message.copy(buffer,message.size(),0);

   n = write(sockfd,buffer,strlen(buffer));
   if (n < 0) 
   error("ERROR writing to socket");
   bzero(buffer,256);

   close(sockfd);
   return 0;
   } */
