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
# Note: Some of the include dirs (like CURL) can automatically be found by CMake at the 'include_directories' step and do not have to be set here.
#       See https://cmake.org/cmake/help/v3.0/module/FindCURL.html
set(IRODS_VERSION "4.2.3")
set(IRODS_EXTERNALS_FULLPATH_CLANG /opt/irods-externals/clang3.8-0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")
set(BOOST_ROOT "/opt/irods-externals/boost1.60.0-0")
set(JANSSON_INCLUDE_DIRS /opt/irods-externals/jansson2.7-0/include/)
set(JANSSON_LIBRARIES /opt/irods-externals/jansson2.7-0/lib/)

# Set compiler options
# TODO: Understand what this does and why this is needed. I just copied this from UU to make it work
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
# TODO: Understand what this does and why this is needed. I just copied this from UU to make it work
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
# CMAKE_SOURCE_DIR is the directory where the CMakeLists.txt file is located
# We will create a bin/ directory within the build/ directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# Include the header files into the project
include_directories("${IRODS_EXTERNALS_FULLPATH_CLANG}/include/c++/v1")

find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})

find_package(Jansson REQUIRED)
include_directories(SYSTEM ${JANSSON_INCLUDE_DIRS})

include_directories("/usr/include/irods")


# Generate the shared Library (= binary) from the source code
# Note: unlike an Executable, a library does not contain a main method. It just contains the classes
add_library(msi_getenv                SHARED msi_getenv/msi_getenv.cpp)
add_library(msi_http_send_file        SHARED msi_http_send_file/msi_http_send_file.cpp)
add_library(msi_json_arrayops         SHARED msi_json_arrayops/libmsi_json_arrayops.cpp)
add_library(msi_json_objops           SHARED msi_json_objops/libmsi_json_objops.cpp)
add_library(msi_time_ts2str           SHARED msi_time_ts2str/libmsi_time_ts2str.cpp)
add_library(msi_unmount               SHARED msi_unmount/unmount.cpp)
add_library(msiput_dataobj_or_coll    SHARED msiput_dataobj_or_coll/libmsiput_dataobj_or_coll.cpp)

# Link the generated library to dependency libraries
target_link_libraries(msi_getenv              LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_http_send_file      LINK_PUBLIC ${Boost_LIBRARIES} ${CURL_LIBRARIES})
target_link_libraries(msi_json_arrayops       LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(msi_json_objops         LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(msi_time_ts2str         LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_unmount             LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msiput_dataobj_or_coll  LINK_PUBLIC ${Boost_LIBRARIES})


#Set the location for library installation
install(CODE "MESSAGE(\"Installing microservices to iRODS plugins folder...\")")
install(TARGETS
        msi_getenv
        msi_http_send_file
        msi_json_arrayops
        msi_json_objops
        msi_time_ts2str
        msi_unmount
        msiput_dataobj_or_coll
        DESTINATION
        /usr/lib/irods/plugins/microservices)

