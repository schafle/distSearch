/*!
 * \file server.h
 *
 * \brief This class defines the attributes and methods represented by a server.
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Wed Jun 15 01:03:52 CDT 2016
 */

#include"node.h"

#define MAX_CHILD 3

class Server: public Node{
	public:
		/* This is the port used when the node is being a server */
		int server_port;

		/* This is the port used when the node is being a server */
		int port_array[MAX_CHILD];

		/* Constructor */
		Server(std::string, int); 

		/* Start the server */
		void start_server();

		/* Stop the server */
		void stop_server();

};
