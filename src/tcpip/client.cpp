/*!
 * \file client.cpp
 *
 * \brief This represents client node
 *	  - Receives query from user
 *	  - Passes query to framework
 *	  - Receives results from the root node	  
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Wed Jul  6 23:36:49 CDT 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

#include "tcpconnector.h"
#include "tcpacceptor.h"

#include "easylogging++.h"

//Definig wildcard values
#define T 'T'
#define I 'I'
#define B 'B'

INITIALIZE_EASYLOGGINGPP

std::string send_query( std::string message, std::string rootnode, int portNum){
	TCPConnector* connector = new TCPConnector();
	TCPStream* stream = connector->connect( rootnode.c_str(), portNum);
	if (stream) {
		boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::string uuid_string = boost::uuids::to_string(uuid);

		stream->send(message.c_str(), message.size());
		LOG(INFO) << "Message with UUID "<< uuid_string << " sent successfully";
		LOG(INFO) << "Total data sent with message "<< uuid_string << " is " << message.size() << "Bytes";
		delete stream;
		return uuid_string;
	}
	else{
		LOG(ERROR) << "Not able to connect with the rootnode " << rootnode;
		return "";
	}
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Please provide some arguments" << std::endl;
		std::cout << "Use -h/--help for reference" << std::endl;
		exit(EXIT_FAILURE);
	}


	char wildcards;       // Currently supporting three file formats ==> Text, Images and Binary files
	std::string query_text = "";
	std::string rootnode = "";
	std::string filename = "";
	int portNum = 3033;	

	int i, j;
	//Command line parsing
	for (i=1; i< argc; i=i+2) 
	{
		if(0 == strcmp(argv[i], "--wildcards"))
		{
			if(0 == strcmp(argv[i+1], "I"))
			{
				wildcards = I;
			}
			else if(0 == strcmp(argv[i+1], "T"))
			{
				wildcards = T;	
			}
			else if(0 == strcmp(argv[i+1], "B"))
			{
				wildcards = B;
			}
			else
			{
				wildcards = T;
			}
			//break;
		}
		else if(0 == strcmp(argv[i], "--query"))
		{	
			for (j=i+1; j< argc; j=j+1)
			{
				query_text += argv[j];
				query_text += " ";
			}
			break;
		}
		else if(0 == strcmp(argv[i], "--filename"))
		{
			filename = argv[i+1];
		}
		else if(0 == strcmp(argv[i], "--rootnode"))
		{
			rootnode = argv[i+1];
		}
		else if(0 == strcmp(argv[i], "--port"))
		{
			portNum = atoi(argv[i+1]);
		}
		else if(0 == strcmp(argv[i], "--help") | 0 == strcmp(argv[i], "-h"))
		{
			std::cout << "Use of search client:" << std::endl;
			std::cout << "\t./binClient" << std::endl;
			std::cout << "\t\t --wildcards I/T/B" << std::endl;
			std::cout << "\t\t --filename query.txt" << std::endl;
			std::cout << "\t\t --rootnode ec2.fulldomain.name.amazon.com" << std::endl;
			std::cout << "\t\t --port 3033" << std::endl;
			std::cout << "\t\t --query Hippopotamus in Africa (This has to be last argument)" << std::endl;
			exit(EXIT_SUCCESS);
		}
		else
		{
			exit(EXIT_FAILURE);
			std::cout << "Use of search client:" << std::endl;
			std::cout << "\t./binClient" << std::endl;
			std::cout << "\t\t --wildcards I/T/B" << std::endl;
			std::cout << "\t\t --filename query.txt" << std::endl;
			std::cout << "\t\t --rootnode ec2.fulldomain.name.amazon.com" << std::endl;
			std::cout << "\t\t --port 3033" << std::endl;
			std::cout << "\t\t --query Hippopotamus in Africa (This has to be last argument" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	
	std::string uuid = "";
	if(rootnode.empty()){
		LOG(INFO) << "Rootnode not provided; connecting to localhost";
		rootnode = "localhost";
	}
	if(!filename.empty()){
		LOG(INFO) << "Reading query from file "<< filename;	
		std::ifstream file(filename);
		std::string message((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());	
		uuid = send_query( message, rootnode, portNum);
	}
	else if(query_text.empty()){
		std::string line;
		std::cout << "Enter your Search Term: ";
		getline(std::cin, line);
		uuid = send_query(line, rootnode, portNum);
	}
	else{
		uuid = send_query( query_text, rootnode, portNum);	
	}
	if(!uuid.empty()){
		LOG(INFO) << "Waiting for results for " << uuid;
		TCPAcceptor* acceptor = NULL;
		acceptor = new TCPAcceptor(3035);
		std::string received;
		if (acceptor->start() == 0) {
			TCPStream* stream = acceptor->accept();
			if (stream != NULL) {
				ssize_t len;
				char line[256];
				if ((len = stream->receive(line, sizeof(line))) > 0) {
					line[len] = 0;
					received = string(line);
					LOG(INFO) << "Received results for query " << uuid;
				}
			}
			delete stream;
		}
	}
	exit(0);
}
