cmake_minimum_required (VERSION 2.6)
project (DistrSearch)

# The version number.
# set (DistSearch_VERSION_MAJOR 1)
# set (DistSearch_VERSION_MINOR 0)

set(PROJECT_SOURCE_DIR "src")
set(PROJECT_BINARY_DIR "bin")

add_executable("${PROJECT_BINARY_DIR}ServerBin" "${PROJECT_SOURCE_DIR}/server.cpp")
add_executable("${PROJECT_BINARY_DIR}ClientBin" "${PROJECT_SOURCE_DIR}/client.cpp")
