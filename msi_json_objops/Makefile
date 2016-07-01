GCC = g++

INC=-I/usr/include/irods/ -I/usr/include/irods/boost/ -I/usr/include/irods/jansson/src

all: libmsi_json_objops

libmsi_json_objops:
	${GCC} ${INC} -DRODS_SERVER -fPIC "-Wl,-E" -shared -g -Wno-deprecated -rdynamic -o $@.so $@.cpp /usr/lib/libirods_client.a /usr/lib/irods/externals/libjansson.a
     

clean:
	@rm -f ./*.so
