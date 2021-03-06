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

#define HUNDMB 640001

INITIALIZE_EASYLOGGINGPP

std::string send_query( std::string message, std::string rootnode, int portNum){
	TCPConnector* connector = new TCPConnector();
	TCPStream* stream = connector->connect( rootnode.c_str(), portNum);
	if (stream) {
		boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::string uuid_string = boost::uuids::to_string(uuid);
		message = uuid_string + message;
		stream->send(message.c_str(), message.size());
		LOG(INFO) << "Message with UUID "<< uuid_string << " sent successfully";
		LOG(INFO) << "Total data sent with message "<< uuid_string << " is " << message.size() << " Bytes";
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
	std::string rootnode = "localhost";
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
	if( rootnode == "localhost"){
		LOG(INFO) << "Rootnode not provided; connecting to localhost";
	}
	if(!filename.empty()){
		LOG(INFO) << "Reading query from file "<< filename;	
		std::ifstream file(filename);
		// The following is supposedly in-efficient way of reading file to string.
		// See http://stackoverflow.com/a/2602060/2914543
		//	std::string message((std::istreambuf_iterator<char>(file)),
		//		std::istreambuf_iterator<char>());	
		
		// Following is the better way of doing it as per http://stackoverflow.com/a/2602258/2914543
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string message = buffer.str();
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
				char* line = new char[HUNDMB];
				std::string delimiter = "___";
				size_t pos = 0;
				std::string token;
				if ((len = stream->receive(line, HUNDMB)) > 0) {
					received = string(line);
					//std::cout << received << std::endl;	
					//LOG(INFO) << "Query " << uuid << " processed in " << 
					//LOG(INFO) << "Query " << uuid << received << " seconds";
				}
				while ((pos = received.find(delimiter)) != std::string::npos) {
				token = received.substr(0, pos);
				std::cout << token << std::endl;
				received.erase(0, pos + delimiter.length());
				}
			}
			delete stream;
		}
	}
	exit(0);
}
