GCC=g++
INC=-I/usr/include/irods/ -I/usr/include/irods/boost/

all: msi_getenv

msi_getenv:
	${GCC} ${INC} -DRODS_SERVER -fPIC "-Wl,-E" -shared -g -Wno-deprecated -rdynamic -o lib$@.so $@.cpp `curl-config --libs` /usr/lib/libirods_client.a

clean:
	@rm -f ./*.so
