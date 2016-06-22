#include<iostream>

#include "../include/node.h"

using namespace std;

int main(){

	int NumPos = 0; /*Get this from the command line argument */

	string query = "What's your name";
	Node n = Node("Ip1", 8012, NumPos); /* Make NumPos configurable with command line argument */

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
				std::cout << "Sending query to "<<*it << std::endl;
				
			}

		break; /* This is just for testing */
		}
	}
	/* else see if it is a leaf node */
	else if(n.am_i_leaf(0,2)){
		/* If this is a leaf node don't send anything forward */
		/* Keep listening, find answer, forward answer and repeat */
	}	
	/* DO this if it neither a leaf nor a root node */
	else{
		/* Keep listening, forward query, receive answer, forward answer and repeat */

	}

}
