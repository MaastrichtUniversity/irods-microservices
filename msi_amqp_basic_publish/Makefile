# TODO: Make this msi compatible with CMake building in 4.2.x

GCC=g++
INC=-I/usr/include/irods/ -I/usr/include/irods/boost/

all: msi_amqp_basic_publish

msi_amqp_basic_publish:
	${GCC} ${INC} -DRODS_SERVER -fPIC "-Wl,-E" -shared -g -Wno-deprecated -rdynamic -o lib$@.so $@.cpp -lrabbitmq /usr/lib/libirods_client.a

clean:
	@rm -f ./*.so
