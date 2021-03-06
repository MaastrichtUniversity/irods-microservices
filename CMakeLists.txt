# We are very grateful to Utrecht University for inspiration on CMake procedures
# https://github.com/UtrechtUniversity/irods-uu-microservices (feature branch 4.2.3)

# Note: Microservices for iRODS >= 4.2 need to be built with the compiler and
# standard library provided by iRODS-externals packages.
# The following packages need to be installed to build the irods-microservices:
#
# irods-externals-clang-runtime3.8-0
# irods-externals-clang3.8-0
# irods-externals

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.3.0 FATAL_ERROR)

# Set global CMAKE variables
# Note: Some of the include dirs (like CURL) can automatically be found by CMake at the 'include_directories' step and do not have to be set here.
#       See https://cmake.org/cmake/help/v3.0/module/FindCURL.html
set(MSI_RELEASE_VERSION "2.0.0")

set(IRODS_VERSION $ENV{IRODS_VERSION}) # Get iRODS version from environment
#set(IRODS_VERSION "4.2.4")            # Or specificy iRODS version manually


# Set additional modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")

# Set some paths for dependencies from irods-externals
set(BOOST_ROOT "/opt/irods-externals/boost1.60.0-0")
set(IRODS_EXTERNALS_FULLPATH_CLANG /opt/irods-externals/clang3.8-0)
set(JANSSON_ROOT /opt/irods-externals/jansson2.7-0)

# Set compiler options
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
                    -Wno-unused-parameter
                    -Wno-gnu-zero-variadic-macro-arguments)

# Use shared libraries in this project
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
link_libraries(c++abi)

# Specify which C and C++ compiler to use
set(CMAKE_C_COMPILER ${IRODS_EXTERNALS_FULLPATH_CLANG}/bin/clang)
set(CMAKE_CXX_COMPILER ${IRODS_EXTERNALS_FULLPATH_CLANG}/bin/clang++)

# Set the project's name and which compiler(s) to use
project(irods-microservices C CXX)

# Check installed iRODS version
find_package(IRODS ${IRODS_VERSION} EXACT REQUIRED)
message(STATUS "Building microservices version ${MSI_RELEASE_VERSION} for irods-server version ${IRODS_VERSION}")

# Include the header files into the project
include_directories("${IRODS_EXTERNALS_FULLPATH_CLANG}/include/c++/v1")

find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})

find_package(Jansson REQUIRED)
include_directories(SYSTEM ${JANSSON_INCLUDE_DIRS})

find_package(Rabbitmq REQUIRED)
include_directories(SYSTEM ${RABBITMQ_INCLUDE_DIRS} )

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
add_library(msi_amqp_basic_publish    SHARED msi_amqp_basic_publish/msi_amqp_basic_publish.cpp)
add_library(msi_set_avu               SHARED msi_set_avu/msi_set_avu.cpp)
add_library(msi_add_avu               SHARED msi_add_avu/msi_add_avu.cpp)
add_library(msi_rmw_avu               SHARED msi_rmw_avu/msi_rmw_avu.cpp)

# Link the generated library to dependency libraries
target_link_libraries(msi_getenv              LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_http_send_file      LINK_PUBLIC ${Boost_LIBRARIES} ${CURL_LIBRARIES})
target_link_libraries(msi_json_arrayops       LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(msi_json_objops         LINK_PUBLIC ${JANSSON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(msi_time_ts2str         LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_unmount             LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msiput_dataobj_or_coll  LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_amqp_basic_publish  LINK_PUBLIC ${Boost_LIBRARIES} ${RABBITMQ_LIBRARIES})
target_link_libraries(msi_set_avu             LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_add_avu             LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(msi_rmw_avu             LINK_PUBLIC ${Boost_LIBRARIES})


# Set the location for library installation
install(CODE "MESSAGE(\"Installing microservices to iRODS plugins folder...\")")
install(TARGETS
        msi_getenv
        msi_http_send_file
        msi_json_arrayops
        msi_json_objops
        msi_time_ts2str
        msi_unmount
        msiput_dataobj_or_coll
        msi_amqp_basic_publish
        msi_set_avu
        msi_add_avu
        msi_rmw_avu
        DESTINATION
        /usr/lib/irods/plugins/microservices)


# Packaging {{{

# Make sure your umask is set correctly when packaging:
# The default umask on CentOS 7 is 0002, which results in e.g. rwxrwxr-x
# directories within generated RPMs, which will conflict with existing
# directories from irods packages, which will be rwxr-xr-x.
# To create packages on CentOS 7, set your umask to 0022 first (`umask 0022`).

set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")

# Run custom CMake module to detect OS family
include(${CMAKE_MODULE_PATH}/DetectOS.cmake)
message(STATUS "Found Linux distro ${DETECTED_OS}")

# Determine which type of package should be generated
if(DETECTED_OS MATCHES "^(Ubuntu|Debian)$")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_VERSION "${IRODS_VERSION}-${MSI_RELEASE_VERSION}")
elseif(DETECTED_OS MATCHES "^(CentOS|RedHatCompatible)$")
    set(CPACK_GENERATOR "RPM")
    set(CPACK_PACKAGE_VERSION "${IRODS_VERSION}_${MSI_RELEASE_VERSION}")
endif()
message(STATUS "Using CPACK generator ${CPACK_GENERATOR}")

set(CPACK_PACKAGE_NAME "rit-irods-microservices")
set(CPACK_PACKAGE_VENDOR "DataHub Maastricht University <datahub@maastrichtuniversity.nl>")
set(CPACK_PACKAGE_CONTACT "DataHub Maastricht University <datahub@maastrichtuniversity.nl>")

#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/packaging/description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "iRODS microservices for the RIT project")

set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "LGPLv3")
set(CPACK_RPM_PACKAGE_REQUIRES "irods-server = ${IRODS_VERSION}, irods-runtime = ${IRODS_VERSION}, libcurl >= 7.29.0, libxml2 >= 2.9.1, irods-externals-jansson2.7-0 >= 1.0")
set(CPACK_RPM_PACKAGE_CONFLICTS "irods-uu-microservices")
set(CPACK_RPM_PACKAGE_URL "https://github.com/MaastrichtUniversity/rit-irods-microservices")
set(CPACK_RPM_PACKAGE_AUTOREQ 0)
set(CPACK_RPM_PACKAGE_AUTOPROV 0)

set(CPACK_DEBIAN_PACKAGE_DEPENDS "irods-server (= ${IRODS_VERSION}), irods-runtime (= ${IRODS_VERSION}), libcurl3 (>= 7.29.0), libxml2 (>= 2.9.1), irods-externals-jansson2.7-0")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/MaastrichtUniversity/rit-irods-microservices")
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/science")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}")

include(CPack)

add_custom_target(rit-package
  COMMAND ${CMAKE_CPACK_COMMAND}
  COMMAND mkdir -p /microservices/packages/

  # TODO: This is not very pretty. I couldn't find a way to get the resulting deb or rpm name in reliable way
  COMMAND mv ${CMAKE_CURRENT_BINARY_DIR}/rit-irods-microservices* /microservices/packages/.
)

# }}}

