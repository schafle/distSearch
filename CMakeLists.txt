cmake_minimum_required (VERSION 2.6)
project (DistrSearch)

# The version number.
# set (DistSearch_VERSION_MAJOR 1)
# set (DistSearch_VERSION_MINOR 0)

file(GLOB_RECURSE
	demo_headers
	"ext/LucenePlusPlus/include*.h"
	)

add_definitions(-DLPP_HAVE_DLL)
find_package(Boost REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	)
include_directories(
	"ext/LucenePlusPlus/include"
	)

include_directories(
	"ext/LucenePlusPlus/build/include"
	)

add_executable(searchfiles
	"main.cpp"
	${demo_headers}
	)
target_link_libraries(searchfiles
	lucene++ boost_system
	)

set(PROJECT_SOURCE_DIR "src/tcpip")
set(PROJECT_INCLUDE_DIR "include/tcpip")
set(PROJECT_BINARY_DIR "bin")

include_directories(
	${PROJECT_INCLUDE_DIR}
	)

#Server 
add_executable("${PROJECT_BINARY_DIR}Server" "${PROJECT_SOURCE_DIR}/server.cpp" "${PROJECT_SOURCE_DIR}/tcpstream.cpp" "${PROJECT_SOURCE_DIR}/tcpacceptor.cpp" "${PROJECT_SOURCE_DIR}/tcpconnector.cpp")	

#Client
add_executable("${PROJECT_BINARY_DIR}Client" "${PROJECT_SOURCE_DIR}/client.cpp" "${PROJECT_SOURCE_DIR}/tcpstream.cpp" "${PROJECT_SOURCE_DIR}/tcpconnector.cpp")

#Server_timeout
add_executable("${PROJECT_BINARY_DIR}ServerTimeout" "${PROJECT_SOURCE_DIR}/server_timeout.cpp" "${PROJECT_SOURCE_DIR}/tcpstream.cpp" "${PROJECT_SOURCE_DIR}/tcpacceptor.cpp" "${PROJECT_SOURCE_DIR}/tcpconnector.cpp")

#Client_timeout
add_executable("${PROJECT_BINARY_DIR}ClientTimeout" "${PROJECT_SOURCE_DIR}/client_timeout.cpp" "${PROJECT_SOURCE_DIR}/tcpstream.cpp" "${PROJECT_SOURCE_DIR}/tcpconnector.cpp")

#Server_pause
add_executable("${PROJECT_BINARY_DIR}ServerPause" "${PROJECT_SOURCE_DIR}/server_pause.cpp")
