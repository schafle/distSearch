/*!
 * \file Client.h
 *
 * \brief This class defines the attributes and methods represented by a Client
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Wed Jun 15 01:11:31 CDT 2016
 */

#include <pthread.h>

#include"node.h"

#define MAX_CHILDREN 3

class Client : public Node{
	public:
		/* This is the port used when the node is being a client */
		int client_port;

		/* Ports used for client's children */
		int port_array[MAX_CHILDREN];

		/* Constructor */
		Client(std::string, int); 
		
		/* Start the client */
		void start_client();

		/* Stop the client */
		void stop_client();

};
