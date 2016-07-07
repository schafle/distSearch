/*
   client.cpp

   Test client for the tcpsockets classes. 

   ------------------------------------------

   Copyright © 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

#include "tcpconnector.h"
#include "tcpacceptor.h"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    int len;
    //string message;
    char line[256];
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
    if (stream) {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	std::string uuid_string = boost::uuids::to_string(uuid);
	std::cout << uuid_string << std::endl;
	std::cout << "Enter your search term here" << std::endl;
	std::cin.getline (line,256);
	std::string message(uuid_string + "==>" + line);
	cout << message.size() << std::endl;
        stream->send(message.c_str(), message.size());
        std::cout << message << std::endl;
	printf("sent - %s\n", message.c_str());
	LOG(INFO) << message ;
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        delete stream;
    }
	
	std::cout << "waiting for results" << std::endl;
	TCPAcceptor* acceptor = NULL;
        acceptor = new TCPAcceptor(3035);
        std::string received;
        if (acceptor->start() == 0) {
                stream = acceptor->accept();
                if (stream != NULL) {
                        ssize_t len;
                        char line[256];
                        if ((len = stream->receive(line, sizeof(line))) > 0) {
                                line[len] = 0;
                                received = string(line);
                                std::cout << received << std::endl;
                        }
                }
                delete stream;
        }


    exit(0);
}
