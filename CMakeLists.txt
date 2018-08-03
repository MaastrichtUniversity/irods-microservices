# We are very grateful to Utrecht University for inspiration on CMake procedures
# https://github.com/UtrechtUniversity/irods-uu-microservices (feature branch 4.2.3)

# Note: Microservices for iRODS >= 4.2 need to be built with the compiler and
# standard library provided by iRODS-externals packages.
# The following packages need to be installed to build the irods-microservices:
#
# irods-externals-clang-runtime3.8-0
# irods-externals-clang3.8-0

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

# Set global CMAKE variables
# TODO: Determine what is the preferred way of doing this: setting the values here vs. using ENV vars vs. providing them with the -D flag in cmake call
set(IRODS_VERSION "4.2.3")
set(IRODS_EXTERNALS_FULLPATH_CLANG /opt/irods-externals/clang3.8-0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")
set(BOOST_ROOT "/opt/irods-externals/boost1.60.0-0")
#set(Boost_INCLUDE_DIR /opt/irods-externals/boost1.60.0-0/include/)
#set(Boost_LIBRARIES /opt/irods-externals/boost1.60.0-0/lib/)
set(JANSSON_INCLUDE_DIRS /opt/irods-externals/jansson2.7-0/include/)
set(JANSSON_LIBRARIES /opt/irods-externals/jansson2.7-0/lib/)

# Set compiler options
# TODO: Understand that this does and why this is needed. I just copied this from UU to make it work
add_compile_options(-std=c++14
                    -Os
                    -fPIC
                    -DRODS_SERVER
                    -nostdinc++
                    -Wall
                    -Wextra
                    -Wpedantic
                    -Wcast-align
                    -Wredundant-decls
                    -Wuninitialized
                    -Wconversion
                    -Wno-missing-field-initializers
                    -Wno-unused-parameter)

# Use shared libraries in this project
# TODO: Understand that this does and why this is needed. I just copied this from UU to make it work
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
link_libraries(c++abi)

# Specify which C and C++ compiler to use
set(CMAKE_C_COMPILER ${IRODS_EXTERNALS_FULLPATH_CLANG}/bin/clang)
set(CMAKE_CXX_COMPILER ${IRODS_EXTERNALS_FULLPATH_CLANG}/bin/clang++)

# Set the project's name and which compiler(s) to use
project(irods-microservices C CXX)

# Check installed iRODS version
find_package(IRODS ${IRODS_VERSION} EXACT REQUIRED)
message(STATUS "Building for irods-server version ${IRODS_VERSION}")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# Include the header files into the project
include_directories("${IRODS_EXTERNALS_FULLPATH_CLANG}/include/c++/v1")

find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

#find_package(CURL REQUIRED)
#include_directories(SYSTEM ${CURL_INCLUDE_DIRS})

#find_package(LibXml2 REQUIRED)
#include_directories(SYSTEM ${LIBXML2_INCLUDE_DIR})

#find_package(LibXslt REQUIRED)
#include_directories(SYSTEM ${LIBXSLT_INCLUDE_DIR})

find_package(Jansson REQUIRED)
include_directories(SYSTEM ${JANSSON_INCLUDE_DIRS})

include_directories("/usr/include/irods")


# Generate the shared Library (= binary) from the source code
# Note: unlike an Executable, a library does not contain a main method. It just contains the classes
add_library(msi_json_arrayops         SHARED msi_json_arrayops/libmsi_json_arrayops.cpp)
#add_library(msi_json_objops           SHARED msi_json_objops/libmsi_json_objops.cpp)
#add_library(msi_http_send_file        SHARED msi_http_send_file/msi_http_send_file.cpp)

# Link the generated library to dependency libraries
target_link_libraries(msi_json_arrayops       LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(msi_json_objops         LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(msi_http_send_file      LINK_PUBLIC ${Boost_LIBRARIES})


#Set the location for library installation
#install(TARGETS
#        msi_json_arrayops
#        msi_json_objops
#        msi_http_send_file
#        DESTINATION
#        /usr/lib/irods/plugins/microservices)

