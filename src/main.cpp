#include<iostream>

#include "node.h"
#include <cstdlib>

using namespace std;

int main(int argc, char** argv){

	if(argc < 4){
		std::cout << "Insufficient arguments to run" << std::endl;
		std::cout << "./executable [positionInCluster] [HostName] [FileName]" << std::endl;
		exit (EXIT_FAILURE);
	}

	int NumPos = atoi(argv[1])-1; /*Get this from the command line argument */

	string HostName =argv[2];

	std::string filename = argv[3];

	std::cout << "Hello Guys! My Name is "<< HostName << " and I have index "<< NumPos << " in the cluser" << std::endl;
	string query = "What's your name?";
	Node n = Node( HostName, 8012, NumPos, filename); /* Make NumPos configurable with command line argument */

	vector<string> children;	
	/* See if it is a root node */
	if(NumPos == 0){
		/* Get the message from command line now
		 * In future this will be received from the search client */
		/* Forward query, receive answer, forward answer and repeat */
		while(true){
			/* Find children of the node first */
			children = n.get_children(0, 2); /* Starting Node 0 and number of branches 2 */

			/* Iterate ovr all the children and Forward query */
			for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
				n.send_message_to_children(*it, 8012, query);
			}

			std::vector<std::string> results_received; 
			/* Iterate ovr all the children and Collect results of the query */
			for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
				std::string temp_result = n.receive_message_from_children(*it, 8013);
				results_received.push_back(temp_result);
			}

			break; /* This is just for testing */
		}
	}
	/* else see if it is a leaf node */
	else if(n.am_i_leaf(0,2)){
		/* If this is a leaf node don't send anything forward */
		/* Keep listening, find answer, forward answer and repeat */
		while(1){
			std::cout << "I Am Leaf" << std::endl;
			n.send_message_to_parent(n.get_parent(NumPos,2), 8012, n.hostname);
		}
	}
	/* DO this if it neither a leaf nor a root node */
	else{
		/* Keep listening, forwar8012, query, receive answer, forward answer and repeat */

		/* Get query from the parent; basically keep waiting for the query*/
		while(1){
			std::cout << "I am neither a leaf nor a root" << std::endl;
			n.get_message_from_parent(n.get_parent(NumPos, 2), 8012);

			/* Find all the children*/
			children = n.get_children(0, 2); /* Starting Node 0 and number of branches 2 */

			/* Iterate ovr all the children and Forward query */
			for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
				n.send_message_to_children(*it, 8012, query);
			}

			std::vector<std::string> results_received; 
			/* Iterate ovr all the children and Collect results of the query */
			for(std::vector<std::string>::iterator it = children.begin(); it != children.end(); ++it) {
				results_received.push_back(n.receive_message_from_children(*it, 8012));
			}
		}
	}	

}
