/*!
 * \file node.h
 *
 * \brief This class defines the attributes and methods represented by each node.
 * 	  Each node can either be a server or a client or both a server and a client 
 * 	  Depending on the scenario this class will be inherited by Server or Client class 
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Tue Jun 14 23:41:30 CDT 2016
 */

#define MAX_CHILD 3

#include<iostream>
class Node{
	public:
		/* This represents the unique id representing the name of the node; It can also be an IP of the node */
		std::string id;	 

		/* This is the port used when the node is being a server */
		int server_port;

		/* This is the port used when the node is being a server */
		int port_array[MAX_CHILD];

		/* Start the server */
		void start_server(std::string);

		/* Start the client */
		void start_client(std::string);

	private:
		/* List of all the child of this node */
		std::string child_array[MAX_CHILD];

		/* Parent of this node */
		std::string parent_id;


};
