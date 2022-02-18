cmake_minimum_required(VERSION 3.16)
project(Base)

add_library(libserver_base 
AsyncLogging.cc
FileUtil.cc
LogFile.cc
Logging.cc
LogStream.cc)
add_library(sub::liblog ALIAS libserver_base)
target_include_directories(libserver_base 
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/../
)
target_link_libraries(libserver_base pthread rt)
set_target_properties(libserver_base PROPERTIES OUTPUT_NAME "server_base")