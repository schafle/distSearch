cmake_minimum_required (VERSION 2.6)
project (DistrSearch)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # for gcc >= 4.7, or c++0x for 4.6

# The version number.
# set (DistSearch_VERSION_MAJOR 1)
# set (DistSearch_VERSION_MINOR 0)

#file(GLOB_RECURSE
#	demo_headers
#	"ext/LucenePlusPlus/include*.h"
#	)

add_definitions(-DLPP_HAVE_DLL)
find_package(Boost REQUIRED)

#include_directories(
#	${Boost_INCLUDE_DIRS}
#	)
#include_directories(
#	"ext/LucenePlusPlus/include"
#	)

#include_directories(
#	"ext/LucenePlusPlus/build/include"
#	)


#include_directories(
#	"include/lucene"
#	)

include_directories(
	"include"
	)
include_directories(
	${demo_headers}
	)

set(PROJECT_SOURCE_DIR "")
set(TCPIP_SOURCE_DIR "src/tcpip")
#set(LUCENE_SOURCE_DIR "src/lucene")
set(PROJECT_INCLUDE_DIR "include/tcpip")
set(PROJECT_BINARY_DIR "bin")


add_executable("binServer" "main.cpp" "src/node.cpp" "src/error.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp")

TARGET_LINK_LIBRARIES( "binServer"
pthread
)


add_executable("binStarServer" "star.cpp" "src/node.cpp" "src/error.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp")


TARGET_LINK_LIBRARIES( "binStarServer"
pthread
)
#target_link_libraries( "${PROJECT_BINARY_DIR}searchfiles"
#	lucene++ boost_system
#)

include_directories(
	${PROJECT_INCLUDE_DIR}
	)

#Server 
#add_executable("${PROJECT_BINARY_DIR}Server" "${TCPIP_SOURCE_DIR}/server.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp")	

#Client
add_executable("${PROJECT_BINARY_DIR}Client" "${TCPIP_SOURCE_DIR}/client.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp")

#Server_timeout
#add_executable("${PROJECT_BINARY_DIR}ServerTimeout" "${TCPIP_SOURCE_DIR}/server_timeout.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp")

#Client_timeout
#add_executable("${PROJECT_BINARY_DIR}ClientTimeout" "${TCPIP_SOURCE_DIR}/client_timeout.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp")

#Server_pause
#add_executable("${PROJECT_BINARY_DIR}ServerPause" "${TCPIP_SOURCE_DIR}/server_pause.cpp")
