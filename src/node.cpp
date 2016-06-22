
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

#include "../include/node.h"

/* Constructor */
Node::Node(std::string hostName, int portNumber, int PosNum) {
	hostName = hostName;
	portNumber = portNumber;
	available_nodes = get_list_of_all_nodes();
	position_in_cluster = PosNum;
}


std::vector<std::string> Node::get_list_of_all_nodes(){

	std::vector<std::string> IPs{"ip0", "ip1", "ip2", "ip3", "ip4", "ip5", "ip6", "ip7"}; 
	return IPs;
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
int Node::get_message_from_parent(std::string ParentHostName, int PortNumber){
return 0;	
}

/* Send message to Children */
int Node::send_message_to_children(std::string childHostName, int PortNumber){
return 0;
}

/* Wait for children to send message */
int Node::receive_message_from_children(std::string childHostName, int PortNumber){
return 0;
}

/* Send message to parent */
int Node::send_message_to_parent(std::string ParentHostName, int PortNumber){
return 0;
}
