# This CMake file tries to find the the RabbitMQ library
# The following variables are set:
#   RABBITMQ_FOUND - System has RabbitMQ client
#   RABBITMQ_LIBRARIES - The RabbitMQ client library
#   RABBITMQ_INCLUDE_DIRS - The RabbitMQ client INCLUDE_DIRS

include(CheckCSourceCompiles)
find_library(RABBITMQ_LIBRARIES NAMES rabbitmq)
find_path(RABBITMQ_INCLUDE_DIRS amqp.h)

if(${RABBITMQ_LIBRARIES} MATCHES "NOTFOUND")
  set(Rabbitmq_FOUND FALSE CACHE INTERNAL "")
  message(STATUS "RabbitMQ library not found.")
  unset(RABBITMQ_LIBRARIES)
else()
  set(Rabbitmq_FOUND TRUE CACHE INTERNAL "")
  message(STATUS "Found RabbitMQ library: ${RABBITMQ_LIBRARIES}")
endif()

set(CMAKE_REQUIRED_INCLUDES ${RABBITMQ_INCLUDE_DIRS})