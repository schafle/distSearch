// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include"query.h"
#include "node.h"
int main(int argc, char* argv[]){
	if(argc < 5){ 
		std::cout << "Insufficient arguments to run" << std::endl;
		std::cout << "./executable [indexDirectory] [positionInCluster] [HostName] [FileName]" << std::endl;
		exit (EXIT_FAILURE);
	}

	std::string index_location = argv[1];
	int posNum = atoi(argv[2]);
	std::string HostName = argv[3];
	std::string NodeDetailsFile = argv[4];
	
	int portNumber = 3033;

	/* Create a node object */
	Node currentNode = Node( HostName, portNumber, posNum, NodeDetailsFile);

	//std::cout << currentNode.get_children(0,2)[0] << std::endl;
	/* Make query irrespective of anything on the current node */
	int a = make_query(index_location);

}

