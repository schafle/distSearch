cmake_minimum_required (VERSION 2.6)
project (DistrSearch)
#option(RELEASE "Building in Debug mode" OFF)
#if (NOT RELEASE)
#	MESSAGE( STATUS "Building in debug mode")
set(CMAKE_BUILD_TYPE Debug)
#	add_definitions(-DELPP_THREAD_SAFE) 
#	add_definitions(-DINITIALIZE_EASYLOGGINGPP)
#else()
#	MESSAGE( STATUS "Building in release mode")
#	set(CMAKE_BUILD_TYPE Release)
#	add_definitions(-DELPP_DISABLE_LOGS)
#endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # for gcc >= 4.7, or c++0x for 4.6

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

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
LINK_DIRECTORIES("/home/ubuntu/distSearch/ext/LucenePlusPlus/build/src/core/")

include_directories(
	"include/lucene"
	)

include_directories(
	"include"
	"ext/logger"
	)
include_directories(
	${demo_headers}
	)

set(PROJECT_SOURCE_DIR "")
set(TCPIP_SOURCE_DIR "src/tcpip")
set(LUCENE_SOURCE_DIR "src/lucene")
set(PROJECT_INCLUDE_DIR "include/tcpip")
set(PROJECT_BINARY_DIR "bin")


#ServerForTree
add_executable("binServer" "main.cpp" "src/node.cpp" "src/error.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "src/doPagingSearch.cpp" "src/query.cpp")

TARGET_LINK_LIBRARIES( "binServer"
	pthread boost_system lucene++
)


add_executable("binStarServer" "star.cpp" "src/node.cpp" "src/error.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "src/doPagingSearch.cpp" "src/query.cpp")


#ServerForStar
TARGET_LINK_LIBRARIES( "binStarServer"
	pthread boost_system lucene++
)

include_directories(
	${PROJECT_INCLUDE_DIR}
	)

#Client
add_executable("${PROJECT_BINARY_DIR}Client" "${TCPIP_SOURCE_DIR}/client.cpp" "${TCPIP_SOURCE_DIR}/tcpstream.cpp" "${TCPIP_SOURCE_DIR}/tcpconnector.cpp" "${TCPIP_SOURCE_DIR}/tcpacceptor.cpp")

target_link_libraries( "${PROJECT_BINARY_DIR}Client"
	boost_system
)
