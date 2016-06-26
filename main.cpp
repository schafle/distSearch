// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include"query.h"

int main(int argc, char* argv[]){
	std::string index_location = argv[1];
	int a = make_query(index_location);
}

