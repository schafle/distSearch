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

#include<iostream>
#include<vector>
#include<string>


#define MAX_CHILD 2  /* Maximum number of children a node can have */

class Node{
	public:
		/* This represents the unique id representing the name of the node; It can also be an IP of the node */
		/* ToDo: Reading the port number from command line argument or a configuration file */
		std::string hostname;	 

		/* This is the port used when the node is being a server */
		/* ToDo: Reading the port number from command line argument or a configuration file */
		int port = 8012;   
		
		/* All the number of nodes available in the cluster
		 * We are storing it as a part of the node to avoid reading it from file everytime it is needed */
		std::vector<std::string>available_nodes;
	
		/* position/index of the node in a cluster */
	 	int position_in_cluster;

		/* This is the port used when the node is being a server */
		//int client_port = 8013;
		
		/* Constructor */
                Node(std::string hostname, int node, int PosNum);

		/* List of all the child of this node */
		std::string child_array[MAX_CHILD];

		/* Get list of all the nodes in the cluster */
		std::vector<std::string> get_list_of_all_nodes(); 
		
		/* Return parent of this node */
		std::string get_parent(int initialNodeIndex, int numOfBranches);

		/* Return vector of children of this node */
		std::vector<std::string>get_children(int starting_node, int num_of_branches);

		/* Returns true if current Node is a leaf node*/
		bool am_i_leaf(int starting_node, int num_of_branches);

		/* Get message from Parent */
		int get_message_from_parent(std::string ParentHostName, int PortNumber);
		
		/* Send message to Children */
		int send_message_to_children(std::string childName, int PortNumber);	

		/* Wait for children to send message */
		int receive_message_from_children(std::string childName, int PortNumber);

		/* Send message to parent */
		int send_message_to_parent(std::string ParentHostName, int PortNumber);

};
